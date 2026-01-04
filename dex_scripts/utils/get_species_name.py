def convert_species_define(species_define):
    """
    Converts a SPECIES_ define (e.g., 'SPECIES_HO_OH_ALOLAN') into a Showdown-compatible name.
    Returns the converted name as a string.
    Raises ValueError if the input is not a valid SPECIES_ define.
    """
    species_replacements = {
        "CHIEN_PAO": "Chien-Pao",
        "CHI_YU": "Chi-Yu",
        "HAKAMO_O": "Hakamo-o",
        "HO_OH": "Ho-Oh",
        "JANGMO_O": "Jangmo-o",
        "KOMMO_O": "Kommo-o",
        "PORYGON_Z": "Porygon-Z",
        "ROTOM_": "Rotom-",
        "TING_LU": "Ting-Lu",
        "TYPE_NULL": "Type: Null",
        "WO_CHIEN": "Wo-Chien",

        "_ALOLAN": "-Alola",
        "_AQUA_BREED": "-Aqua",
        "_BATTLE_BOND": "-Bond",
        "_BLAZE_BREED": "-Blaze",
        "_CAP": "",
        "_CLOAK": "",
        "_COMBAT_BREED": "-Combat",
        "_CROWED_SHIELD": "-Crowned",
        "_CROWED_SWORD": "-Crowned",
        "_DRIVE": "",
        "_EAST_SEA": "-East",
        "_FAMILY_OF_FOUR": "-Four",
        "_FEMALE": "-F",
        "_FLOWER": "",
        "_GALARIAN": "-Galar",
        "_GIGANTAMAX": "-Gmax",
        "_HISUIAN": "-Hisui",
        "_ICE_RIDER": "-Ice",
        "_NOICE_FACE": "-Noice",
        "_ORIGIN": "-Origin",
        "_ORIGINAL_COLOR": "-Original",
        "_PALDEAN": "-Paldea",
        "_PLUMAGE": "",
        "_POKE_BALL": "-Pokeball",
        "_SHADOW_RIDER": "-Shadow",
        "_STRIKE_STYLE": "-Style",
        "_TOTEM": "-Totem",
        "_ZEN_MODE": "-Zen",
    }

    if not isinstance(species_define, str):
        raise TypeError("Input must be a string")
    if not species_define.startswith("SPECIES_"):
        raise ValueError("Input must start with 'SPECIES_'")
    species_ = species_define[len("SPECIES_"):]
    for match, replacement in species_replacements.items():
        species_ = species_.replace(match, replacement)
    return species_.replace("_", " ").title()
