// =============================================================================
//  Logger.cpp - implementarea jurnalului de evenimente.
// =============================================================================
#include "Logger.h"

#include <ctime>

Logger::Logger(const std::string& caleFisier)
    : out_(caleFisier, std::ios::app) {
    if (out_.is_open()) {
        out_ << "----- sesiune noua (" << acum() << ") -----\n";
        out_.flush();
    }
}

Logger::~Logger() {
    if (out_.is_open()) {
        out_ << "----- sesiune incheiata (" << acum() << ") -----\n";
        out_.flush();
        out_.close();
    }
}

void Logger::eveniment(const std::string& mesaj) {
    if (!out_.is_open()) return;
    out_ << "[" << acum() << "] " << mesaj << "\n";
    out_.flush(); // scriem imediat, ca jurnalul sa fie complet chiar daca jocul iese brusc
}

std::string Logger::acum() {
    std::time_t t = std::time(nullptr);
    std::tm tm_buf{};
#if defined(_WIN32)
    localtime_s(&tm_buf, &t);
#else
    localtime_r(&t, &tm_buf);
#endif
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_buf);
    return std::string(buf);
}
