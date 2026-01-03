import wildEncounters from "../../data/wild_encounters.json";
import { mapLocations } from "../generateMapLocations";
import type {
  PokemonLocationData,
  GroupedPokemonEncounters,
  WildEncountersData,
  MapEncounter,
  EncounterField,
} from "../../types/encounterTypes";

import speciesLabelMap from "../../data/speciesLabelMap.json";

// Type the imported data
const wildEncountersData = wildEncounters as WildEncountersData;
const encounterList: MapEncounter[] =
  wildEncountersData.wild_encounter_groups[0].encounters;
const wildMonHeaders: EncounterField[] =
  wildEncountersData.wild_encounter_groups[0].fields;

export const deduplicatePokemonLocations = (
  locations: PokemonLocationData[]
): PokemonLocationData[] => {
  const grouped = new Map<string, PokemonLocationData[]>();

  // Group by locationId + method combination
  locations.forEach((location) => {
    const key = `${location.locationId}_${location.method}`;
    const existing = grouped.get(key) || [];
    existing.push(location);
    grouped.set(key, existing);
  });

  // Consolidate each location group
  return Array.from(grouped.entries()).map(([, locationGroup]) => {
    const consolidated: PokemonLocationData = {
      locationId: locationGroup[0].locationId,
      locationName: locationGroup[0].locationName,
      method: locationGroup[0].method,
      min_level: Math.min(...locationGroup.map((l) => l.min_level)),
      max_level: Math.max(...locationGroup.map((l) => l.max_level)),
      slot: locationGroup[0].slot, // Keep first slot for reference
      encounterRate: locationGroup.reduce(
        (sum, l) => sum + (l.encounterRate || 0),
        0
      ),
    };

    return consolidated;
  });
};

export const processPokemonEncounters = (species: number | null) => {
  if (!species) return { locations: [] };

  const resultMap = new Map<string, PokemonLocationData>();

  // Iterate through all map encounters
  encounterList.forEach((encounter: MapEncounter) => {
    (
      ["land_mons", "water_mons", "rock_smash_mons", "fishing_mons"] as const
    ).forEach((method) => {
      const methodData = encounter[method];
      if (methodData?.mons) {
        // Find the encounter rates for this method type
        const methodHeader = wildMonHeaders?.find(
          (h: EncounterField) => h.type === method
        );
        const encounterRates = methodHeader?.encounter_rates || [];

        methodData.mons.forEach((mon, slot: number) => {
          const monSpeciesId = getSpeciesIdFromLabel(mon.species);
          if (monSpeciesId === species) {
            // Get the location display name
            const location = mapLocations.find(
              ([mapId]) => mapId === encounter.map
            );
            const locationName = location ? location[1] : encounter.map;

            // Create a unique key for this location/method/slot combination
            const key = `${encounter.map}_${method}_${slot}`;
            
            const locationData: PokemonLocationData = {
              locationId: encounter.map,
              locationName,
              method,
              min_level: mon.min_level || 1,
              max_level: mon.max_level || mon.min_level || 1,
              slot,
              encounterRate: encounterRates[slot],
            };

            // If we already have this location/method/slot, merge the data
            const existing = resultMap.get(key);
            if (existing) {
              // Merge encounter rates and level ranges
              existing.min_level = Math.min(existing.min_level, locationData.min_level);
              existing.max_level = Math.max(existing.max_level, locationData.max_level);
              existing.encounterRate = (existing.encounterRate || 0) + (locationData.encounterRate || 0);
            } else {
              resultMap.set(key, locationData);
            }
          }
        });
      }
    });
  });

  return { locations: Array.from(resultMap.values()) };
};

export const getSpeciesIdFromLabel = (label: string): number => {
  return Number(speciesLabelMap[label as keyof typeof speciesLabelMap]);
};

export const groupPokemonEncountersByMethod = (
  locations: PokemonLocationData[]
): GroupedPokemonEncounters => {
  const grouped: GroupedPokemonEncounters = {
    land_mons: [],
    water_mons: [],
    rock_smash_mons: [],
    fishing_old_rod: [],
    fishing_good_rod: [],
    fishing_super_rod: [],
  };

  locations.forEach((location) => {
    if (location.method === "land_mons") {
      grouped.land_mons.push(location);
    } else if (location.method === "water_mons") {
      grouped.water_mons.push(location);
    } else if (location.method === "rock_smash_mons") {
      grouped.rock_smash_mons.push(location);
    } else if (location.method === "fishing_mons") {
      // Determine rod type based on slot number (matching encounterGrouper logic)
      const slot = location.slot || 0;
      if (slot >= 0 && slot <= 2) {
        grouped.fishing_old_rod.push(location);
      } else if (slot >= 3 && slot <= 5) {
        grouped.fishing_good_rod.push(location);
      } else if (slot >= 6 && slot <= 10) {
        grouped.fishing_super_rod.push(location);
      }
    }
  });

  // Deduplicate each method group
  return {
    land_mons: deduplicatePokemonLocations(grouped.land_mons),
    water_mons: deduplicatePokemonLocations(grouped.water_mons),
    rock_smash_mons: deduplicatePokemonLocations(grouped.rock_smash_mons),
    fishing_old_rod: deduplicatePokemonLocations(grouped.fishing_old_rod),
    fishing_good_rod: deduplicatePokemonLocations(grouped.fishing_good_rod),
    fishing_super_rod: deduplicatePokemonLocations(grouped.fishing_super_rod),
  };
};

export const getPokemonDisplayName = (species: string | null) => {
  if (!species) return "";

  // Convert species name to display format
  // e.g., "SPECIES_PIKACHU" -> "Pikachu"
  const displayName = species
    .replace(/^SPECIES_/, "")
    .toLowerCase()
    .replace(/^\w/, (c) => c.toUpperCase());

  return displayName;
};
