// =============================================================================
//  Harta.cpp - construirea intersectiei si interogarile geometrice.
// =============================================================================
#include "Harta.h"
#include "Vehicul.h"

Harta::Harta(int linii, int coloane) {
    geo_.linii = linii;
    geo_.coloane = coloane;
    geo_.midC = coloane / 2;
    geo_.midR = linii / 2;

    // Latimea unei benzi se scaleaza usor cu ecranul (pentru o intersectie mare),
    // dar este limitata ca sa ramana proportionala.
    int laneW = coloane / 10;
    if (laneW < 8)  laneW = 8;
    if (laneW > 16) laneW = 16;
    int laneH = linii / 10;
    if (laneH < 3) laneH = 3;
    if (laneH > 8) laneH = 8;

    geo_.vColStanga  = geo_.midC - laneW;
    geo_.vColDreapta = geo_.midC + laneW;
    geo_.hRowSus     = geo_.midR - laneH;
    geo_.hRowJos     = geo_.midR + laneH;

    construieste();
}

void Harta::construieste() {
    const int L = geo_.linii, C = geo_.coloane;
    grila_.assign(L, std::string(C, ' '));

    auto pune = [&](int r, int c, char ch) {
        if (r >= 0 && r < L && c >= 0 && c < C) grila_[r][c] = ch;
    };

    // --- Peretii drumului vertical (cate 2 coloane: '**') ---
    // Prezenti pe toata inaltimea, cu exceptia deschiderii pentru intersectie.
    for (int r = 0; r < L; ++r) {
        bool inBandaOrizontala = (r >= geo_.hRowSus && r <= geo_.hRowJos);
        if (!inBandaOrizontala) {
            pune(r, geo_.vColStanga - 1, '*');
            pune(r, geo_.vColStanga - 2, '*');
            pune(r, geo_.vColDreapta + 1, '*');
            pune(r, geo_.vColDreapta + 2, '*');
        }
    }

    // --- Peretii drumului orizontal (cate o linie: '****...') ---
    for (int c = 0; c < C; ++c) {
        bool inBandaVerticala = (c >= geo_.vColStanga && c <= geo_.vColDreapta);
        if (!inBandaVerticala) {
            pune(geo_.hRowSus - 1, c, '*');
            pune(geo_.hRowJos + 1, c, '*');
        }
    }

    // --- Linia intrerupta de pe mijlocul drumului vertical ('|') ---
    for (int r = 0; r < L; ++r) {
        bool inBandaOrizontala = (r >= geo_.hRowSus && r <= geo_.hRowJos);
        if (!inBandaOrizontala && r % 2 == 0) {
            pune(r, geo_.midC, '|');
        }
    }

    // --- Linia intrerupta de pe mijlocul drumului orizontal ('-') ---
    for (int c = 0; c < C; ++c) {
        bool inBandaVerticala = (c >= geo_.vColStanga && c <= geo_.vColDreapta);
        if (!inBandaVerticala && c % 2 == 0) {
            pune(geo_.midR, c, '-');
        }
    }
}

bool Harta::esteAsfalt(int linie, int coloana) const {
    bool peVertical   = (coloana >= geo_.vColStanga && coloana <= geo_.vColDreapta);
    bool peOrizontal  = (linie   >= geo_.hRowSus    && linie   <= geo_.hRowJos);
    return peVertical || peOrizontal;
}

bool Harta::esteInIntersectie(int linie, int coloana) const {
    bool inV = (coloana >= geo_.vColStanga && coloana <= geo_.vColDreapta);
    bool inH = (linie   >= geo_.hRowSus    && linie   <= geo_.hRowJos);
    return inV && inH;
}

bool Harta::esteContrasens(const Vehicul& v) const {
    int centerC = v.centruColoana();
    int centerR = v.centruLinie();

    bool peVertical    = (centerC >= geo_.vColStanga && centerC <= geo_.vColDreapta);
    bool inIntersectie = (centerR >= geo_.hRowSus    && centerR <= geo_.hRowJos);

    // Evaluam contrasensul doar pe portiunea verticala, in afara intersectiei.
    if (!peVertical || inIntersectie) return false;

    // Circulatie pe dreapta: mergand in sus -> banda corecta este la dreapta
    // liniei centrale (coloana > midC). Mergand in jos -> banda din stanga.
    if (v.directie() == Directie::Sus) return centerC < geo_.midC;
    if (v.directie() == Directie::Jos) return centerC > geo_.midC;
    return false;
}
