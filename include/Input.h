// =============================================================================
//  Input.h - citirea tastaturii in mod non-blocant.
// -----------------------------------------------------------------------------
//  Suporta atat tastele WASD, cat si sagetile (care sosesc ca secvente de
//  evadare ANSI: ESC [ A/B/C/D).
// =============================================================================
#ifndef INPUT_H
#define INPUT_H

// Tastele relevante pentru joc.
enum class Tasta {
    Niciuna,
    Sus,
    Jos,
    Stanga,
    Dreapta,
    Pauza,
    Iesire
};

class Input {
public:
    // Citeste tot ce s-a acumulat in buffer-ul tastaturii in acest cadru si
    // returneaza ultima tasta relevanta (sau Tasta::Niciuna daca nimic).
    // Necesita ca terminalul sa fie deja in mod raw non-blocant (vezi Ecran).
    Tasta citeste();
};

#endif // INPUT_H
