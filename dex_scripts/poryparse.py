#!/usr/bin/env python3
"""
Python utilities for dex_scripts pipeline
"""

import os
import subprocess
import json
import yaml
from pathlib import Path
from typing import Optional, Tuple
from config import OPTIONS
from sprite_processors import PokemonSpriteProcessor, TrainerSpriteProcessor
from utils import DefineParser, JsonProcessor, SpeciesPostProcessor
import argparse


class DexPipeline:
    def __init__(self):
        self.OPTIONS = OPTIONS
        self.script_dir = Path(__file__).parent.absolute()
        self.project_root = self._find_project_root()
        self.dex_dir = self.project_root / "dex_files"
        self.pointer_dir = self.dex_dir / "pointers"

    def _find_project_root(self) -> Path:
        """Find project root by looking for Makefile"""
        current = self.script_dir
        while current != current.parent:
            if (current / "Makefile").exists():
                return current
            current = current.parent
        raise FileNotFoundError("Could not find Makefile")

    def set_output_paths(self, base: str, is_pointer: bool) -> None:
        """Set output file paths"""
        if is_pointer:
            self.yaml_output = self.pointer_dir / "yaml" / f"{base}.yaml"
            self.json_output = self.pointer_dir / "json" / f"{base}.json"
        else:
            self.yaml_output = self.dex_dir / "yaml" / f"{base}.yaml"
            self.json_output = self.dex_dir / "json" / f"{base}.json"

    def create_directories(self) -> None:
        """Create output directories if needed"""
        self.yaml_output.parent.mkdir(parents=True, exist_ok=True)
        self.json_output.parent.mkdir(parents=True, exist_ok=True)

    def is_venv_active(self) -> None:
        """Check if virtual environment is active"""
        venv_dir = self.project_root / ".venv"
        if not venv_dir.exists() or not (venv_dir / "bin" / "activate").exists():
            raise FileNotFoundError(
                f"Python virtual environment not found at {venv_dir}"
            )

        # Check if already in virtual environment
        if not os.environ.get("VIRTUAL_ENV"):
            print("Warning: Virtual environment not active. Please activate it first.")
            # Note: We can't activate venv from within Python, user must do it

    def run_make_test(self, test_name: str) -> None:
        """Run make test and extract data between markers"""
        full_test_name = f"DEX: {test_name}"
        print(f"Running test: {full_test_name}")

        # Run make command
        cmd = ["make", "-j", "check", f"TESTS={full_test_name}"]
        result = subprocess.run(
            cmd, cwd=self.project_root, capture_output=True, text=True
        )

        if result.returncode != 0:
            print(f"Make command failed with return code {result.returncode}")
            print(f"stderr: {result.stderr}")
            raise subprocess.CalledProcessError(result.returncode, cmd)

        # Extract data between markers
        output_lines = result.stdout.split("\n")
        extracting = False
        extracted_lines = []

        for line in output_lines:
            if "===DEBUG OUTPUT START===" in line:
                extracting = True
                continue
            elif "===DEBUG OUTPUT END===" in line:
                extracting = False
                continue
            elif extracting:
                extracted_lines.append(line)

        # Write extracted YAML
        with open(self.yaml_output, "w") as f:
            f.write("\n".join(extracted_lines))

    def convert_yaml_to_json(self) -> None:
        """Convert YAML to JSON using Python yaml module"""
        print("Converting YAML to JSON...")

        try:
            with open(self.yaml_output, "r") as f:
                yaml_data = yaml.safe_load(f)

            with open(self.json_output, "w") as f:
                json.dump(yaml_data, f, ensure_ascii=False, indent=2)

        except yaml.YAMLError as e:
            print(f"YAML parsing error: {e}")
            # Fallback to yq if yaml module fails
            self._convert_with_yq()

    def _convert_with_yq(self) -> None:
        """Fallback: convert YAML to JSON using yq command"""
        if not self._command_exists("yq"):
            raise FileNotFoundError(
                "yq is not installed and Python yaml parsing failed"
            )

        print("Falling back to yq for YAML conversion...")
        cmd = ["yq", ".", str(self.yaml_output), "-j"]
        result = subprocess.run(cmd, capture_output=True, text=True)

        if result.returncode != 0:
            raise subprocess.CalledProcessError(result.returncode, cmd)

        with open(self.json_output, "w") as f:
            f.write(result.stdout)

    def _command_exists(self, command: str) -> bool:
        """Check if a command exists in PATH"""
        return subprocess.run(["which", command], capture_output=True).returncode == 0

    # def run_fixjson(self) -> None:
    #     """Run fixjson.py if it exists"""
    #     fixjson_script = self.dex_dir / "fixjson.py"
    #     if fixjson_script.exists():
    #         print("Running fixjson.py...")
    #         cmd = ["python", str(fixjson_script), str(self.json_output), str(self.fixed_json_output)]
    #         result = subprocess.run(cmd)
    #         if result.returncode != 0:
    #             print(f"fixjson.py failed with return code {result.returncode}")
    #     else:
    #         print("Warning: fixjson.py not found, skipping")

    def _validate_options(self, option: str) -> bool:
        """Check if the user provided options are valid"""
        return option.lower() in self.OPTIONS

    def run_pre_processor(self, pre_processor: str | None = None) -> None:
        """If specified, run pre processor"""
        if pre_processor == "species":
            processor = DefineParser(
                self.project_root / "include/constants/species.h",
                self.project_root / "dex_files/json",
            )
            processor.parse_defines()
            const_to_num, num_to_const = processor.build_mappings()
            processor.dump_json(const_to_num, num_to_const)

    def run_post_processor(self, post_processor: str | None = None) -> None:
        """Run post-processor if specified"""
        if post_processor == "sprite":
            processor = PokemonSpriteProcessor()
            processor.process_sprites()

        if post_processor == "trainer_sprite":
            processor = TrainerSpriteProcessor()
            processor.process_sprites()

        if post_processor == "json":
            processor = JsonProcessor(input_path=self.json_output)
            processor.process()

        if post_processor == "species":
            # Load the JSON data
            with open(self.json_output, "r", encoding="utf-8") as f:
                data = json.load(f)
            # Run the species post processor
            speciesData = SpeciesPostProcessor(data).process()
            # Pass processed data directly to JsonProcessor if possible
            processor = JsonProcessor(data=speciesData, filename="speciesData")
            processor.process()


    def run_core_pipeline(self, option: str) -> None:
        """Run the core pipeline: make test -> YAML -> JSON"""
        if not self._validate_options(option):
            raise ValueError(
                f"Valid options are: {list(self.OPTIONS.keys())}. You entered {option}"
            )

        test_name = OPTIONS[option]["test"]
        output_base = OPTIONS[option]["output"]
        is_pointer = OPTIONS[option]["is_pointer"]

        print("Running core pipeline...")
        print(f"Test: {test_name}")
        print(f"Output base: {output_base}")
        print(f"Is pointer?: {is_pointer}")

        # Initialize
        self.set_output_paths(output_base, is_pointer)
        self.create_directories()
        self.is_venv_active()

        print(f"YAML: {self.yaml_output}")
        print(f"JSON: {self.json_output}")
        print()

        # Core pipeline steps
        self.run_make_test(test_name)
        self.convert_yaml_to_json()

        print("Core pipeline complete!")


def main():

    parser = argparse.ArgumentParser(description="Data extractor utility")
    parser.add_argument("option", help="Which pipeline option to run (or 'all')")
    parser.add_argument(
        "-p", "--postprocess", action="store_true", help="Run post-processing step"
    )

    args = parser.parse_args()

    option = args.option

    if option == "all":
        # Collect all unique pre-processors (excluding None)
        preprocessors = set(
            OPTIONS[opt]["pre_processor"]
            for opt in OPTIONS.keys()
            if OPTIONS[opt].get("pre_processor")
        )
        # Run all unique pre-processors first
        for preproc in preprocessors:
            pipeline = DexPipeline()
            pipeline.run_pre_processor(preproc)
        # Then run the core pipeline and post-processors for each option
        for opt in OPTIONS.keys():
            pipeline = DexPipeline()
            pipeline.run_core_pipeline(opt)
            if args.postprocess:
                pipeline.run_post_processor(OPTIONS[opt]["post_processor"])

    else:
        pipeline = DexPipeline()
        pipeline.run_pre_processor(OPTIONS[option]["pre_processor"])
        pipeline.run_core_pipeline(option)
        if args.postprocess:
            pipeline.run_post_processor(OPTIONS[option]["post_processor"])


if __name__ == "__main__":
    main()
