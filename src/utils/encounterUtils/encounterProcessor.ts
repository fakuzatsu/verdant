import wildEncounters from "../../data/wild_encounters.json";
import { mapLocations } from "../generateMapLocations";
import { getSpeciesIdFromLabel } from "./pokemonEncounterProcessor";
import type {
  EncounterData,
  WildEncountersData,
  MapEncounter,
  EncounterField,
} from "../../types/encounterTypes";

// Type the imported data
const wildEncountersData = wildEncounters as WildEncountersData;
const encounterList: MapEncounter[] =
  wildEncountersData.wild_encounter_groups[0].encounters;
const wildMonHeaders: EncounterField[] =
  wildEncountersData.wild_encounter_groups[0].fields;

export const processEncounters = (selectedRegionId: string | null) => {
  if (!selectedRegionId) return { encounters: [] };

  const encounter = encounterList.find(
    (e: MapEncounter) => e.map === selectedRegionId
  );
  if (!encounter) return { encounters: [] };

  const resultMap = new Map<string, EncounterData>();

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
        if (mon.species) {
          const speciesId = getSpeciesIdFromLabel(mon.species);
          // Create a unique key based on species ID and method (not the raw species label)
          const key = `${speciesId}_${method}_${slot}`;

          const encounterData: EncounterData = {
            species: mon.species, // Keep the original species label for display
            method,
            min_level: mon.min_level || 1,
            max_level: mon.max_level || mon.min_level || 1,
            slot,
            encounterRate: encounterRates[slot],
          };

          // If we already have this species ID/method/slot, merge the data
          const existing = resultMap.get(key);
          if (existing) {
            // Merge encounter rates and level ranges
            existing.min_level = Math.min(
              existing.min_level,
              encounterData.min_level
            );
            existing.max_level = Math.max(
              existing.max_level,
              encounterData.max_level
            );
            existing.encounterRate =
              (existing.encounterRate || 0) +
              (encounterData.encounterRate || 0);
          } else {
            resultMap.set(key, encounterData);
          }
        }
      });
    }
  });

  return { encounters: Array.from(resultMap.values()) };
};

export const getDisplayName = (selectedRegionId: string | null) => {
  if (!selectedRegionId) return "";
  const location = mapLocations.find(([mapId]) => mapId === selectedRegionId);
  return location ? location[1] : selectedRegionId;
};
