// =============================================================================
//  Sprite.cpp - definirea sprite-urilor ASCII.
// =============================================================================
#include "Sprite.h"

namespace sprite {

const std::vector<std::string>& masinaJucator() {
    static const std::vector<std::string> s = {
        "e@@e",
        " @@ ",
        "e@@e"
    };
    return s;
}

const std::vector<std::string>& masinaJucatorOrizontala() {
    static const std::vector<std::string> s = {
        "e  e",
        "@@@@",
        "e  e"
    };
    return s;
}

const std::vector<std::string>& masina() {
    static const std::vector<std::string> s = {
        "0XX0",
        " XX ",
        "0XX0"
    };
    return s;
}

const std::vector<std::string>& camion() {
    static const std::vector<std::string> s = {
        "0XX0",
        " XX ",
        " XX ",
        " XX ",
        "0XX0"
    };
    return s;
}

const std::vector<std::string>& motocicleta() {
    static const std::vector<std::string> s = {
        "o",
        "@",
        "o"
    };
    return s;
}

const std::vector<std::string>& masinaOrizontala() {
    static const std::vector<std::string> s = {
        " oo ",
        "[XX]",
        " oo "
    };
    return s;
}

const std::vector<std::string>& camionOrizontal() {
    static const std::vector<std::string> s = {
        " oooo ",
        "[XXXX]",
        " oooo "
    };
    return s;
}

const std::vector<std::string>& motocicletaOrizontala() {
    static const std::vector<std::string> s = {
        "o @ o"
    };
    return s;
}

} // namespace sprite
