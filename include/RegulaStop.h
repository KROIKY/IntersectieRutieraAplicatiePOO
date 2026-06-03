// =============================================================================
//  RegulaStop.h - regula indicatorului STOP.
// -----------------------------------------------------------------------------
//  Vehiculul are voie sa intre numai dupa ce a oprit complet aproximativ o
//  secunda in zona de dinaintea intersectiei.
// =============================================================================
#ifndef REGULA_STOP_H
#define REGULA_STOP_H

#include "RegulaCirculatie.h"

class RegulaStop : public RegulaCirculatie {
public:
    std::string nume() const override { return "STOP"; }

    bool permiteIntrarea(const ContextRegula& ctx) const override {
        return ctx.timpStationar >= 1.0; // a oprit complet ~1 secunda
    }

    std::string textStare() const override { return "STOP"; }
    std::string codCuloare() const override { return "31"; } // rosu
    std::string motivIncalcare() const override {
        return "Nu ai oprit la indicatorul STOP!";
    }
};

#endif // REGULA_STOP_H
