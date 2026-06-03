// =============================================================================
//  Masina.h - vehicul de tip masina (NPC standard).
// =============================================================================
#ifndef MASINA_H
#define MASINA_H

#include "Vehicul.h"
#include "Sprite.h"

class Masina : public Vehicul {
public:
    explicit Masina(Pozitie poz, Directie dir = Directie::Sus)
        : Vehicul(poz, dir) {}

    int vitezaMaxima() const override { return 3; }
    std::string tip() const override { return "Masina"; }
    const std::vector<std::string>& sprite() const override {
        bool orizontal = (dir_ == Directie::Stanga || dir_ == Directie::Dreapta);
        return orizontal ? sprite::masinaOrizontala() : sprite::masina();
    }
};

#endif // MASINA_H
