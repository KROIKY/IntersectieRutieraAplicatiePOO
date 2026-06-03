// =============================================================================
//  Camion.h - vehicul de tip camion (lung, lent).
// =============================================================================
#ifndef CAMION_H
#define CAMION_H

#include "Vehicul.h"
#include "Sprite.h"

class Camion : public Vehicul {
public:
    explicit Camion(Pozitie poz, Directie dir = Directie::Sus)
        : Vehicul(poz, dir) {}

    int vitezaMaxima() const override { return 2; } // cel mai lent
    std::string tip() const override { return "Camion"; }
    const std::vector<std::string>& sprite() const override {
        bool orizontal = (dir_ == Directie::Stanga || dir_ == Directie::Dreapta);
        return orizontal ? sprite::camionOrizontal() : sprite::camion();
    }
};

#endif // CAMION_H
