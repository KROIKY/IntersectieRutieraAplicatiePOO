// =============================================================================
//  Intersectie.h - intersectia, cu regula ei de circulatie (compozitie).
// -----------------------------------------------------------------------------
//  Concept POO: compozitie - Intersectie DETINE un obiect RegulaCirculatie
//  (prin unique_ptr) si delega catre el deciziile de circulatie.
// =============================================================================
#ifndef INTERSECTIE_H
#define INTERSECTIE_H

#include "Harta.h"            // Geometrie
#include "RegulaCirculatie.h"

#include <memory>
#include <string>

class Ecran;
class Vehicul;

class Intersectie {
public:
    Intersectie(const Geometrie& geo, std::unique_ptr<RegulaCirculatie> regula);

    void actualizeaza(double dt) { regula_->actualizeaza(dt); }

    RegulaCirculatie&       regula()       { return *regula_; }
    const RegulaCirculatie& regula() const { return *regula_; }
    void seteazaRegula(std::unique_ptr<RegulaCirculatie> r) { regula_ = std::move(r); }

    // Deseneaza, pe randul de sus, regula curenta si starea ei (cu culoare).
    void deseneaza(Ecran& ecran) const;

    // Pozitia vehiculului fata de intersectie (folosind centrul vehiculului).
    bool inZonaStop(const Vehicul& v) const; // imediat sub intersectie
    bool subIntersectie(const Vehicul& v) const;
    bool inBox(const Vehicul& v) const;
    bool deasupraIntersectiei(const Vehicul& v) const;

private:
    Geometrie geo_;
    std::unique_ptr<RegulaCirculatie> regula_;
};

#endif // INTERSECTIE_H
