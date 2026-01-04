import os
from .sprite_utils import SpriteProcessor, parse_pointer, extract_palette, render_sprite


class TrainerSpriteProcessor(SpriteProcessor):
    """Trainer sprite processor"""
    
    def __init__(self):
        super().__init__(
            json_file="dex_files/pointers/json/trainerSprites.json",
            elf_file="pokeemerald-test.elf",
            output_dir="dex_files/sprites/trainers"
        )
    
    def setup_directories(self):
        """Create output directories"""
        os.makedirs(self.output_dir, exist_ok=True)
    
    def process_sprites(self, target_ids=None):
        """Process trainer sprites"""
        self.setup_directories()
        trainer_sprites = self.load_data()
        
        # Deduplicate sprites by pic ID
        unique_sprites = {}
        for sprite_data in trainer_sprites:
            pic_id = sprite_data.get("trainerPic")
            if pic_id is not None and pic_id not in unique_sprites:
                unique_sprites[pic_id] = sprite_data
        
        for pic_id, sprite_data in unique_sprites.items():
            # Filter by target IDs if specified
            if target_ids and pic_id not in target_ids:
                continue
                
            print(f"Processing trainer sprite (pic ID #{pic_id})...")
            
            # Get pointers
            sprite_ptr = parse_pointer(sprite_data.get("frontSprite pointer"))
            palette_ptr = parse_pointer(sprite_data.get("palette pointer"))
            
            if not sprite_ptr or not palette_ptr:
                print(f"  Missing pointers for pic ID {pic_id}")
                continue
            
            # Get sprite data
            sprite_data_bytes = self.get_sprite_data(self.elf_file, sprite_ptr)
            if not sprite_data_bytes:
                print(f"  Failed to read sprite data for pic ID {pic_id}")
                continue
            
            # Get palette
            palette = extract_palette(self.elf_file, palette_ptr)
            if not palette:
                print(f"  Failed to extract palette for pic ID {pic_id}")
                continue
            
            # Try different sizes
            self._process_trainer_sprite(pic_id, sprite_data_bytes, palette)
    
    def _process_trainer_sprite(self, pic_id, sprite_data, palette):
        """Process a single trainer sprite with multiple size attempts"""
        possible_sizes = [(64, 64), (80, 80)]
        
        for width, height in possible_sizes:
            single_frame_size = (width * height) // 2
            
            if len(sprite_data) >= single_frame_size:
                try:
                    image = render_sprite(sprite_data[:single_frame_size], palette, width, height)
                    output_path = os.path.join(self.output_dir, f"{pic_id}.png")
                    image.save(output_path)
                    print(f"  Saved trainer sprite ({width}x{height}) to {output_path}")
                    return  # Success, exit early
                except Exception as e:
                    print(f"  Error rendering {width}x{height} sprite: {e}")
                    continue
        
        print(f"  Failed to render sprite for pic ID {pic_id} with any size")


def main():
    import sys
    
    processor = TrainerSpriteProcessor()
    
    # Parse command line arguments for specific IDs
    target_ids = None
    if len(sys.argv) > 1:
        try:
            target_ids = [int(arg) for arg in sys.argv[1:]]
            print(f"Processing specific trainer pic IDs: {target_ids}")
        except ValueError:
            print("Invalid ID format. Use: python process_trainer_sprites.py [id1] [id2] ...")
            sys.exit(1)
    
    processor.process_sprites(target_ids)


if __name__ == "__main__":
    main()
