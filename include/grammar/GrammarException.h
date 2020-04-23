//
// Created by nrx on 15.04.2020.
//

#ifndef COMPILERS_LAB2_GRAMMAREXCEPTION_H
#define COMPILERS_LAB2_GRAMMAREXCEPTION_H

#include <stdexcept>
#include <string>

class GrammarException : public std::logic_error {
public:
    GrammarException(const std::string &msg);
    GrammarException(const GrammarException &exp);
};
#endif //COMPILERS_LAB2_GRAMMAREXCEPTION_H
