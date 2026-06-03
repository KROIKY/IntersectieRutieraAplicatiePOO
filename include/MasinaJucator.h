// =============================================================================
//  MasinaJucator.h - masina controlata de jucator.
// -----------------------------------------------------------------------------
//  Deriva din Masina (un nivel suplimentar de mostenire) si suprascrie doar
//  sprite-ul si tipul. Demonstreaza specializarea unei clase deja derivate.
// =============================================================================
#ifndef MASINA_JUCATOR_H
#define MASINA_JUCATOR_H

#include "Masina.h"
#include "Sprite.h"

class MasinaJucator : public Masina {
public:
    explicit MasinaJucator(Pozitie poz, Directie dir = Directie::Sus)
        : Masina(poz, dir) {}

    std::string tip() const override { return "Masina (jucator)"; }
    const std::vector<std::string>& sprite() const override {
        bool orizontal = (dir_ == Directie::Stanga || dir_ == Directie::Dreapta);
        return orizontal ? sprite::masinaJucatorOrizontala() : sprite::masinaJucator();
    }
};

#endif // MASINA_JUCATOR_H
