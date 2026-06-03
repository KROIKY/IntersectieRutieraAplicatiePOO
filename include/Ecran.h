// =============================================================================
//  Ecran.h - gestionarea terminalului (Linux).
// -----------------------------------------------------------------------------
//  Aceasta clasa se ocupa de:
//    - intrarea/iesirea din modul "raw" al terminalului (termios);
//    - ascunderea cursorului si folosirea buffer-ului alternativ de ecran;
//    - pozitionarea cu secvente ANSI (\033[linie;coloanaH);
//    - pastrarea unei "harti statice" (fundalul) desenata O SINGURA DATA;
//    - redesenarea SELECTIVA: doar celulele care se schimba (vehiculele).
//
//  Ideea centrala a proiectului: fundalul (intersectia) este fix si se deseneaza
//  o singura data, iar la fiecare cadru se sterge/redeseneaza doar vehiculul.
//  Astfel nu apare palpaire (flicker), pentru ca nu stergem tot ecranul.
// =============================================================================
#ifndef ECRAN_H
#define ECRAN_H

#include <string>
#include <vector>
#include <termios.h>

class Ecran {
public:
    Ecran() = default;
    ~Ecran();

    // Pregateste terminalul: mod raw, fara ecou, ascunde cursorul, curata ecranul.
    void initializeaza();
    // Readuce terminalul la starea normala (cursor vizibil, mod canonic).
    void restaureaza();

    int linii() const { return nrLinii_; }
    int coloane() const { return nrColoane_; }

    // Defineste harta statica (fundalul). Fiecare string este o linie.
    void seteazaFundal(const std::vector<std::string>& harta);
    // Deseneaza intreaga harta statica (se apeleaza o singura data per nivel).
    void deseneazaFundal();

    // Returneaza caracterul de fundal de la o pozitie (pentru a "sterge" un vehicul).
    char fundalLa(int linie, int coloana) const;
    // Verifica daca o pozitie este in interiorul ecranului.
    bool inInterior(int linie, int coloana) const;

    // Adauga in buffer scrierea unui caracter / text la o pozitie.
    void scrieLa(int linie, int coloana, char ch);
    void scrieLa(int linie, int coloana, const std::string& text);
    // "Sterge" o pozitie rescriind caracterul de fundal de acolo.
    void stergeLa(int linie, int coloana);

    // Trimite efectiv buffer-ul acumulat catre terminal.
    void actualizeaza();

    // Handler pentru semnale (ex: Ctrl+C) - restaureaza terminalul si iese.
    static void laSemnal(int semnal);

private:
    void detecteazaDimensiuni();
    void scrieDirect(const std::string& s); // scrie imediat pe terminal

    int nrLinii_ = 24;
    int nrColoane_ = 80;

    std::vector<std::string> fundal_; // harta statica
    std::string buffer_;              // secvente ANSI acumulate intr-un cadru

    struct termios termiosVechi_{};   // setarile terminalului inainte de raw mode
    bool initializat_ = false;
};

#endif // ECRAN_H
