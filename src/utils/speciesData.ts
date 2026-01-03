import speciesData from "../data/speciesData.json";
import { type Pokemon } from "../types";

export function getSpeciesName(speciesId: number): string {
  const species = speciesData.find((species) => species.speciesId === speciesId);
  if (!species) return "None";
  return species.speciesName || "None";
}

// export function getNameKey(speciesId: number): string {
//   const species = speciesData.find((species) => species.speciesId === speciesId);
//   if (!species) return "None";
//   return species.nameKey || "None";
// }

export function getSpeciesData(speciesId: number): Pokemon {
  const species = speciesData.find((species) => species.speciesId === speciesId);
  if (!species) throw new Error(`Species ${speciesId} not found`);
  return species;
}

export function hasForms(pokemon: Pokemon): boolean {
  const forms = speciesData.filter(
    (species) => species.dexId === pokemon.dexId
  );
  return forms.length > 1;
}

export const getSpriteById = (speciesId: number): string => {
  return `sprites/front/${speciesId}.png`;
};
