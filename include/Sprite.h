// =============================================================================
//  Sprite.h - desenele ASCII (sprite-urile) ale vehiculelor.
// -----------------------------------------------------------------------------
//  Fiecare sprite este un vector de linii. Caracterul spatiu ' ' este tratat
//  ca transparent la desenare (lasa fundalul sa se vada).
//
//  Orientarea de baza este verticala (vehiculul priveste in sus/jos). Variantele
//  orizontale (pentru viraje) se adauga in etapele urmatoare.
// =============================================================================
#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <vector>

namespace sprite {

// Masina jucatorului:  e@@e / @@ / e@@e
const std::vector<std::string>& masinaJucator();

// Masina NPC:          0XX0 / XX / 0XX0
const std::vector<std::string>& masina();

// Camion NPC (lung):   0XX0 / XX / XX / XX / 0XX0
const std::vector<std::string>& camion();

// Motocicleta NPC:     o / @ / o
const std::vector<std::string>& motocicleta();

// --- Variante orizontale (pentru vehiculele care circula stanga/dreapta) ---
const std::vector<std::string>& masinaOrizontala();
const std::vector<std::string>& camionOrizontal();
const std::vector<std::string>& motocicletaOrizontala();

} // namespace sprite

#endif // SPRITE_H
