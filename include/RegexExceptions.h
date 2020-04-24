//
// Created by nrx on 21.03.2020.
//

#ifndef STATE_MACHINE_REGEXEXCEPTIONS_H
#define STATE_MACHINE_REGEXEXCEPTIONS_H
#include <stdexcept>

class GraphException : std::logic_error {
public:
    GraphException(const std::string &msg);
};

class InvalidVertexException : GraphException {
public:
    InvalidVertexException(const std::string &msg);
};

#endif //STATE_MACHINE_REGEXEXCEPTIONS_H
