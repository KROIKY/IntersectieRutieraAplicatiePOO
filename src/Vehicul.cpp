// =============================================================================
//  Vehicul.cpp - implementarea functionalitatii comune tuturor vehiculelor.
// =============================================================================
#include "Vehicul.h"
#include "Ecran.h"

int Vehicul::inaltime() const {
    return static_cast<int>(sprite().size());
}

int Vehicul::latime() const {
    int maxim = 0;
    for (const auto& linie : sprite()) {
        if (static_cast<int>(linie.size()) > maxim) {
            maxim = static_cast<int>(linie.size());
        }
    }
    return maxim;
}

bool Vehicul::ocupaCelula(int linie, int coloana) const {
    const auto& s = sprite();
    int r = linie - poz_.linie;
    int c = coloana - poz_.coloana;
    if (r < 0 || r >= static_cast<int>(s.size())) return false;
    if (c < 0 || c >= static_cast<int>(s[r].size())) return false;
    return s[r][c] != ' ';
}

void Vehicul::deseneaza(Ecran& ecran) const {
    const auto& s = sprite();
    for (int r = 0; r < static_cast<int>(s.size()); ++r) {
        for (int c = 0; c < static_cast<int>(s[r].size()); ++c) {
            char ch = s[r][c];
            if (ch == ' ') continue; // transparent
            ecran.scrieLa(poz_.linie + r, poz_.coloana + c, ch);
        }
    }
}

void Vehicul::sterge(Ecran& ecran) const {
    const auto& s = sprite();
    for (int r = 0; r < static_cast<int>(s.size()); ++r) {
        for (int c = 0; c < static_cast<int>(s[r].size()); ++c) {
            if (s[r][c] == ' ') continue;
            // Restaureaza caracterul de fundal de sub acest caracter.
            ecran.stergeLa(poz_.linie + r, poz_.coloana + c);
        }
    }
}
