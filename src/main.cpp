// =============================================================================
//  Cod Rutier - Simulare de trafic rutier in consola
//  Tema 3121A - Proiect Programare Orientata pe Obiecte (C++)
// -----------------------------------------------------------------------------
//  Punctul de intrare. Toata logica este in clasa Joc; main doar o porneste.
// =============================================================================

#include "Joc.h"

#include <exception>
#include <iostream>

int main() {
    try {
        Joc joc;
        joc.ruleaza();
    } catch (const std::exception& e) {
        // Plasa de siguranta: daca apare o exceptie, mesajul ajunge in terminal
        // (terminalul este readus la normal de destructorul clasei Ecran).
        std::cerr << "Eroare: " << e.what() << "\n";
        return 1;
    }
    std::cout << "Iesire din joc. Pe curand!\n";
    return 0;
}
