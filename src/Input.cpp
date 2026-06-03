// =============================================================================
//  Input.cpp - implementarea citirii non-blocante a tastaturii.
// =============================================================================
#include "Input.h"

#include <unistd.h>   // read, STDIN_FILENO

Tasta Input::citeste() {
    char buf[32];
    ssize_t n = ::read(STDIN_FILENO, buf, sizeof(buf));
    if (n <= 0) {
        return Tasta::Niciuna; // nimic apasat in acest cadru
    }

    Tasta rezultat = Tasta::Niciuna;

    for (ssize_t i = 0; i < n; ++i) {
        char c = buf[i];

        if (c == '\033') {
            // Secventa de evadare: posibil o sageata (ESC [ A/B/C/D).
            if (i + 2 < n && buf[i + 1] == '[') {
                switch (buf[i + 2]) {
                    case 'A': rezultat = Tasta::Sus;     break;
                    case 'B': rezultat = Tasta::Jos;     break;
                    case 'C': rezultat = Tasta::Dreapta; break;
                    case 'D': rezultat = Tasta::Stanga;  break;
                    default:  break;
                }
                i += 2; // am consumat '[' si litera
            } else {
                // ESC simplu (fara secventa) = iesire din joc.
                rezultat = Tasta::Iesire;
            }
        } else {
            switch (c) {
                case 'w': case 'W': rezultat = Tasta::Sus;     break;
                case 's': case 'S': rezultat = Tasta::Jos;     break;
                case 'a': case 'A': rezultat = Tasta::Stanga;  break;
                case 'd': case 'D': rezultat = Tasta::Dreapta; break;
                case 'p': case 'P': rezultat = Tasta::Pauza;   break;
                case 'q': case 'Q': rezultat = Tasta::Iesire;  break;
                default: break;
            }
        }
    }

    return rezultat;
}
