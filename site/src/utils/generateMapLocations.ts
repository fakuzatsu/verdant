import wildEncounters from '../data/wild_encounters.json'
import type { WildEncountersData } from '../types/encounterTypes'

type MapLocation = [string, string] // [mapId, mapName]

export const getMapName = (mapId: string) => {
  return(mapId.replace('MAP_', '')
    .split('_')
    .map((word: string) => {
      // Handle special cases for numbers and mixed alphanumeric
      if (/^\d+[A-Z]+$/.test(word)) {
        // Cases like "1F", "2R" -> "1F", "2R"
        return word.toUpperCase()
      } else if (/^[A-Z]+\d+[A-Z]*$/.test(word)) {
        // Cases like "B1F", "PUZZLE1", "MAP1" -> "B1F", "Puzzle 1", "Map 1"
        const match = word.match(/^([A-Z]+)(\d+)([A-Z]*)$/)
        if (match) {
          const letters = match[1]
          const numbers = match[2]
          const suffix = match[3]

          // If it's a floor designation like B1F, keep it as is but uppercase
          if (letters.length === 1 && suffix.length > 0) {
            return `${letters}${numbers}${suffix}`
          } else {
            // Regular case like PUZZLE1 -> Puzzle 1
            const formattedLetters =
              letters.charAt(0).toUpperCase() + letters.slice(1).toLowerCase()
            return `${formattedLetters} ${numbers}${suffix}`
          }
        }
      } else if (/^\d+$/.test(word)) {
        // Pure numbers stay as is
        return word
      }
      // Regular words
      return word.charAt(0).toUpperCase() + word.slice(1).toLowerCase()
    })
    .join(' '))
}

function generateMapLocations(): MapLocation[] {
  // Create a Map to store unique map locations
  const mapSet = new Map<string, string>()

  // Type the imported data and get encounters from the new JSON structure
  const wildEncountersData = wildEncounters as WildEncountersData
  const encounters = wildEncountersData.wild_encounter_groups[0].encounters

  // Iterate through all encounters
  encounters.forEach((encounter) => {
    if (encounter.map) {
      // Generate a display name from the map ID
      const displayName = encounter.map
        .replace('MAP_', '')
        .split('_')
        .map((word: string) => {
          // Handle special cases for numbers and mixed alphanumeric
          if (/^\d+[A-Z]+$/.test(word)) {
            // Cases like "1F", "2R" -> "1F", "2R"
            return word.toUpperCase()
          } else if (/^[A-Z]+\d+[A-Z]*$/.test(word)) {
            // Cases like "B1F", "PUZZLE1", "MAP1" -> "B1F", "Puzzle 1", "Map 1"
            const match = word.match(/^([A-Z]+)(\d+)([A-Z]*)$/)
            if (match) {
              const letters = match[1]
              const numbers = match[2]
              const suffix = match[3]

              // If it's a floor designation like B1F, keep it as is but uppercase
              if (letters.length === 1 && suffix.length > 0) {
                return `${letters}${numbers}${suffix}`
              } else {
                // Regular case like PUZZLE1 -> Puzzle 1
                const formattedLetters =
                  letters.charAt(0).toUpperCase() +
                  letters.slice(1).toLowerCase()
                return `${formattedLetters} ${numbers}${suffix}`
              }
            }
          } else if (/^\d+$/.test(word)) {
            // Pure numbers stay as is
            return word
          }
          // Regular words
          return word.charAt(0).toUpperCase() + word.slice(1).toLowerCase()
        })
        .join(' ')

      mapSet.set(encounter.map, displayName)
    }
  })

  // Convert the Map to an array of tuples
  const mapLocations: MapLocation[] = Array.from(mapSet.entries())

  // Sort by map ID for consistency
  mapLocations.sort(([a], [b]) => a.localeCompare(b))

  return mapLocations
}

export const mapLocations = generateMapLocations()
export default mapLocations
