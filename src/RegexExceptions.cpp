//
// Created by nrx on 21.03.2020.
//

#include "regex/RegexExceptions.h"

GraphException::GraphException(const std::string &msg) : std::logic_error(msg) {

}

InvalidVertexException::InvalidVertexException(const std::string &msg) : GraphException(msg) {

}