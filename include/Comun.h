// =============================================================================
//  Comun.h - tipuri si structuri folosite in tot proiectul.
// =============================================================================
#ifndef COMUN_H
#define COMUN_H

// Pozitie pe ecran / pe harta, exprimata in (linie, coloana).
// Coordonatele sunt 0-based intern; conversia la ANSI (1-based) se face in Ecran.
struct Pozitie {
    int linie = 0;
    int coloana = 0;

    Pozitie() = default;
    Pozitie(int l, int c) : linie(l), coloana(c) {}

    bool operator==(const Pozitie& alta) const {
        return linie == alta.linie && coloana == alta.coloana;
    }
    bool operator!=(const Pozitie& alta) const {
        return !(*this == alta);
    }
};

// Directiile de deplasare a vehiculelor pe harta.
enum class Directie {
    Sus,
    Jos,
    Stanga,
    Dreapta
};

#endif // COMUN_H
