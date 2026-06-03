// =============================================================================
//  Joc.cpp - implementarea buclei principale si a regulilor de baza.
// =============================================================================
#include "Joc.h"

#include <cmath>
#include <string>
#include <unistd.h> // usleep

namespace {
    const double SECUNDE_CONTRASENS = 3.0; // limita pe contrasens
}

bool Joc::esteOffRoad(const Pozitie& poz) const {
    const auto& s = jucator_->sprite();
    for (int r = 0; r < static_cast<int>(s.size()); ++r) {
        for (int c = 0; c < static_cast<int>(s[r].size()); ++c) {
            if (s[r][c] == ' ') continue;
            if (!harta_->esteAsfalt(poz.linie + r, poz.coloana + c)) {
                return true; // macar o celula a vehiculului este in afara drumului
            }
        }
    }
    return false;
}

void Joc::deseneazaStatus(const std::string& avertisment) {
    const int L = ecran_.linii();
    const int C = ecran_.coloane();

    std::string status = " Scor: " + std::to_string(scor_) +
                         "   Vieti: " + std::to_string(vieti_) +
                         "   [WASD/sageti] misca, P pauza, Q iesire";
    if (!avertisment.empty()) {
        status += "   " + avertisment;
    }

    // Completam / taiem la latimea ecranului ca sa stergem complet randul vechi.
    if (static_cast<int>(status.size()) > C) status.resize(C);
    else status.resize(C, ' ');

    ecran_.scrieLa(L - 1, 0, status);
}

void Joc::ruleaza() {
    ecran_.initializeaza();

    const int L = ecran_.linii();
    const int C = ecran_.coloane();

    // 1) Harta statica (intersectia).
    harta_ = std::make_unique<Harta>(L, C);
    ecran_.seteazaFundal(harta_->grila());
    ecran_.deseneazaFundal();

    // 2) Masina jucatorului: porneste pe banda din dreapta, jos, cu fata in sus.
    const Geometrie& g = harta_->geometrie();
    Pozitie start(L - 4, g.midC + 3);
    jucator_ = std::make_unique<MasinaJucator>(start, Directie::Sus);
    jucator_->deseneaza(ecran_);
    deseneazaStatus("");
    ecran_.actualizeaza();

    eraOffRoad_ = esteOffRoad(start);

    // Limitele de deplasare (sprite-ul trebuie sa incapa, randul de status liber).
    const int minLinie = 0;
    const int maxLinie = L - 1 - jucator_->inaltime();
    const int minCol   = 0;
    const int maxCol   = C - jucator_->latime();

    bool ruleaza = true;
    bool pauza   = false;
    bool gameOver = false;

    // 3) Bucla principala.
    while (ruleaza) {
        Tasta t = input_.citeste();

        if (t == Tasta::Iesire) {
            break;
        } else if (t == Tasta::Pauza) {
            pauza = !pauza;
        } else if (!pauza && t != Tasta::Niciuna) {
            Pozitie nou   = jucator_->pozitie();
            Directie dir  = jucator_->directie();
            switch (t) {
                case Tasta::Sus:     nou.linie--;   dir = Directie::Sus; break;
                case Tasta::Jos:     nou.linie++;   dir = Directie::Jos; break;
                case Tasta::Stanga:  nou.coloana--;                      break;
                case Tasta::Dreapta: nou.coloana++;                      break;
                default: break;
            }

            if (nou.linie < minLinie) nou.linie = minLinie;
            if (nou.linie > maxLinie) nou.linie = maxLinie;
            if (nou.coloana < minCol) nou.coloana = minCol;
            if (nou.coloana > maxCol) nou.coloana = maxCol;

            if (nou != jucator_->pozitie() || dir != jucator_->directie()) {
                jucator_->sterge(ecran_);
                jucator_->setPozitie(nou);
                jucator_->setDirectie(dir);
                jucator_->deseneaza(ecran_);
            }
        }

        // --- Penalizare la iesirea de pe carosabil (o data per iesire) ---
        bool offAcum = esteOffRoad(jucator_->pozitie());
        if (offAcum && !eraOffRoad_) {
            vieti_--;
        }
        eraOffRoad_ = offAcum;

        if (vieti_ <= 0) {
            afiseazaGameOver("Ai ramas fara vieti (prea multe iesiri de pe carosabil).");
            gameOver = true;
            break;
        }

        // --- Regula contrasensului (avertisment + 3 secunde) ---
        std::string avert;
        bool contra = harta_->esteContrasens(*jucator_);
        if (contra) {
            auto acum = std::chrono::steady_clock::now();
            if (!peContrasens_) {
                peContrasens_ = true;
                startContrasens_ = acum;
            }
            double trecut = std::chrono::duration<double>(acum - startContrasens_).count();
            double ramas  = SECUNDE_CONTRASENS - trecut;
            if (ramas <= 0.0) {
                afiseazaGameOver("Ai stat prea mult pe contrasens!");
                gameOver = true;
                break;
            }
            avert = "!! CONTRASENS !! Revino pe banda ta in " +
                    std::to_string(static_cast<int>(std::ceil(ramas))) + "s";
        } else {
            peContrasens_ = false;
            if (offAcum) {
                avert = "Atentie: in afara carosabilului!";
            }
        }

        deseneazaStatus(avert);
        ecran_.actualizeaza();

        usleep(16000); // ~60 cadre/secunda
    }

    if (gameOver) {
        asteaptaIesire();
    }

    ecran_.restaureaza();
}

void Joc::afiseazaGameOver(const std::string& motiv) {
    const int L = ecran_.linii();
    const int C = ecran_.coloane();
    const int r = L / 2;

    auto centrat = [&](int linie, const std::string& text) {
        int col = (C - static_cast<int>(text.size())) / 2;
        if (col < 0) col = 0;
        ecran_.scrieLa(linie, col, text);
    };

    centrat(r - 2, "==================================");
    centrat(r - 1, "             GAME OVER            ");
    centrat(r,     motiv);
    centrat(r + 1, "Scor final: " + std::to_string(scor_));
    centrat(r + 2, "Apasa Q sau Esc pentru iesire.");
    centrat(r + 3, "==================================");
    ecran_.actualizeaza();
}

void Joc::asteaptaIesire() {
    while (true) {
        Tasta t = input_.citeste();
        if (t == Tasta::Iesire) break;
        usleep(20000);
    }
}
