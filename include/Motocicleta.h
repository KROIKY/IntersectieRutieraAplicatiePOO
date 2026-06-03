// =============================================================================
//  Motocicleta.h - vehicul de tip motocicleta (rapida, ingusta).
// =============================================================================
#ifndef MOTOCICLETA_H
#define MOTOCICLETA_H

#include "Vehicul.h"
#include "Sprite.h"

class Motocicleta : public Vehicul {
public:
    explicit Motocicleta(Pozitie poz, Directie dir = Directie::Sus)
        : Vehicul(poz, dir) {}

    int vitezaMaxima() const override { return 4; } // cea mai rapida
    std::string tip() const override { return "Motocicleta"; }
    const std::vector<std::string>& sprite() const override {
        return sprite::motocicleta();
    }
};

#endif // MOTOCICLETA_H
