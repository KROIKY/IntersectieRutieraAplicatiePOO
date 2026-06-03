// =============================================================================
//  test_vehicul.cpp - teste pentru ierarhia Vehicul (mostenire + polimorfism).
// =============================================================================
#include "Test.h"

#include "Masina.h"
#include "MasinaJucator.h"
#include "Motocicleta.h"
#include "Camion.h"

int main() {
    std::cout << "[test_vehicul]\n";

    // --- Polimorfism: fiecare tip raspunde diferit la tip()/vitezaMaxima() ---
    Masina      m({10, 10});
    Motocicleta moto({10, 10});
    Camion      cam({10, 10});
    MasinaJucator jucator({10, 10});

    CHECK_EQ(m.tip(),       std::string("Masina"));
    CHECK_EQ(moto.tip(),    std::string("Motocicleta"));
    CHECK_EQ(cam.tip(),     std::string("Camion"));
    CHECK_EQ(jucator.tip(), std::string("Masina (jucator)"));

    // Motocicleta e cea mai rapida, camionul cel mai lent.
    CHECK(moto.vitezaMaxima() > m.vitezaMaxima());
    CHECK(m.vitezaMaxima()    > cam.vitezaMaxima());

    // --- Apel polimorfic prin pointer la clasa de baza ---
    Vehicul* v = &cam;
    CHECK_EQ(v->tip(), std::string("Camion"));
    CHECK_EQ(v->vitezaMaxima(), 2);

    // --- Dimensiunile sprite-ului sunt pozitive ---
    CHECK(m.inaltime() > 0);
    CHECK(m.latime()   > 0);

    // --- Deplasare: setPozitie modifica pozitia, directia ramane separata ---
    Masina mob({5, 5}, Directie::Sus);
    mob.setPozitie({7, 9});
    CHECK_EQ(mob.pozitie().linie,   7);
    CHECK_EQ(mob.pozitie().coloana, 9);

    // --- Sprite-ul se schimba intre orientarea verticala si orizontala ---
    Masina rot({0, 0}, Directie::Sus);
    int latimeVertical = rot.latime();
    rot.setDirectie(Directie::Dreapta);
    int latimeOrizontal = rot.latime();
    // Varianta orizontala e mai lata decat cea verticala (vehicul "intins").
    CHECK(latimeOrizontal >= latimeVertical);

    // --- ocupaCelula: coltul stanga-sus al sprite-ului este la pozitie ---
    Masina ocup({4, 4}, Directie::Sus);
    Pozitie p = ocup.pozitie();
    // Cel putin o celula din interiorul sprite-ului trebuie sa fie ocupata.
    bool gasitOcupata = false;
    for (int r = 0; r < ocup.inaltime() && !gasitOcupata; ++r)
        for (int c = 0; c < ocup.latime() && !gasitOcupata; ++c)
            if (ocup.ocupaCelula(p.linie + r, p.coloana + c)) gasitOcupata = true;
    CHECK(gasitOcupata);
    // O celula clar in afara sprite-ului nu este ocupata.
    CHECK(!ocup.ocupaCelula(p.linie - 1, p.coloana - 1));

    return test::rezumat();
}
