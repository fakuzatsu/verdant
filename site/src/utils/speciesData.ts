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
  const spriteMap = import.meta.glob('../assets/sprites/front/*.png', { query: '?url', import: 'default', eager: true }) as Record<string, string>
  const key = `../assets/sprites/front/${speciesId}.png`
  return spriteMap[key] ?? new URL(`../assets/sprites/front/${speciesId}.png`, import.meta.url).href
}
