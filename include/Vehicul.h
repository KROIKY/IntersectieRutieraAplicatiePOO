// =============================================================================
//  Vehicul.h - clasa abstracta de baza pentru toate vehiculele.
// -----------------------------------------------------------------------------
//  Concepte POO ilustrate:
//    - clasa abstracta (metode virtuale pure: vitezaMaxima, tip, sprite);
//    - mostenire: Masina / Motocicleta / Camion deriva din Vehicul;
//    - polimorfism: fiecare tip raspunde diferit la vitezaMaxima()/tip();
//    - incapsulare: pozitia si directia sunt protejate.
//
//  Pozitia (poz_) reprezinta coltul stanga-sus al sprite-ului pe ecran.
// =============================================================================
#ifndef VEHICUL_H
#define VEHICUL_H

#include "Comun.h"
#include <string>
#include <vector>

class Ecran; // declaratie anticipata (evitam includerea Ecran.h aici)

class Vehicul {
public:
    explicit Vehicul(Pozitie poz, Directie dir = Directie::Sus)
        : poz_(poz), dir_(dir) {}
    virtual ~Vehicul() = default;

    // --- Interfata polimorfica (implementata de clasele derivate) ---
    virtual int vitezaMaxima() const = 0;                       // celule/tact
    virtual std::string tip() const = 0;                        // "Masina"...
    virtual const std::vector<std::string>& sprite() const = 0; // desenul ASCII

    // --- Accesori (incapsulare) ---
    Pozitie pozitie() const { return poz_; }
    void setPozitie(Pozitie p) { poz_ = p; }
    Directie directie() const { return dir_; }
    void setDirectie(Directie d) { dir_ = d; }

    // Dimensiunile sprite-ului curent.
    int inaltime() const;
    int latime() const;

    // Coloana / linia centrului vehiculului (utile pentru logica de banda).
    int centruColoana() const { return poz_.coloana + latime() / 2; }
    int centruLinie()   const { return poz_.linie   + inaltime() / 2; }

    // Adevarat daca celula (linie, coloana) este ocupata de un caracter
    // ne-transparent al sprite-ului.
    bool ocupaCelula(int linie, int coloana) const;

    // --- Desenare folosind Ecran (refresh selectiv) ---
    void deseneaza(Ecran& ecran) const; // scrie caracterele ne-spatiu
    void sterge(Ecran& ecran) const;    // restaureaza fundalul sub vehicul

protected:
    Pozitie poz_;
    Directie dir_;
};

#endif // VEHICUL_H
