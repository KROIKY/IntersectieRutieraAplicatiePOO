// =============================================================================
//  Joc.cpp - implementarea buclei principale, a regulilor si a traficului NPC.
// =============================================================================
#include "Joc.h"
#include "RegulaStop.h"
#include "RegulaSemafor.h"
#include "RegulaPrioritateDreapta.h"
#include "Masina.h"
#include "Motocicleta.h"
#include "Camion.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unistd.h> // usleep

namespace {
    const double SECUNDE_CONTRASENS = 3.0; // limita pe contrasens
    const double DT = 0.016;               // pas de timp aproximativ (60 FPS)
    const int    NR_REGULI = 2; // semafor + prioritate de dreapta (STOP eliminat)
    const int    MAX_NPC = 3;
    const int    CADRE_INTRE_SPAWN = 80;   // ~1.3 secunde

    // Suprapunere de dreptunghiuri (cu o distanta 'gap' pe coloane) - folosita
    // pentru a pastra distanta intre vehiculele NPC din aceeasi banda.
    bool boxSeSuprapune(const Pozitie& pa, int wa, int ha,
                        const Pozitie& pb, int wb, int hb, int gap) {
        bool peLinii   = !(pa.linie + ha - 1 < pb.linie || pa.linie > pb.linie + hb - 1);
        bool peColoane = !(pa.coloana + wa - 1 + gap < pb.coloana ||
                           pa.coloana > pb.coloana + wb - 1 + gap);
        return peLinii && peColoane;
    }

    // Adevarat daca vreo celula ocupata de 'a' coincide cu o celula a lui 'b'.
    bool celuleSeSuprapun(const Vehicul& a, const Vehicul& b) {
        const auto& s = a.sprite();
        Pozitie p = a.pozitie();
        for (int r = 0; r < static_cast<int>(s.size()); ++r) {
            for (int c = 0; c < static_cast<int>(s[r].size()); ++c) {
                if (s[r][c] == ' ') continue;
                if (b.ocupaCelula(p.linie + r, p.coloana + c)) return true;
            }
        }
        return false;
    }
}

std::unique_ptr<RegulaCirculatie> Joc::creeazaRegula(int index) const {
    // Regula STOP a fost eliminata la cererea utilizatorului.
    switch (index % NR_REGULI) {
        case 0:  return std::make_unique<RegulaSemafor>();
        default: return std::make_unique<RegulaPrioritateDreapta>();
    }
}

bool Joc::esteOffRoad(const Pozitie& poz) const {
    const auto& s = jucator_->sprite();
    for (int r = 0; r < static_cast<int>(s.size()); ++r) {
        for (int c = 0; c < static_cast<int>(s[r].size()); ++c) {
            if (s[r][c] == ' ') continue;
            if (!harta_->esteAsfalt(poz.linie + r, poz.coloana + c)) return true;
        }
    }
    return false;
}

void Joc::spawnNpc() {
    if (static_cast<int>(npcuri_.size()) >= MAX_NPC) return;

    const Geometrie& g = harta_->geometrie();
    int tip = std::rand() % 3;
    bool dinDreapta = (std::rand() % 2 == 0);
    Directie d = dinDreapta ? Directie::Stanga : Directie::Dreapta;

    // Banda de sus pentru cei care vin din dreapta, banda de jos pentru ceilalti.
    int row = dinDreapta ? g.hRowSus : (g.midR + 1);
    Pozitie p(row, 0);

    std::unique_ptr<Vehicul> v;
    if      (tip == 0) v = std::make_unique<Masina>(p, d);
    else if (tip == 1) v = std::make_unique<Motocicleta>(p, d);
    else               v = std::make_unique<Camion>(p, d);

    // Pozitie de start in afara ecranului, pentru a intra lin.
    p.coloana = dinDreapta ? g.coloane : -v->latime();
    v->setPozitie(p);

    int interval = (6 - v->vitezaMaxima()) * 2; // viteza mai mare => interval mic
    if (interval < 2) interval = 2;

    npcuri_.push_back(Npc{std::move(v), interval, 0});
}

void Joc::actualizeazaNpc() {
    const Geometrie& g = harta_->geometrie();

    for (size_t i = 0; i < npcuri_.size(); ++i) {
        Npc& n = npcuri_[i];
        if (++n.contor < n.interval) continue;
        n.contor = 0;

        Pozitie p = n.v->pozitie();
        int w = n.v->latime();
        int h = n.v->inaltime();
        Directie d = n.v->directie();

        Pozitie pNext = p;
        if (d == Directie::Stanga) pNext.coloana--;
        else                       pNext.coloana++;

        // 1) Oprire la limita intersectiei: semafor rosu pentru NPC, sau cedarea
        //    catre jucator la regula de prioritate de dreapta. Folosim o banda
        //    de cateva celule la intrare, ca vehiculul sa nu "scape" peste linie.
        bool opritLaIntersectie = false;
        if (npcTrebuieSaStea(*n.v)) {
            if (d == Directie::Dreapta) {
                int front = p.coloana + w - 1;
                if (front >= g.vColStanga - 1 && front <= g.vColStanga + 1)
                    opritLaIntersectie = true;
            } else { // Stanga
                int front = p.coloana;
                if (front <= g.vColDreapta + 1 && front >= g.vColDreapta - 1)
                    opritLaIntersectie = true;
            }
        }

        // 2) Pastrarea distantei: nu intra peste un alt NPC din fata.
        bool blocat = false;
        if (!opritLaIntersectie) {
            for (size_t j = 0; j < npcuri_.size() && !blocat; ++j) {
                if (j == i) continue;
                blocat = boxSeSuprapune(pNext, w, h,
                                        npcuri_[j].v->pozitie(),
                                        npcuri_[j].v->latime(),
                                        npcuri_[j].v->inaltime(), 1);
            }
        }

        if (!opritLaIntersectie && !blocat) n.v->setPozitie(pNext);
    }

    // Eliminam vehiculele care au iesit complet din ecran.
    npcuri_.erase(std::remove_if(npcuri_.begin(), npcuri_.end(), [&](const Npc& n) {
        Pozitie p = n.v->pozitie();
        if (n.v->directie() == Directie::Stanga) return p.coloana + n.v->latime() < 0;
        return p.coloana > g.coloane;
    }), npcuri_.end());
}

bool Joc::jucatorEngageazaIntersectia() const {
    const Geometrie& g = harta_->geometrie();
    int cr = jucator_->centruLinie();
    int cc = jucator_->centruColoana();
    bool peVertical = (cc >= g.vColStanga && cc <= g.vColDreapta);
    // In intersectie sau pe abordarea de jos, aproape de ea.
    bool aproape = (cr >= g.hRowSus - 2 && cr <= g.hRowJos + 6);
    return peVertical && aproape;
}

bool Joc::existaNpcDinDreapta() const {
    // Vehicul venit din dreapta jucatorului = se deplaseaza spre stanga (Stanga),
    // aflat in intersectie sau pe abordarea dreapta, aproape de ea.
    const Geometrie& g = harta_->geometrie();
    const int MARJA = 12;
    for (const auto& n : npcuri_) {
        if (n.v->directie() != Directie::Stanga) continue;
        Pozitie p = n.v->pozitie();
        int w = n.v->latime();
        bool nuAtrecut  = (p.coloana + w - 1 >= g.vColStanga); // inca nu a trecut de box
        bool eAproape   = (p.coloana <= g.vColDreapta + MARJA); // e in/langa box pe dreapta
        if (nuAtrecut && eAproape) return true;
    }
    return false;
}

bool Joc::npcTrebuieSaStea(const Vehicul& v) const {
    // Semafor: NPC are rosu cand jucatorul NU are rosu.
    if (auto* sem = dynamic_cast<RegulaSemafor*>(&intersectie_->regula())) {
        return sem->culoare() != RegulaSemafor::Culoare::Rosu;
    }
    // Prioritate de dreapta: NPC venit din STANGA jucatorului (merge spre dreapta)
    // ii cedeaza jucatorului cand acesta este la/aproape de intersectie.
    if (dynamic_cast<RegulaPrioritateDreapta*>(&intersectie_->regula())) {
        if (v.directie() == Directie::Dreapta) return jucatorEngageazaIntersectia();
        return false; // NPC din dreapta jucatorului are prioritate, nu se opreste
    }
    return false;
}

bool Joc::coliziuneCuNpc() const {
    for (const auto& n : npcuri_) {
        if (celuleSeSuprapun(*jucator_, *n.v)) return true;
    }
    return false;
}

void Joc::deseneazaStatus(const std::string& avertisment) {
    const int L = ecran_.linii();
    const int C = ecran_.coloane();

    std::string status = " Scor: " + std::to_string(scor_) +
                         "   Vieti: " + std::to_string(vieti_) +
                         "   Treceri: " + std::to_string(treceriReusite_) +
                         "   [WASD/sageti] misca, P pauza, Q iesire";
    if (!avertisment.empty()) status += "   " + avertisment;

    if (static_cast<int>(status.size()) > C) status.resize(C);
    else status.resize(C, ' ');

    ecran_.scrieLa(L - 1, 0, status);
}

void Joc::reseteazaJucator() {
    // Desenarea/stergerea sunt gestionate de fazele buclei; aici doar starea.
    jucator_->setPozitie(startJucator_);
    jucator_->setDirectie(Directie::Sus);
    eraSubIntersectie_ = true;
    aIntratInBox_ = false;
    timpStationar_ = 0.0;
    peContrasens_ = false;
    eraOffRoad_ = esteOffRoad(startJucator_);
}

void Joc::schimbaRegula() {
    indexRegula_ = (indexRegula_ + 1) % NR_REGULI;
    intersectie_->seteazaRegula(creeazaRegula(indexRegula_));
}

void Joc::ruleaza() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    ecran_.initializeaza();

    const int L = ecran_.linii();
    const int C = ecran_.coloane();

    harta_ = std::make_unique<Harta>(L, C);
    ecran_.seteazaFundal(harta_->grila());
    ecran_.deseneazaFundal();

    intersectie_ = std::make_unique<Intersectie>(harta_->geometrie(),
                                                 creeazaRegula(indexRegula_));

    const Geometrie& g = harta_->geometrie();
    startJucator_ = Pozitie(L - 4, g.midC + 3);
    jucator_ = std::make_unique<MasinaJucator>(startJucator_, Directie::Sus);

    jucator_->deseneaza(ecran_);
    intersectie_->deseneaza(ecran_);
    deseneazaStatus("");
    ecran_.actualizeaza();

    eraOffRoad_ = esteOffRoad(startJucator_);
    eraSubIntersectie_ = intersectie_->subIntersectie(*jucator_);

    const int minLinie = 0;
    const int maxLinie = L - 1 - jucator_->inaltime();
    const int minCol   = 0;
    const int maxCol   = C - jucator_->latime();

    bool pauza = false;
    bool gameOver = false;

    while (true) {
        // ---- FAZA A: stergem toate vehiculele de pe pozitiile curente ----
        jucator_->sterge(ecran_);
        for (auto& n : npcuri_) n.v->sterge(ecran_);

        // ---- FAZA B: input ----
        Tasta t = input_.citeste();
        if (t == Tasta::Iesire) break;

        // ---- Pauza de tranzitie intre niveluri (ca sa nu incalci instant
        //      regula urmatoare daca tii apasata acceleratia) ----
        if (tranzitieTimer_ > 0.0) {
            tranzitieTimer_ -= DT;
            bool gata = (tranzitieTimer_ <= 0.0);
            if (gata) ecran_.deseneazaFundal(); // curata banner-ul de pe fundal
            for (auto& n : npcuri_) n.v->deseneaza(ecran_);
            jucator_->deseneaza(ecran_);
            intersectie_->deseneaza(ecran_);
            if (!gata) {
                int nivel = treceriReusite_ + 1;
                auto centrat = [&](int linie, const std::string& txt) {
                    int col = (C - static_cast<int>(txt.size())) / 2;
                    if (col < 0) col = 0;
                    ecran_.scrieLa(linie, col, txt);
                };
                centrat(L / 2 - 1, "==================================");
                centrat(L / 2,     "  NIVELUL " + std::to_string(nivel) +
                                   "   -   Regula: " + intersectie_->regula().nume() + "  ");
                centrat(L / 2 + 1, "==================================");
                deseneazaStatus("Pregateste-te...");
            } else {
                deseneazaStatus("");
            }
            ecran_.actualizeaza();
            usleep(16000);
            continue;
        }

        bool s_a_miscat = false;
        if (t == Tasta::Pauza) {
            pauza = !pauza;
        } else if (!pauza && t != Tasta::Niciuna) {
            Pozitie nou  = jucator_->pozitie();
            Directie dir = jucator_->directie();
            switch (t) {
                case Tasta::Sus:     nou.linie--;   dir = Directie::Sus;     break;
                case Tasta::Jos:     nou.linie++;   dir = Directie::Jos;     break;
                case Tasta::Stanga:  nou.coloana--; dir = Directie::Stanga;  break;
                case Tasta::Dreapta: nou.coloana++; dir = Directie::Dreapta; break;
                default: break;
            }
            if (nou.linie < minLinie) nou.linie = minLinie;
            if (nou.linie > maxLinie) nou.linie = maxLinie;
            if (nou.coloana < minCol) nou.coloana = minCol;
            if (nou.coloana > maxCol) nou.coloana = maxCol;

            if (nou != jucator_->pozitie() || dir != jucator_->directie()) {
                jucator_->setPozitie(nou);
                jucator_->setDirectie(dir);
                s_a_miscat = true;
            }
        }

        std::string avert;

        if (!pauza) {
            // ---- Trafic NPC ----
            if (--cadreSpawn_ <= 0) { spawnNpc(); cadreSpawn_ = CADRE_INTRE_SPAWN; }
            actualizeazaNpc();

            intersectie_->actualizeaza(DT);

            // Prioritate de dreapta: jucatorul cedeaza DOAR vehiculelor venite
            // din dreapta lui; e liber daca nu vine niciunul din dreapta.
            if (auto* rp = dynamic_cast<RegulaPrioritateDreapta*>(&intersectie_->regula())) {
                rp->seteazaLiber(!existaNpcDinDreapta());
            }

            // Cronometrarea opririi in zona de STOP.
            if (intersectie_->inZonaStop(*jucator_) && !s_a_miscat) timpStationar_ += DT;
            else if (s_a_miscat) timpStationar_ = 0.0;

            // Intrarea in intersectie -> evaluarea regulii.
            bool inBoxAcum = intersectie_->inBox(*jucator_);
            if (inBoxAcum && eraSubIntersectie_ && !aIntratInBox_) {
                ContextRegula ctx;
                ctx.oprit = !s_a_miscat;
                ctx.timpStationar = timpStationar_;
                if (!intersectie_->regula().permiteIntrarea(ctx)) {
                    for (auto& n : npcuri_) n.v->deseneaza(ecran_);
                    afiseazaGameOver(intersectie_->regula().motivIncalcare());
                    gameOver = true; break;
                }
                aIntratInBox_ = true;
            }

            // Trecere reusita -> a iesit prin ORICARE brat (sus / stanga / dreapta).
            if (aIntratInBox_ && (intersectie_->deasupraIntersectiei(*jucator_) ||
                                  intersectie_->inStangaIntersectiei(*jucator_) ||
                                  intersectie_->inDreaptaIntersectiei(*jucator_))) {
                scor_ += 100;
                ++treceriReusite_;
                schimbaRegula();
                reseteazaJucator();
                tranzitieTimer_ = 2.0; // pauza scurta inainte de nivelul urmator
            }
            eraSubIntersectie_ = intersectie_->subIntersectie(*jucator_);

            // ---- FAZA C: coliziune cu un NPC ----
            if (coliziuneCuNpc()) {
                for (auto& n : npcuri_) n.v->deseneaza(ecran_);
                jucator_->deseneaza(ecran_);
                afiseazaGameOver("Coliziune cu un vehicul!");
                gameOver = true; break;
            }

            // Penalizare la iesirea de pe carosabil.
            bool offAcum = esteOffRoad(jucator_->pozitie());
            if (offAcum && !eraOffRoad_) vieti_--;
            eraOffRoad_ = offAcum;
            if (vieti_ <= 0) {
                for (auto& n : npcuri_) n.v->deseneaza(ecran_);
                afiseazaGameOver("Ai ramas fara vieti (iesiri de pe carosabil).");
                gameOver = true; break;
            }

            // Regula contrasensului.
            bool contra = harta_->esteContrasens(*jucator_);
            if (contra) {
                auto acum = std::chrono::steady_clock::now();
                if (!peContrasens_) { peContrasens_ = true; startContrasens_ = acum; }
                double trecut = std::chrono::duration<double>(acum - startContrasens_).count();
                double ramas  = SECUNDE_CONTRASENS - trecut;
                if (ramas <= 0.0) {
                    for (auto& n : npcuri_) n.v->deseneaza(ecran_);
                    afiseazaGameOver("Ai stat prea mult pe contrasens!");
                    gameOver = true; break;
                }
                avert = "!! CONTRASENS !! Revino pe banda ta in " +
                        std::to_string(static_cast<int>(std::ceil(ramas))) + "s";
            } else {
                peContrasens_ = false;
                if (offAcum) avert = "Atentie: in afara carosabilului!";
            }
        } else {
            avert = "-- PAUZA --";
        }

        // ---- FAZA D: desenam toate vehiculele (NPC sub, jucatorul deasupra) ----
        for (auto& n : npcuri_) n.v->deseneaza(ecran_);
        jucator_->deseneaza(ecran_);

        intersectie_->deseneaza(ecran_);
        deseneazaStatus(avert);
        ecran_.actualizeaza();

        usleep(16000);
    }

    if (gameOver) asteaptaIesire();
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
    centrat(r + 1, "Scor final: " + std::to_string(scor_) +
                   "   Treceri reusite: " + std::to_string(treceriReusite_));
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
