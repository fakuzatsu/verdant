import statics from '../../data/statics.json';

export interface SingleEncounter {
  species: string;
  level: number;
  map: string;
  explanation: string;
}

export interface ChoiceGroup {
  map: string;
  explanation: string;
  choices: {
    species: string;
    level: number;
    isRandom?: boolean;
  }[];
}

export interface EggEncounter {
  species: string;
  map: string;
  explanation: string;
}

/**
 * Gets all single static encounters (including gifts)
 */
export function getSingleEncounters(): SingleEncounter[] {
  const singles: SingleEncounter[] = [];

  statics.forEach((entry) => {
    // Handle single species encounters (string)
    if (entry.species && typeof entry.species === 'string') {
      singles.push({
        species: entry.species,
        level: entry.level,
        map: entry.map,
        explanation: entry.explanation
      });
    }
    // Handle single species encounters (array with only one item)
    else if (entry.species && Array.isArray(entry.species) && entry.species.length === 1) {
      singles.push({
        species: entry.species[0].species,
        level: entry.species[0].level,
        map: entry.map,
        explanation: entry.explanation
      });
    }
  });

  return singles;
}

/**
 * Gets all choice encounters grouped by choice set
 */
export function getChoiceEncounters(): ChoiceGroup[] {
  const choiceGroups: ChoiceGroup[] = [];

  statics.forEach((entry) => {
    // Handle choice-based encounters (species array with multiple items)
    if (entry.species && Array.isArray(entry.species) && entry.species.length > 1) {
      const choices = entry.species.map(pokemon => ({
        species: pokemon.species,
        level: pokemon.level,
        isRandom: 'isRandom' in pokemon ? pokemon.isRandom : undefined
      }));

      choiceGroups.push({
        map: entry.map,
        explanation: entry.explanation,
        choices
      });
    }
  });

  return choiceGroups;
}

/**
 * Gets all egg encounters
 */
export function getEggEncounters(): EggEncounter[] {
  const eggs: EggEncounter[] = [];

  statics.forEach((entry) => {
    // Handle egg encounters
    if (entry.eggs && Array.isArray(entry.eggs)) {
      entry.eggs.forEach((egg) => {
        eggs.push({
          species: egg.species,
          map: entry.map,
          explanation: entry.explanation
        });
      });
    }
  });

  return eggs;
}

