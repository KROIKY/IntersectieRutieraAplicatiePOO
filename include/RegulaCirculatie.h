// =============================================================================
//  RegulaCirculatie.h - clasa abstracta pentru regulile de la intersectie.
// -----------------------------------------------------------------------------
//  Polimorfism: fiecare regula (STOP, semafor, prioritate de dreapta) decide
//  diferit daca vehiculul are voie sa intre in intersectie.
// =============================================================================
#ifndef REGULA_CIRCULATIE_H
#define REGULA_CIRCULATIE_H

#include <string>

// Informatii despre starea jucatorului in momentul evaluarii regulii.
struct ContextRegula {
    bool   oprit = false;          // vehiculul sta pe loc acum?
    double timpStationar = 0.0;    // de cate secunde sta pe loc in zona de stop
};

class RegulaCirculatie {
public:
    virtual ~RegulaCirculatie() = default;

    // Numele regulii (afisat jucatorului).
    virtual std::string nume() const = 0;

    // Actualizeaza starea interna (ex: culoarea semaforului). dt = secunde.
    virtual void actualizeaza(double /*dt*/) {}

    // Decide daca vehiculul poate intra in intersectie acum.
    virtual bool permiteIntrarea(const ContextRegula& ctx) const = 0;

    // Cuvantul de stare afisat (ex: "VERDE", "STOP").
    virtual std::string textStare() const { return nume(); }

    // Codul de culoare ANSI pentru cuvantul de stare (37 = alb implicit).
    virtual std::string codCuloare() const { return "37"; }

    // Mesajul afisat la incalcarea regulii (game over).
    virtual std::string motivIncalcare() const = 0;
};

#endif // REGULA_CIRCULATIE_H
