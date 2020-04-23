//
// Created by nrx on 15.04.2020.
//

#include "grammar/GrammarException.h"

GrammarException::GrammarException(const std::string &msg) : std::logic_error(msg)
{}

GrammarException::GrammarException(const GrammarException &exp) : std::logic_error(*this)
{}