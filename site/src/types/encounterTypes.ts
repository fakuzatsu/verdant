export interface EncounterData {
  species: string;
  method: string;
  min_level: number;
  max_level: number;
  slot: number;
  encounterRate?: number;
}

export interface GroupedEncounters {
  land_mons: EncounterData[];
  water_mons: EncounterData[];
  rock_smash_mons: EncounterData[];
  fishing_old_rod: EncounterData[];
  fishing_good_rod: EncounterData[];
  fishing_super_rod: EncounterData[];
}

// Types for the wild encounters JSON structure
export interface WildPokemon {
  min_level: number;
  max_level: number;
  species: string;
}

export interface EncounterMethodData {
  encounter_rate: number;
  mons: WildPokemon[];
}

export interface FishingGroups {
  old_rod: number[];
  good_rod: number[];
  super_rod: number[];
}

export interface EncounterField {
  type: 'land_mons' | 'water_mons' | 'rock_smash_mons' | 'fishing_mons';
  encounter_rates: number[];
  groups?: FishingGroups;
}

export interface MapEncounter {
  map: string;
  base_label: string;
  land_mons?: EncounterMethodData;
  water_mons?: EncounterMethodData;
  rock_smash_mons?: EncounterMethodData;
  fishing_mons?: EncounterMethodData;
}

export interface WildEncounterGroup {
  label: string;
  for_maps: boolean;
  fields: EncounterField[];
  encounters: MapEncounter[];
}

export interface WildEncountersData {
  wild_encounter_groups: WildEncounterGroup[];
}

export interface PokemonLocationData {
  locationId: string;
  locationName: string;
  method: string;
  min_level: number;
  max_level: number;
  slot?: number;
  encounterRate?: number;
}

export interface PokemonEncounterData {
  species: string;
  locations: PokemonLocationData[];
}

export interface GroupedPokemonEncounters {
  land_mons: PokemonLocationData[];
  water_mons: PokemonLocationData[];
  rock_smash_mons: PokemonLocationData[];
  fishing_old_rod: PokemonLocationData[];
  fishing_good_rod: PokemonLocationData[];
  fishing_super_rod: PokemonLocationData[];
}
