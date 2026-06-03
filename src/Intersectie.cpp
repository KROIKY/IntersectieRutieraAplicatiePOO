// =============================================================================
//  Intersectie.cpp - implementarea intersectiei.
// =============================================================================
#include "Intersectie.h"
#include "Ecran.h"
#include "Vehicul.h"

Intersectie::Intersectie(const Geometrie& geo, std::unique_ptr<RegulaCirculatie> regula)
    : geo_(geo), regula_(std::move(regula)) {}

void Intersectie::deseneaza(Ecran& ecran) const {
    // Construim textul "REGULA: <nume>   [<stare>]" cu starea colorata ANSI.
    std::string prefix = " REGULA: " + regula_->nume() + "   [";
    std::string stare  = regula_->textStare();
    std::string sufix  = "]";

    const int latimeCamp = 56; // latimea rezervata, ca sa stergem textul vechi
    int vizibil = static_cast<int>(prefix.size() + stare.size() + sufix.size());
    std::string padding(vizibil < latimeCamp ? latimeCamp - vizibil : 0, ' ');

    std::string colorat = "\033[1;" + regula_->codCuloare() + "m" + stare + "\033[0m";

    ecran.scrieLa(0, 1, prefix + colorat + sufix + padding);
}

bool Intersectie::inZonaStop(const Vehicul& v) const {
    int cr = v.centruLinie();
    int cc = v.centruColoana();
    bool peVertical = (cc >= geo_.vColStanga && cc <= geo_.vColDreapta);
    // Zona de stop: cateva randuri imediat sub intersectie.
    return peVertical && cr > geo_.hRowJos && cr <= geo_.hRowJos + 4;
}

bool Intersectie::subIntersectie(const Vehicul& v) const {
    return v.centruLinie() > geo_.hRowJos;
}

bool Intersectie::inBox(const Vehicul& v) const {
    int cr = v.centruLinie();
    int cc = v.centruColoana();
    return cr >= geo_.hRowSus && cr <= geo_.hRowJos &&
           cc >= geo_.vColStanga && cc <= geo_.vColDreapta;
}

bool Intersectie::deasupraIntersectiei(const Vehicul& v) const {
    return v.centruLinie() < geo_.hRowSus;
}
