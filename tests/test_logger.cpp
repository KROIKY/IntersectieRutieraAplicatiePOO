// =============================================================================
//  test_logger.cpp - teste pentru jurnalul de evenimente (I/O cu fisiere).
// =============================================================================
#include "Test.h"

#include "Logger.h"

#include <cstdio>    // std::remove
#include <fstream>
#include <sstream>
#include <string>

namespace {
    // Citeste tot continutul unui fisier intr-un string.
    std::string citesteTot(const std::string& cale) {
        std::ifstream in(cale);
        std::stringstream ss;
        ss << in.rdbuf();
        return ss.str();
    }
}

int main() {
    std::cout << "[test_logger]\n";

    const std::string cale = "test_jurnal.tmp.log";
    std::remove(cale.c_str()); // pornim de la zero

    {
        Logger jurnal(cale);
        CHECK(jurnal.esteDeschis());
        jurnal.eveniment("Intrare in intersectie");
        jurnal.eveniment("Coliziune cu un vehicul");
        // Destructorul se apeleaza la iesirea din bloc -> fisierul se inchide.
    }

    std::string continut = citesteTot(cale);

    // Evenimentele scrise trebuie sa apara in fisier.
    CHECK(continut.find("Intrare in intersectie")      != std::string::npos);
    CHECK(continut.find("Coliziune cu un vehicul")      != std::string::npos);
    // Marcatorii de sesiune (constructor + destructor) trebuie sa fie prezenti.
    CHECK(continut.find("sesiune noua")      != std::string::npos);
    CHECK(continut.find("sesiune incheiata") != std::string::npos);
    // Fiecare eveniment are o marca temporala intre paranteze drepte.
    CHECK(continut.find("[") != std::string::npos);

    // --- Mod "append": o a doua sesiune pastreaza continutul anterior ---
    {
        Logger jurnal2(cale);
        jurnal2.eveniment("A doua sesiune");
    }
    std::string continut2 = citesteTot(cale);
    CHECK(continut2.find("Intrare in intersectie") != std::string::npos); // inca prezent
    CHECK(continut2.find("A doua sesiune")          != std::string::npos);
    CHECK(continut2.size() > continut.size());

    std::remove(cale.c_str()); // curatenie
    return test::rezumat();
}
