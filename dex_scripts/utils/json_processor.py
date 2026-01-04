
import json
import argparse
from pathlib import Path
from utils.key_json import JSONKeyProcessor



class JsonProcessor:
    def __init__(self, input_path=None, output_path=None, data=None, filename=None):
        self.data = data
        if input_path is None and data is None:
            raise ValueError("Either input_path or data must be provided.")
        self.script_dir = Path(__file__).parent.absolute()
        self.project_root = self.find_project_root(self.script_dir)
        self.dex_dir = self.project_root / "dex_files"
        self.processed_dir = self.dex_dir / "processed"
        self.processed_dir.mkdir(parents=True, exist_ok=True)
        if input_path is not None:
            self.input_filename = Path(input_path).name
            self.input_path = input_path
        elif data is not None:
            if filename:
                self.input_filename = f"{filename}.json"
            else:
                self.input_filename = "processed.json"
            self.input_path = None
        self.output_path = output_path or (self.processed_dir / self.input_filename)

    @staticmethod
    def load_json(filename):
        with open(filename, 'r', encoding='utf-8') as f:
            return json.load(f)

    @staticmethod
    def find_project_root(start_dir: Path) -> Path:
        """Find project root by looking for Makefile, starting from start_dir."""
        current = start_dir
        while current != current.parent:
            if (current / "Makefile").exists():
                return current
            current = current.parent
        raise FileNotFoundError("Could not find Makefile")

    def process(self):
        if self.data is not None:
            data = self.data
        else:
            data = self.load_json(self.input_path)
        processor = JSONKeyProcessor(data)
        processed_data = processor.process()  # Print info
        with open(self.output_path, 'w', encoding='utf-8') as f:
            json.dump(processed_data, f, ensure_ascii=False, indent=2)
        print(f'JSON processed and saved to {self.output_path}')

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='Process a JSON file.')
    parser.add_argument('input', help='Path to the input JSON file')
    args = parser.parse_args()
    JsonProcessor(input_path=args.input).process()
