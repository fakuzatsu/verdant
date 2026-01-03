import type { EncounterData } from "../../types/encounterTypes";
import { getSpeciesIdFromLabel } from "./pokemonEncounterProcessor";

export const deduplicateEncounters = (encounters: EncounterData[]): EncounterData[] => {
  const grouped = new Map<number, EncounterData[]>();
  
  // Group by species ID (not the raw label)
  encounters.forEach(encounter => {
    const speciesId = getSpeciesIdFromLabel(encounter.species);
    const existing = grouped.get(speciesId) || [];
    existing.push(encounter);
    grouped.set(speciesId, existing);
  });
  
  // Consolidate each species group
  return Array.from(grouped.entries()).map(([, speciesEncounters]) => {
    const consolidated: EncounterData = {
      species: speciesEncounters[0].species, // Keep the first species label for display
      method: speciesEncounters[0].method, // All should have same method within a group
      min_level: Math.min(...speciesEncounters.map(e => e.min_level)),
      max_level: Math.max(...speciesEncounters.map(e => e.max_level)),
      slot: speciesEncounters[0].slot, // Keep first slot for reference
      encounterRate: speciesEncounters.reduce((sum, e) => sum + (e.encounterRate || 0), 0)
    };
    
    return consolidated;
  });
};
