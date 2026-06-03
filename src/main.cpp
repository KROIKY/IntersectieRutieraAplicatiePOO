// =============================================================================
//  Cod Rutier - Simulare de trafic rutier in consola
//  Tema 3121A - Proiect Programare Orientata pe Obiecte (C++)
// -----------------------------------------------------------------------------
//  Etapa 1: demonstratia motorului de terminal.
//    - se deseneaza o intersectie pe TOT ecranul, O SINGURA DATA (fundal static);
//    - un marcator '@' se misca cu WASD sau cu sagetile;
//    - la fiecare mutare se "sterge" doar pozitia veche si se redeseneaza cea
//      noua => fundalul nu palpaie, doar vehiculul se reimprospateaza.
//
//  Sprite-urile reale ale vehiculelor (Masina/Motocicleta/Camion) si ierarhia
//  de clase se adauga in Etapa 2.
// =============================================================================

#include "Comun.h"
#include "Ecran.h"
#include "Input.h"

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>   // usleep

// Construieste o harta statica de tip intersectie in cruce, dimensionata la
// terminalul curent. Returneaza un vector de linii (fiecare cat ecranul de lat).
static std::vector<std::string> construiesteIntersectie(int linii, int coloane) {
    std::vector<std::string> harta(linii, std::string(coloane, ' '));

    const int midR = linii / 2;
    const int midC = coloane / 2;
    const int vrL = midC - 5, vrR = midC + 5; // marginile drumului vertical
    const int hrT = midR - 3, hrB = midR + 3; // marginile drumului orizontal

    auto inDrumVertical   = [&](int c) { return c >= vrL && c <= vrR; };
    auto inDrumOrizontal  = [&](int r) { return r >= hrT && r <= hrB; };

    // Marginile drumurilor (intrerupte in zona intersectiei).
    for (int r = 0; r < linii; ++r) {
        if (!inDrumOrizontal(r)) {
            if (vrL >= 0)       harta[r][vrL] = '*';
            if (vrR < coloane)  harta[r][vrR] = '*';
        }
    }
    for (int c = 0; c < coloane; ++c) {
        if (!inDrumVertical(c)) {
            if (hrT >= 0)      harta[hrT][c] = '*';
            if (hrB < linii)   harta[hrB][c] = '*';
        }
    }

    // Liniile discontinue de pe mijlocul drumurilor.
    for (int r = 0; r < linii; ++r) {
        if (!inDrumOrizontal(r) && r % 2 == 0) harta[r][midC] = '|';
    }
    for (int c = 0; c < coloane; ++c) {
        if (!inDrumVertical(c) && c % 2 == 0) harta[midR][c] = '-';
    }

    // Bara de titlu (sus) si bara de informatii (jos).
    const std::string titlu = " COD RUTIER  -  Etapa 1: motor terminal (demo) ";
    const std::string hud   = " WASD / sageti = misca   |   P = pauza   |   Q / Esc = iesire ";
    int startTitlu = (coloane - static_cast<int>(titlu.size())) / 2;
    if (startTitlu < 0) startTitlu = 0;
    for (int i = 0; i < static_cast<int>(titlu.size()) && startTitlu + i < coloane; ++i) {
        harta[0][startTitlu + i] = titlu[i];
    }
    for (int i = 0; i < static_cast<int>(hud.size()) && i < coloane; ++i) {
        harta[linii - 1][i] = hud[i];
    }

    return harta;
}

int main() {
    Ecran ecran;
    ecran.initializeaza();

    const int linii   = ecran.linii();
    const int coloane = ecran.coloane();

    // 1) Desenam fundalul static (intersectia) o singura data.
    ecran.seteazaFundal(construiesteIntersectie(linii, coloane));
    ecran.deseneazaFundal();

    // 2) Plasam marcatorul (viitoarea masina) in centrul intersectiei.
    Pozitie poz(linii / 2, coloane / 2);
    ecran.scrieLa(poz.linie, poz.coloana, '@');
    ecran.actualizeaza();

    Input input;
    bool ruleaza = true;
    bool pauza   = false;

    // Limitele in care se poate misca marcatorul (lasam liber randul de titlu/HUD).
    const int minLinie = 1, maxLinie = linii - 2;
    const int minCol   = 1, maxCol   = coloane - 2;

    // 3) Bucla principala: doar marcatorul se reimprospateaza.
    while (ruleaza) {
        Tasta t = input.citeste();

        if (t == Tasta::Iesire) {
            ruleaza = false;
        } else if (t == Tasta::Pauza) {
            pauza = !pauza;
        } else if (!pauza && t != Tasta::Niciuna) {
            Pozitie nou = poz;
            switch (t) {
                case Tasta::Sus:     nou.linie--;   break;
                case Tasta::Jos:     nou.linie++;   break;
                case Tasta::Stanga:  nou.coloana--; break;
                case Tasta::Dreapta: nou.coloana++; break;
                default: break;
            }

            // Pastram marcatorul in interiorul zonei de joc.
            if (nou.linie < minLinie) nou.linie = minLinie;
            if (nou.linie > maxLinie) nou.linie = maxLinie;
            if (nou.coloana < minCol) nou.coloana = minCol;
            if (nou.coloana > maxCol) nou.coloana = maxCol;

            if (nou != poz) {
                ecran.stergeLa(poz.linie, poz.coloana); // restaureaza fundalul
                ecran.scrieLa(nou.linie, nou.coloana, '@');
                ecran.actualizeaza();
                poz = nou;
            }
        }

        usleep(16000); // ~60 cadre/sec, ca sa nu consumam 100% CPU
    }

    ecran.restaureaza();
    std::cout << "Iesire din joc. Pe curand!\n";
    return 0;
}
