// =============================================================================
//  Meniu.h - meniuri simple in stil "old-school", cu butoane dreptunghiulare.
// -----------------------------------------------------------------------------
//  Afiseaza un titlu si o lista de optiuni desenate ca dreptunghiuri mari.
//  Navigarea se face cu W/S sau sageti, selectia cu Enter/Space, iar Q/Esc
//  inseamna "inapoi".
// =============================================================================
#ifndef MENIU_H
#define MENIU_H

#include "Ecran.h"
#include "Input.h"

#include <string>
#include <vector>

class Meniu {
public:
    Meniu(Ecran& ecran, Input& input) : ecran_(ecran), input_(input) {}

    // Returneaza indexul optiunii alese (0..n-1), sau -1 daca s-a apasat Q/Esc.
    int alege(const std::string& titlu, const std::vector<std::string>& optiuni);

private:
    void deseneazaButon(int linieSus, int colCentru, int latime, int inaltime,
                        const std::string& text, bool selectat);

    Ecran& ecran_;
    Input& input_;
};

#endif // MENIU_H
