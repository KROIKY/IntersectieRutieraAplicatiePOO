// =============================================================================
//  test_reguli.cpp - teste pentru regulile de circulatie polimorfe.
// =============================================================================
#include "Test.h"

#include "RegulaSemafor.h"
#include "RegulaPrioritateDreapta.h"
#include "RegulaStop.h"

int main() {
    std::cout << "[test_reguli]\n";

    // --- RegulaSemafor: pe verde se poate intra, pe rosu nu ---
    {
        RegulaSemafor sem;
        ContextRegula ctx; // oprit/timp nu conteaza pentru semafor
        CHECK_EQ(sem.nume(), std::string("SEMAFOR"));

        // Implicit este Verde -> intrarea e permisa.
        CHECK(sem.permiteIntrarea(ctx));
        CHECK_EQ(sem.textStare(), std::string("VERDE"));

        // Avansam timpul pana in faza ROSU (ciclu: V 0-5s, G 5-6.5s, R 6.5-11.5s).
        sem.actualizeaza(7.0);
        CHECK(sem.culoare() == RegulaSemafor::Culoare::Rosu);
        CHECK(!sem.permiteIntrarea(ctx));
        CHECK_EQ(sem.textStare(),  std::string("ROSU"));
        CHECK_EQ(sem.codCuloare(), std::string("31")); // rosu ANSI

        // Inca 5s -> ciclul se inchide (12 -> 0.5s) si revine pe Verde.
        sem.actualizeaza(5.0);
        CHECK(sem.culoare() == RegulaSemafor::Culoare::Verde);
        CHECK(sem.permiteIntrarea(ctx));

        CHECK(!sem.motivIncalcare().empty());
    }

    // --- RegulaPrioritateDreapta: liber implicit, cedeaza cand i se cere ---
    {
        RegulaPrioritateDreapta rp;
        ContextRegula ctx;
        CHECK_EQ(rp.nume(), std::string("PRIORITATE DREAPTA"));

        // Implicit liber -> intrarea permisa.
        CHECK(rp.permiteIntrarea(ctx));
        CHECK_EQ(rp.textStare(), std::string("LIBER"));

        // Cand vine cineva din dreapta -> trebuie sa cedeze (intrare interzisa).
        rp.seteazaLiber(false);
        CHECK(!rp.permiteIntrarea(ctx));
        CHECK_EQ(rp.textStare(),  std::string("CEDEAZA"));
        CHECK_EQ(rp.codCuloare(), std::string("33")); // galben ANSI

        rp.seteazaLiber(true);
        CHECK(rp.permiteIntrarea(ctx));

        CHECK(!rp.motivIncalcare().empty());
    }

    // --- Tratare polimorfica printr-un pointer la clasa de baza ---
    {
        RegulaCirculatie* r = new RegulaSemafor();
        CHECK_EQ(r->nume(), std::string("SEMAFOR"));
        delete r;
    }

    // --- RegulaStop (scoasa din rotatie, dar trebuie sa ramana coerenta) ---
    {
        RegulaStop stop;
        CHECK(!stop.nume().empty());
        CHECK(!stop.motivIncalcare().empty());

        // Nu a stat oprit destul -> nu poate intra.
        ContextRegula fara; fara.oprit = false; fara.timpStationar = 0.0;
        CHECK(!stop.permiteIntrarea(fara));

        // A stat oprit suficient -> poate intra.
        ContextRegula dupa; dupa.oprit = true; dupa.timpStationar = 5.0;
        CHECK(stop.permiteIntrarea(dupa));
    }

    return test::rezumat();
}
