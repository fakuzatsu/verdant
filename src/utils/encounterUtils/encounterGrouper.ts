import type { EncounterData, GroupedEncounters } from "../../types/encounterTypes";
import { deduplicateEncounters } from "./encounterDeduplicator";

export const groupEncountersByMethod = (encounters: EncounterData[]): GroupedEncounters => {
  return {
    land_mons: deduplicateEncounters(encounters.filter(e => e.method === 'land_mons')),
    water_mons: deduplicateEncounters(encounters.filter(e => e.method === 'water_mons')),
    rock_smash_mons: deduplicateEncounters(encounters.filter(e => e.method === 'rock_smash_mons')),
    fishing_old_rod: deduplicateEncounters(encounters.filter(e => e.method === 'fishing_mons' && e.slot >= 0 && e.slot <= 2)),
    fishing_good_rod: deduplicateEncounters(encounters.filter(e => e.method === 'fishing_mons' && e.slot >= 3 && e.slot <= 5)),
    fishing_super_rod: deduplicateEncounters(encounters.filter(e => e.method === 'fishing_mons' && e.slot >= 6 && e.slot <= 10)),
  };
};
