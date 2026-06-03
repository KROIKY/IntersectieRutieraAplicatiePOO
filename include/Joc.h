// =============================================================================
//  Joc.h - logica principala a jocului (bucla, stare, reguli).
// -----------------------------------------------------------------------------
//  Etapa 2: control masina, refresh selectiv, contrasens, vieti.
//  Etapa 3: intersectie cu reguli polimorfe (STOP / semafor / prioritate),
//           scor pentru trecere corecta, game over la incalcare, ciclarea
//           regulilor la fiecare trecere reusita.
// =============================================================================
#ifndef JOC_H
#define JOC_H

#include "Ecran.h"
#include "Input.h"
#include "Harta.h"
#include "Intersectie.h"
#include "MasinaJucator.h"
#include "RegulaCirculatie.h"
#include "Vehicul.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

class Joc {
public:
    Joc() = default;
    void ruleaza();

private:
    // Meniuri (stil old-school cu butoane dreptunghiulare).
    bool meniuPrincipal();        // true = PLAY, false = EXIT
    int  alegeDificultate();      // 5 / 10 / 20 intersectii, sau -1 = inapoi
    void ruleazaJoc(int tintaIntersectii); // o partida efectiva

    void deseneazaStatus(const std::string& avertisment);
    bool esteOffRoad(const Pozitie& poz) const;
    void afiseazaGameOver(const std::string& motiv);
    void afiseazaVictorie();
    void asteaptaIesire();

    void reseteazaJucator();                 // readuce masina la start (respawn)
    void schimbaRegula();                    // trece la urmatoarea regula
    std::unique_ptr<RegulaCirculatie> creeazaRegula(int index) const;

    // --- Trafic NPC (Etapa 4) ---
    struct Npc {
        std::unique_ptr<Vehicul> v;
        int interval;     // cate cadre intre doua mutari
        int contor = 0;
    };
    void spawnNpc();                         // genereaza un vehicul NPC
    void actualizeazaNpc();                  // muta NPC-urile, sterge cele iesite
    bool coliziuneCuNpc() const;             // jucatorul a lovit un NPC?

    // Decide daca un vehicul NPC trebuie sa se opreasca la limita intersectiei
    // (rosu la semafor, sau cedeaza jucatorului la prioritate de dreapta).
    bool npcTrebuieSaStea(const Vehicul& v) const;
    // Jucatorul este la/aproape de intersectie (venind de jos pe verticala)?
    bool jucatorEngageazaIntersectia() const;
    // Exista un NPC venit din DREAPTA jucatorului (caruia trebuie sa-i cedeze)?
    bool existaNpcDinDreapta() const;

    Ecran ecran_;
    Input input_;
    std::unique_ptr<Harta> harta_;
    std::unique_ptr<Intersectie> intersectie_;
    std::unique_ptr<MasinaJucator> jucator_;

    Pozitie startJucator_;

    int scor_  = 0;
    int vieti_ = 3;
    int indexRegula_ = 0;
    int treceriReusite_ = 0;
    int tintaIntersectii_ = 5;   // cate intersectii trebuie trecute pentru a castiga

    // Stare regula / intersectie
    bool eraSubIntersectie_ = true;
    bool aIntratInBox_ = false;
    double timpStationar_ = 0.0;

    // Contrasens
    bool peContrasens_ = false;
    std::chrono::steady_clock::time_point startContrasens_;

    bool eraOffRoad_ = false;

    // Mesaj temporar (ex: "Intersectie trecuta! +100")
    std::string mesaj_;
    double mesajTimer_ = 0.0;

    // Pauza intre niveluri (ca jucatorul sa nu incalce instant regula urmatoare)
    double tranzitieTimer_ = 0.0;

    // Trafic NPC
    std::vector<Npc> npcuri_;
    int cadreSpawn_ = 0;
};

#endif // JOC_H
