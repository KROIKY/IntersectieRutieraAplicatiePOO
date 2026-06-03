// =============================================================================
//  test_harta.cpp - teste pentru geometria hartii si regulile spatiale.
// =============================================================================
#include "Test.h"

#include "Harta.h"
#include "Masina.h"

int main() {
    std::cout << "[test_harta]\n";

    Harta h(40, 120);
    const Geometrie& g = h.geometrie();

    // --- Geometria de baza este coerenta ---
    CHECK_EQ(g.linii,   40);
    CHECK_EQ(g.coloane, 120);
    CHECK(g.vColStanga < g.midC && g.midC < g.vColDreapta);
    CHECK(g.hRowSus    < g.midR && g.midR < g.hRowJos);

    // --- Asfalt vs. iarba ---
    // Centrul intersectiei este asfalt si in intersectie.
    CHECK(h.esteAsfalt(g.midR, g.midC));
    CHECK(h.esteInIntersectie(g.midR, g.midC));
    // Coltul (0,0) este in afara carosabilului (iarba).
    CHECK(!h.esteAsfalt(0, 0));
    CHECK(!h.esteInIntersectie(0, 0));
    // Pe drumul orizontal, dar in afara patratului central -> asfalt, dar nu intersectie.
    CHECK(h.esteAsfalt(g.midR, 1));
    CHECK(!h.esteInIntersectie(g.midR, 1));

    // --- Contrasens (circulatie pe dreapta) ---
    // Mergand in SUS, banda corecta e la DREAPTA liniei centrale (coloana > midC).
    // Plasam sub intersectie (linie > hRowJos), pe banda din STANGA -> contrasens.
    {
        Masina gresit({static_cast<int>(g.hRowJos + 5), g.vColStanga + 1}, Directie::Sus);
        CHECK(h.esteContrasens(gresit));
    }
    // Aceeasi zona, dar pe banda din dreapta -> corect, fara contrasens.
    {
        Masina corect({static_cast<int>(g.hRowJos + 5), g.midC + 2}, Directie::Sus);
        CHECK(!h.esteContrasens(corect));
    }
    // In interiorul intersectiei nu se evalueaza contrasensul.
    {
        Masina inBox({g.midR, g.vColStanga + 1}, Directie::Sus);
        CHECK(!h.esteContrasens(inBox));
    }

    return test::rezumat();
}
