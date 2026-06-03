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
        return sprite::masina();
    }
};

#endif // MASINA_H
