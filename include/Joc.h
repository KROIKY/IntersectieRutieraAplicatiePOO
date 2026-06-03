// =============================================================================
//  Joc.h - logica principala a jocului (bucla, stare, reguli de baza).
// -----------------------------------------------------------------------------
//  Responsabilitati in Etapa 2:
//    - construieste harta (intersectia) si deseneaza fundalul o singura data;
//    - controleaza masina jucatorului (WASD / sageti), cu refresh selectiv;
//    - penalizeaza iesirea de pe carosabil (pierdere de viata);
//    - detecteaza contrasensul si aplica regula celor 3 secunde (game over);
//    - afiseaza bara de stare (scor, vieti, avertismente) si ecranul de final.
//
//  Coliziunile cu vehicule NPC si virajele la intersectie se adauga in etapele
//  3 si 4.
// =============================================================================
#ifndef JOC_H
#define JOC_H

#include "Ecran.h"
#include "Input.h"
#include "Harta.h"
#include "MasinaJucator.h"

#include <chrono>
#include <memory>
#include <string>

class Joc {
public:
    Joc() = default;
    void ruleaza();

private:
    void deseneazaStatus(const std::string& avertisment);
    bool esteOffRoad(const Pozitie& poz) const; // vehiculul iese de pe asfalt?
    void afiseazaGameOver(const std::string& motiv);
    void asteaptaIesire(); // bucla de asteptare pana la Q/Esc

    Ecran ecran_;
    Input input_;
    std::unique_ptr<Harta> harta_;
    std::unique_ptr<MasinaJucator> jucator_;

    int scor_  = 0;
    int vieti_ = 3;

    bool peContrasens_ = false;
    std::chrono::steady_clock::time_point startContrasens_;

    bool eraOffRoad_ = false;
};

#endif // JOC_H
