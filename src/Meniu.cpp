// =============================================================================
//  Meniu.cpp - implementarea meniurilor cu butoane dreptunghiulare.
// =============================================================================
#include "Meniu.h"

#include <unistd.h> // usleep

void Meniu::deseneazaButon(int linieSus, int colCentru, int latime, int inaltime,
                           const std::string& text, bool selectat) {
    int colStanga = colCentru - latime / 2;

    char colt = selectat ? '#' : '+';
    char hor  = selectat ? '#' : '=';
    char ver  = selectat ? '#' : '|';
    // Selectat: galben aprins; neselectat: gri stins.
    std::string on  = selectat ? "\033[1;93m" : "\033[2;37m";
    std::string off = "\033[0m";

    auto scrie = [&](int lin, const std::string& continut) {
        ecran_.scrieLa(lin, colStanga, on + continut + off);
    };

    // Marginea de sus si de jos.
    std::string margine;
    margine += colt;
    margine += std::string(latime - 2, hor);
    margine += colt;

    scrie(linieSus, margine);

    int randMijloc = inaltime / 2;
    for (int r = 1; r < inaltime - 1; ++r) {
        std::string rand;
        if (r == randMijloc) {
            std::string et = selectat ? (">> " + text + " <<") : text;
            int interior = latime - 2;
            int pad = interior - static_cast<int>(et.size());
            int st = pad > 0 ? pad / 2 : 0;
            std::string continut = std::string(st, ' ') + et;
            if (static_cast<int>(continut.size()) < interior)
                continut += std::string(interior - continut.size(), ' ');
            if (static_cast<int>(continut.size()) > interior)
                continut = continut.substr(0, interior);
            rand = std::string(1, ver) + continut + std::string(1, ver);
        } else {
            rand = std::string(1, ver) + std::string(latime - 2, ' ') + std::string(1, ver);
        }
        scrie(linieSus + r, rand);
    }

    scrie(linieSus + inaltime - 1, margine);
}

int Meniu::alege(const std::string& titlu, const std::vector<std::string>& optiuni) {
    const int L = ecran_.linii();
    const int C = ecran_.coloane();

    // Fundal gol pentru a curata ecranul sub meniu.
    std::vector<std::string> gol(L, std::string(C, ' '));
    ecran_.seteazaFundal(gol);
    ecran_.deseneazaFundal();

    int latimeBtn = 40;
    if (latimeBtn > C - 4) latimeBtn = C - 4;
    const int inaltimeBtn = 5;
    const int spatiu = 2;
    const int n = static_cast<int>(optiuni.size());
    const int totalH = n * (inaltimeBtn + spatiu);
    int startLinie = (L - totalH) / 2 + 1;
    if (startLinie < 4) startLinie = 4;

    auto centrat = [&](int lin, const std::string& t, const std::string& col) {
        int c = (C - static_cast<int>(t.size())) / 2;
        if (c < 0) c = 0;
        ecran_.scrieLa(lin, c, col + t + "\033[0m");
    };

    int sel = 0;
    while (true) {
        centrat(startLinie - 3, titlu, "\033[1;96m");
        for (int i = 0; i < n; ++i) {
            int lin = startLinie + i * (inaltimeBtn + spatiu);
            deseneazaButon(lin, C / 2, latimeBtn, inaltimeBtn, optiuni[i], i == sel);
        }
        centrat(startLinie + totalH + 1,
                "W/S sau sageti = navigare    Enter/Space = selecteaza    Q = inapoi",
                "\033[2;37m");
        ecran_.actualizeaza();

        Tasta t = input_.citeste();
        if (t == Tasta::Sus)      sel = (sel - 1 + n) % n;
        else if (t == Tasta::Jos) sel = (sel + 1) % n;
        else if (t == Tasta::Confirma) return sel;
        else if (t == Tasta::Iesire)   return -1;

        usleep(16000);
    }
}
