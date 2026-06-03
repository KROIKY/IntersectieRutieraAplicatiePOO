// =============================================================================
//  Logger.h - jurnal de evenimente al jocului, scris intr-un fisier .log.
// -----------------------------------------------------------------------------
//  Concepte ilustrate:
//    - I/O cu fisiere (std::ofstream);
//    - RAII: fisierul se deschide in constructor si se inchide in destructor;
//    - incapsulare: stream-ul si formatarea sunt ascunse in spatele unei
//      interfete simple (eveniment(...)).
//
//  Fiecare linie din jurnal are forma:  [AAAA-LL-ZZ hh:mm:ss] <mesaj>
//  Daca fisierul nu poate fi deschis, logger-ul devine inactiv (nu arunca
//  exceptii), pentru a nu impiedica rularea jocului.
// =============================================================================
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

class Logger {
public:
    // Deschide fisierul in mod "append" (pastreaza istoricul intre rulari).
    explicit Logger(const std::string& caleFisier);
    ~Logger();

    // Scrie o linie de eveniment (cu marca temporala) in jurnal.
    void eveniment(const std::string& mesaj);

    // Adevarat daca fisierul a putut fi deschis pentru scriere.
    bool esteDeschis() const { return out_.is_open(); }

private:
    // Returneaza data si ora curenta sub forma "AAAA-LL-ZZ hh:mm:ss".
    static std::string acum();

    std::ofstream out_;
};

#endif // LOGGER_H
