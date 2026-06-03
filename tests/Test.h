// =============================================================================
//  Test.h - micro-framework de teste unitare (fara dependinte externe).
// -----------------------------------------------------------------------------
//  Folosire:
//    #include "Test.h"
//    int main() {
//        CHECK(2 + 2 == 4);
//        CHECK_EQ(vehicul.tip(), "Masina");
//        return test::rezumat();
//    }
//
//  Fiecare fisier de test este compilat intr-un binar separat (vezi Makefile).
//  Macro-urile inregistreaza esecurile si afiseaza linia; rezumat() intoarce
//  numarul de esecuri (0 = succes), pe care Makefile il foloseste ca exit code.
// =============================================================================
#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>

namespace test {

inline int& nrVerificari() { static int n = 0; return n; }
inline int& nrEsecuri()    { static int n = 0; return n; }

// Inregistreaza rezultatul unei verificari. 'expr' este textul conditiei.
inline void inregistreaza(bool ok, const char* expr, const char* fisier, int linie) {
    ++nrVerificari();
    if (!ok) {
        ++nrEsecuri();
        std::cerr << "  ESEC: " << expr << "  (" << fisier << ":" << linie << ")\n";
    }
}

// Varianta cu valori afisate, pentru egalitati (folosita de CHECK_EQ).
template <typename A, typename B>
inline void inregistreazaEgal(const A& a, const B& b, const char* expr,
                              const char* fisier, int linie) {
    ++nrVerificari();
    if (!(a == b)) {
        ++nrEsecuri();
        std::cerr << "  ESEC: " << expr << "  (" << fisier << ":" << linie << ")\n"
                  << "        stanga = " << a << "\n"
                  << "        dreapta = " << b << "\n";
    }
}

inline int rezumat() {
    std::cout << "  " << (nrVerificari() - nrEsecuri()) << "/" << nrVerificari()
              << " verificari au trecut.\n";
    return nrEsecuri(); // 0 = succes (exit code pentru Makefile)
}

} // namespace test

#define CHECK(expr) \
    ::test::inregistreaza((expr), #expr, __FILE__, __LINE__)

#define CHECK_EQ(a, b) \
    ::test::inregistreazaEgal((a), (b), #a " == " #b, __FILE__, __LINE__)

#endif // TEST_H
