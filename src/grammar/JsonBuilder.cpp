//
// Created by nrx on 16.04.2020.
//
#ifdef USE_JSON_BUILDER

#include "grammar/JsonBuilder.h"


JsonBuilder::JsonBuilder(std::string json) : json(std::move(json))
{}

Grammar JsonBuilder::build() {
    Grammar g;
}

#endif