// =============================================================================
//  RegulaSemafor.h - regula semaforului (rosu / galben / verde).
// -----------------------------------------------------------------------------
//  Culoarea se schimba ciclic in timp. Vehiculul nu are voie sa intre pe rosu.
// =============================================================================
#ifndef REGULA_SEMAFOR_H
#define REGULA_SEMAFOR_H

#include "RegulaCirculatie.h"

class RegulaSemafor : public RegulaCirculatie {
public:
    enum class Culoare { Verde, Galben, Rosu };

    std::string nume() const override { return "SEMAFOR"; }

    void actualizeaza(double dt) override {
        timp_ += dt;
        // Ciclu de 11.5 secunde: Verde 5s, Galben 1.5s, Rosu 5s.
        double t = timp_;
        while (t >= 11.5) t -= 11.5;
        if      (t < 5.0)  culoare_ = Culoare::Verde;
        else if (t < 6.5)  culoare_ = Culoare::Galben;
        else               culoare_ = Culoare::Rosu;
    }

    bool permiteIntrarea(const ContextRegula& /*ctx*/) const override {
        return culoare_ != Culoare::Rosu; // pe rosu este interzis
    }

    std::string textStare() const override {
        switch (culoare_) {
            case Culoare::Verde:  return "VERDE";
            case Culoare::Galben: return "GALBEN";
            case Culoare::Rosu:   return "ROSU";
        }
        return "?";
    }

    std::string codCuloare() const override {
        switch (culoare_) {
            case Culoare::Verde:  return "32"; // verde
            case Culoare::Galben: return "33"; // galben
            case Culoare::Rosu:   return "31"; // rosu
        }
        return "37";
    }

    std::string motivIncalcare() const override {
        return "Ai trecut pe ROSU la semafor!";
    }

    Culoare culoare() const { return culoare_; }

private:
    Culoare culoare_ = Culoare::Verde;
    double  timp_ = 0.0;
};

#endif // REGULA_SEMAFOR_H
