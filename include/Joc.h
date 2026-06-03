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
    void deseneazaStatus(const std::string& avertisment);
    bool esteOffRoad(const Pozitie& poz) const;
    void afiseazaGameOver(const std::string& motiv);
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
    bool exista_npc_in_intersectie() const;  // pentru prioritatea de dreapta
    bool coliziuneCuNpc() const;             // jucatorul a lovit un NPC?
    bool npcAreRosu() const;                 // semaforul e rosu pentru trafic NPC?

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
