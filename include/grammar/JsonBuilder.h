//
// Created by nrx on 16.04.2020.
//

#ifndef COMPILERS_LAB2_JSONBUILDER_H
#define COMPILERS_LAB2_JSONBUILDER_H
#ifdef USE_JSON_BUILDER
#include "grammar/Grammar.h"
#include <string>

class JsonBuilder : public Grammar::Builder {
public:
    JsonBuilder(std::string json);
    Grammar build();
private:
    std::string json;
};
#endif
#endif //COMPILERS_LAB2_JSONBUILDER_H
