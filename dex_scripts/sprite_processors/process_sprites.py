import os
from .sprite_utils import SpriteProcessor, parse_pointer, extract_palette, render_sprite


class PokemonSpriteProcessor(SpriteProcessor):
    """Pokemon sprite processor"""
    
    def __init__(self):
        super().__init__(
            json_file="dex_files/pointers/json/pokemonSprites.json",
            elf_file="pokeemerald-test.elf",
            output_dir="dex_files/sprites"
        )
        
    def setup_directories(self):
        """Create output directories"""
        os.makedirs(self.output_dir, exist_ok=True)
        os.makedirs(os.path.join(self.output_dir, "front"), exist_ok=True)
        os.makedirs(os.path.join(self.output_dir, "front_shiny"), exist_ok=True)
        os.makedirs(os.path.join(self.output_dir, "anim_front"), exist_ok=True)
        os.makedirs(os.path.join(self.output_dir, "anim_front_shiny"), exist_ok=True)
    
    def process_sprites(self, target_ids=None):
        """Process Pokemon sprites"""
        self.setup_directories()
        sprites = self.load_data()
        
        for pokemon in sprites:
            if "index" not in pokemon or "speciesName" not in pokemon:
                continue
                
            index = pokemon["index"]
            name = pokemon["speciesName"]
            
            # Filter by target IDs if specified
            if target_ids and index not in target_ids:
                continue
                
            print(f"Processing {name} (#{index})...")
            
            # Get pointers
            sprite_ptr = parse_pointer(pokemon.get("frontSprite pointer"))
            normal_palette_ptr = parse_pointer(pokemon.get("normalPalette pointer"))
            shiny_palette_ptr = parse_pointer(pokemon.get("shinyPalette pointer"))
            
            if not sprite_ptr or not normal_palette_ptr:
                print(f"  Missing pointers for {name}")
                continue
            
            # Get sprite data
            sprite_data = self.get_sprite_data(self.elf_file, sprite_ptr)
            if not sprite_data:
                print(f"  Failed to read sprite data for {name}")
                continue
                
            # Get palettes
            normal_palette = extract_palette(self.elf_file, normal_palette_ptr)
            if not normal_palette:
                print(f"  Failed to extract normal palette for {name}")
                continue
            
            # Process sprites
            self._process_pokemon_sprites(index, sprite_data, normal_palette, shiny_palette_ptr)
    
    def _process_pokemon_sprites(self, index, sprite_data, normal_palette, shiny_palette_ptr):
        """Process sprites for a single Pokemon"""
        width = height = 64
        single_frame_size = (width * height) // 2
        
        # Normal sprite
        normal_image = render_sprite(sprite_data[:single_frame_size], normal_palette, width, height)
        normal_output_path = os.path.join(self.output_dir, "front", f"{index}.png")
        normal_image.save(normal_output_path)
        print(f"  Saved normal sprite to {normal_output_path}")
        
        # Animation frame
        anim_front_dir = os.path.join(self.output_dir, "anim_front")
        if len(sprite_data) >= single_frame_size * 2:
            anim_frame_data = sprite_data[single_frame_size : single_frame_size * 2]
            anim_image = render_sprite(anim_frame_data, normal_palette, width, height)
            anim_output_path = os.path.join(anim_front_dir, f"{index}.png")
            anim_image.save(anim_output_path)
            print(f"  Saved anim frame to {anim_output_path}")
        else:
            anim_output_path = os.path.join(anim_front_dir, f"{index}.png")
            normal_image.save(anim_output_path)
            print(f"  Copied normal frame to anim directory: {anim_output_path}")
        
        # Shiny variants
        if shiny_palette_ptr:
            shiny_palette = extract_palette(self.elf_file, shiny_palette_ptr)
            if shiny_palette:
                self._process_shiny_sprites(index, sprite_data, shiny_palette, single_frame_size)
    
    def _process_shiny_sprites(self, index, sprite_data, shiny_palette, single_frame_size):
        """Process shiny sprites for a Pokemon"""
        width = height = 64
        
        # Shiny sprite
        shiny_image = render_sprite(sprite_data[:single_frame_size], shiny_palette, width, height)
        shiny_output_path = os.path.join(self.output_dir, "front_shiny", f"{index}.png")
        shiny_image.save(shiny_output_path)
        print(f"  Saved shiny sprite to {shiny_output_path}")
        
        # Shiny animation frame
        anim_front_shiny_dir = os.path.join(self.output_dir, "anim_front_shiny")
        if len(sprite_data) >= single_frame_size * 2:
            anim_frame_data = sprite_data[single_frame_size : single_frame_size * 2]
            anim_shiny_image = render_sprite(anim_frame_data, shiny_palette, width, height)
            anim_shiny_output_path = os.path.join(anim_front_shiny_dir, f"{index}.png")
            anim_shiny_image.save(anim_shiny_output_path)
            print(f"  Saved shiny anim frame to {anim_shiny_output_path}")
        else:
            anim_shiny_output_path = os.path.join(anim_front_shiny_dir, f"{index}.png")
            shiny_image.save(anim_shiny_output_path)
            print(f"  Copied shiny frame to anim directory: {anim_shiny_output_path}")


def main():
    import sys
    
    processor = PokemonSpriteProcessor()
    
    # Parse command line arguments for specific IDs
    target_ids = None
    if len(sys.argv) > 1:
        try:
            target_ids = [int(arg) for arg in sys.argv[1:]]
            print(f"Processing specific Pokemon IDs: {target_ids}")
        except ValueError:
            print("Invalid ID format. Use: python process_sprites.py [id1] [id2] ...")
            sys.exit(1)
    
    processor.process_sprites(target_ids)


if __name__ == "__main__":
    main()
