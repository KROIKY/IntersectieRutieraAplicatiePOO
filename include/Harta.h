// =============================================================================
//  Harta.h - intersectia statica (in cruce) cu doua benzi pe sens.
// -----------------------------------------------------------------------------
//  Construieste grila de caractere a fundalului si ofera interogari geometrice
//  folosite de logica jocului:
//    - esteAsfalt(r,c)        : pozitia este pe carosabil?
//    - esteInIntersectie(r,c) : pozitia este in patratul central?
//    - esteContrasens(vehicul): vehiculul circula pe banda opusa sensului sau?
//
//  Drumul vertical are doua benzi separate de linia intrerupta '|' (coloana
//  centrala). Conform circulatiei pe dreapta, un vehicul care merge in SUS
//  trebuie sa fie pe banda din DREAPTA (coloane > centru).
// =============================================================================
#ifndef HARTA_H
#define HARTA_H

#include "Comun.h"
#include <string>
#include <vector>

class Vehicul;

// Parametrii geometrici ai intersectiei (calculati din dimensiunea ecranului).
struct Geometrie {
    int linii = 0, coloane = 0;
    int midR = 0, midC = 0;        // centrul intersectiei
    int vColStanga = 0, vColDreapta = 0; // marginile asfaltului drumului vertical
    int hRowSus = 0, hRowJos = 0;        // marginile asfaltului drumului orizontal
};

class Harta {
public:
    Harta(int linii, int coloane);

    const std::vector<std::string>& grila() const { return grila_; }
    const Geometrie& geometrie() const { return geo_; }

    bool esteAsfalt(int linie, int coloana) const;
    bool esteInIntersectie(int linie, int coloana) const;

    // Adevarat daca vehiculul circula pe contrasens (banda opusa sensului sau),
    // pe portiunea verticala de drum, in afara intersectiei.
    bool esteContrasens(const Vehicul& v) const;

private:
    void construieste();

    Geometrie geo_;
    std::vector<std::string> grila_;
};

#endif // HARTA_H
