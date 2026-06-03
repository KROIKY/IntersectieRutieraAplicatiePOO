// =============================================================================
//  Ecran.cpp - implementarea gestionarii terminalului.
// =============================================================================
#include "Ecran.h"

#include <cstdlib>
#include <csignal>
#include <string>
#include <unistd.h>      // read, write, STDIN_FILENO, STDOUT_FILENO
#include <sys/ioctl.h>   // ioctl, TIOCGWINSZ, winsize

// --- Secvente ANSI utile -----------------------------------------------------
namespace {
    const char* ASCUNDE_CURSOR   = "\033[?25l";
    const char* ARATA_CURSOR     = "\033[?25h";
    const char* BUFFER_ALTERNATIV_ON  = "\033[?1049h"; // ecran separat (ca vim)
    const char* BUFFER_ALTERNATIV_OFF = "\033[?1049l";
    const char* CURATA_TOT       = "\033[2J";
    const char* ACASA            = "\033[H";

    // Pointer catre instanta curenta, folosit de handler-ul de semnal ca sa
    // poata restaura terminalul daca utilizatorul apasa Ctrl+C.
    Ecran* g_ecranCurent = nullptr;
}

Ecran::~Ecran() {
    restaureaza();
}

void Ecran::scrieDirect(const std::string& s) {
    // Scriere neintermediata pe iesirea standard (evitam std::cout in mod raw).
    ssize_t ignorat = ::write(STDOUT_FILENO, s.data(), s.size());
    (void)ignorat;
}

void Ecran::detecteazaDimensiuni() {
    struct winsize w{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_row > 0 && w.ws_col > 0) {
        nrLinii_   = w.ws_row;
        nrColoane_ = w.ws_col;
    } else {
        // Valori de rezerva daca nu putem interoga terminalul.
        nrLinii_   = 24;
        nrColoane_ = 80;
    }
}

void Ecran::initializeaza() {
    if (initializat_) return;

    detecteazaDimensiuni();

    // Salvam setarile curente si trecem in mod raw (fara ecou, necanonic).
    tcgetattr(STDIN_FILENO, &termiosVechi_);
    struct termios raw = termiosVechi_;
    raw.c_lflag &= ~(ICANON | ECHO); // pastram ISIG ca sa primim Ctrl+C
    raw.c_cc[VMIN]  = 0;             // read() nu blocheaza...
    raw.c_cc[VTIME] = 0;             // ...si nu asteapta deloc -> input non-blocant
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    // Trecem pe ecranul alternativ, ascundem cursorul si curatam.
    scrieDirect(BUFFER_ALTERNATIV_ON);
    scrieDirect(ASCUNDE_CURSOR);
    scrieDirect(CURATA_TOT);
    scrieDirect(ACASA);

    // Inregistram handler-ul de semnal pentru a restaura terminalul la Ctrl+C.
    g_ecranCurent = this;
    std::signal(SIGINT,  Ecran::laSemnal);
    std::signal(SIGTERM, Ecran::laSemnal);

    initializat_ = true;
}

void Ecran::restaureaza() {
    if (!initializat_) return;

    scrieDirect(ARATA_CURSOR);
    scrieDirect(BUFFER_ALTERNATIV_OFF);

    // Restauram setarile initiale ale terminalului.
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosVechi_);

    initializat_ = false;
    g_ecranCurent = nullptr;
}

void Ecran::laSemnal(int /*semnal*/) {
    if (g_ecranCurent != nullptr) {
        g_ecranCurent->restaureaza();
    }
    std::exit(0);
}

void Ecran::seteazaFundal(const std::vector<std::string>& harta) {
    fundal_ = harta;
    // Ne asiguram ca fiecare linie are exact nrColoane_ caractere (completam cu spatii).
    for (auto& linie : fundal_) {
        if (static_cast<int>(linie.size()) < nrColoane_) {
            linie.resize(nrColoane_, ' ');
        }
    }
}

void Ecran::deseneazaFundal() {
    buffer_.clear();
    buffer_ += ACASA;
    for (int l = 0; l < static_cast<int>(fundal_.size()) && l < nrLinii_; ++l) {
        // Pozitionam la inceputul liniei (ANSI este 1-based).
        buffer_ += "\033[" + std::to_string(l + 1) + ";1H";
        buffer_ += fundal_[l].substr(0, nrColoane_);
    }
    actualizeaza();
}

bool Ecran::inInterior(int linie, int coloana) const {
    return linie >= 0 && linie < nrLinii_ && coloana >= 0 && coloana < nrColoane_;
}

char Ecran::fundalLa(int linie, int coloana) const {
    if (linie < 0 || linie >= static_cast<int>(fundal_.size())) return ' ';
    if (coloana < 0 || coloana >= static_cast<int>(fundal_[linie].size())) return ' ';
    return fundal_[linie][coloana];
}

void Ecran::scrieLa(int linie, int coloana, char ch) {
    if (!inInterior(linie, coloana)) return;
    // ANSI: \033[<linie>;<coloana>H, ambele 1-based.
    buffer_ += "\033[" + std::to_string(linie + 1) + ";" +
               std::to_string(coloana + 1) + "H";
    buffer_ += ch;
}

void Ecran::scrieLa(int linie, int coloana, const std::string& text) {
    if (linie < 0 || linie >= nrLinii_) return;
    buffer_ += "\033[" + std::to_string(linie + 1) + ";" +
               std::to_string(coloana + 1) + "H";
    buffer_ += text;
}

void Ecran::stergeLa(int linie, int coloana) {
    scrieLa(linie, coloana, fundalLa(linie, coloana));
}

void Ecran::actualizeaza() {
    if (buffer_.empty()) return;
    scrieDirect(buffer_);
    buffer_.clear();
}
