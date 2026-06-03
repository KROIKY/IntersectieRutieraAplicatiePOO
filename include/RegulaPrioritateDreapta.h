// =============================================================================
//  RegulaPrioritateDreapta.h - regula prioritatii de dreapta.
// -----------------------------------------------------------------------------
//  Vehiculul trebuie sa cedeze vehiculelor care vin din dreapta. In lipsa
//  traficului NPC (adaugat in Etapa 4) intrarea este permisa. Logica de cedare
//  efectiva se completeaza cand exista vehicule pe drumul transversal.
// =============================================================================
#ifndef REGULA_PRIORITATE_DREAPTA_H
#define REGULA_PRIORITATE_DREAPTA_H

#include "RegulaCirculatie.h"

class RegulaPrioritateDreapta : public RegulaCirculatie {
public:
    std::string nume() const override { return "PRIORITATE DREAPTA"; }

    bool permiteIntrarea(const ContextRegula& /*ctx*/) const override {
        return liber_; // implicit liber daca nu vine nimeni din dreapta
    }

    // Apelata din Etapa 4: marcheaza daca vine un vehicul din dreapta.
    void seteazaLiber(bool liber) { liber_ = liber; }

    std::string textStare() const override { return liber_ ? "LIBER" : "CEDEAZA"; }
    std::string codCuloare() const override { return liber_ ? "32" : "33"; }
    std::string motivIncalcare() const override {
        return "Nu ai cedat prioritatea de dreapta!";
    }

private:
    bool liber_ = true;
};

#endif // REGULA_PRIORITATE_DREAPTA_H
