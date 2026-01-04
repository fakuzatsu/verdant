# dex_scripts/utils/species_post_processor.py

class SpeciesPostProcessor:
    def __init__(self, data):
        self.data = data

    def get_species_name(self, species_define):
        """
        Converts the species define to a Showdown-compatible name and adds it as 'nameKey' in each entry.
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
        return species_.replace("_", "-").title()
    

    def process_name_keys(self):
        # Hardcoded path to the generated speciesLabelMap.json
        import json
        from pathlib import Path
        label_map_path = Path(__file__).parent.parent.parent / "dex_files/json/speciesLabelMap.json"
        with open(label_map_path, "r", encoding="utf-8") as f:
            label_map = json.load(f)
        for entry in self.data:
            species_id = str(entry.get("speciesId"))
            species_define = label_map.get(species_id)
            if species_define:
                try:
                    entry["nameKey"] = self.get_species_name(species_define)
                except Exception:
                    entry["nameKey"] = None
            else:
                entry["nameKey"] = None
        return self.data

    def add_siblings(self):
        # Build a mapping from dexId to all speciesIds with that dexId
        dexid_to_species = {}
        for entry in self.data:
            dex_id = entry.get("dexId")
            species_id = entry.get("speciesId")
            if dex_id is not None and species_id is not None:
                dexid_to_species.setdefault(dex_id, []).append(species_id)

        # For each entry, add a 'siblings' field with all other speciesIds sharing the same dexId
        for entry in self.data:
            dex_id = entry.get("dexId")
            species_id = entry.get("speciesId")
            if dex_id is not None and species_id is not None:
                siblings = [sid for sid in dexid_to_species[dex_id] if sid != species_id]
                entry["siblings"] = siblings
        return self.data

    def process(self):
        # Call your processing methods in order
        self.process_name_keys()
        self.add_siblings()
        return self.data
