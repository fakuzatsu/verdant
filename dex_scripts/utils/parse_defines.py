import re
import json
from pathlib import Path

class DefineParser:
    def __init__(self, species_file, output_dir):
        self.species_file = species_file
        self.output_dir = output_dir
        self.define_re = re.compile(r'#define\s+(\w+)\s+(.+)')
        self.defines = {}
        self.resolved = {}

    @staticmethod
    def find_project_root() -> Path:
        """Find project root by looking for Makefile from this script's directory upwards"""
        current = Path(__file__).parent
        while current != current.parent:
            if (current / "Makefile").exists():
                return current
            current = current.parent
        raise FileNotFoundError("Could not find Makefile")

    def parse_defines(self):
        with open(self.species_file) as f:
            for line in f:
                m = self.define_re.match(line)
                if m:
                    key, value = m.groups()
                    self.defines[key] = value.strip()

    def resolve(self, key, seen=None):
        if seen is None:
            seen = set()
        if key in self.resolved:
            return self.resolved[key]
        if key in seen:
            raise ValueError(f"Circular define: {key}")
        seen.add(key)
        value = self.defines[key]
        if value.isdigit():
            self.resolved[key] = int(value)
            return int(value)
        elif value in self.defines:
            num = self.resolve(value, seen)
            self.resolved[key] = num
            return num
        else:
            tokens = re.findall(r'\b\w+\b', value)
            expr = value
            for token in set(tokens):
                if token in self.defines:
                    expr = re.sub(rf'\b{token}\b', str(self.resolve(token, seen.copy())), expr)
            try:
                num = eval(expr)
                self.resolved[key] = num
                return num
            except Exception:
                self.resolved[key] = value
                return value

    def build_mappings(self):
        for key in self.defines:
            self.resolve(key)
        const_to_num = {k: v for k, v in self.resolved.items() if isinstance(v, int)}
        num_to_const = {}
        for k, v in const_to_num.items():
            if v not in num_to_const:
                num_to_const[v] = k
        return const_to_num, num_to_const

    def dump_json(self, const_to_num, num_to_const):
        speciesLabelMap = {str(num): name for num, name in num_to_const.items()}
        labelSpeciesMap = const_to_num
        self.output_dir.mkdir(parents=True, exist_ok=True)
        with open(self.output_dir / "speciesLabelMap.json", "w") as f:
            json.dump(speciesLabelMap, f, indent=2)
        with open(self.output_dir / "labelSpeciesMap.json", "w") as f:
            json.dump(labelSpeciesMap, f, indent=2)

def main():
    project_root = DefineParser.find_project_root()
    species_file = project_root / "include/constants/species.h"
    output_dir = project_root / "dex_files/json"
    parser = DefineParser(species_file, output_dir)
    parser.parse_defines()
    const_to_num, num_to_const = parser.build_mappings()
    parser.dump_json(const_to_num, num_to_const)

if __name__ == "__main__":
    main()
