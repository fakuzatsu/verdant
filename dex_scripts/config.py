OPTIONS = {
    "species": {
        "test": "Pokemon", 
        "output": "speciesData", 
        "is_pointer": False,
        "pre_processor": "species",
        "post_processor": "species"
        },

    "abilities": {
        "test": "Abilities", 
        "output": "abilityData", 
        "is_pointer": False,
        "pre_processor": "none",
        "post_processor": "json"
        },

    "items": {
        "test": "Items", 
        "output": "itemData", 
        "is_pointer": False,
        "pre_processor": "none",
        "post_processor": "json"
        },

    "moves": {
        "test": "Moves", 
        "output": "moveData", 
        "is_pointer": False,
        "pre_processor": "none",
        "post_processor": "json"
        },

    "trainers": {
        "test": "Trainers", 
        "output": "trainerData", 
        "is_pointer": False,
        "pre_processor": "none",
        "post_processor": "none"
        },

    "types": {
        "test": "Types", 
        "output": "typeData", 
        "is_pointer": False,
        "pre_processor": "none",
        "post_processor": "json"
        },

    "sprites": {
        "test": "Sprites", 
        "output": "pokemonSprites", 
        "is_pointer": True,
        "pre_processor": "none",
        "post_processor": "sprite"
        },

    "trainersprites": {
        "test": "Trainer Sprites", 
        "output": "trainerSprites", 
        "is_pointer": True,
        "pre_processor": "none",
        "post_processor": "trainer_sprite"
        },
}
