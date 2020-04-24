//
// Created by nrx on 15.04.2020.
//
#include "grammar/Symbol.h"

Symbol::Block::Block(std::string &&n, std::string &&p) : name(n), pattern(p)
{}

Symbol::Block::Block(const std::string &n, const std::string &p) : name(n), pattern(p)
{}


Symbol::Symbol(const std::string &name,const std::string &pattern) {
    data = std::make_shared<Block>(name, pattern);
}


Symbol::Symbol(const std::string &np) {
    data = std::make_shared<Block>(np, np);
}

Symbol::Symbol(std::string &&name, std::string &&pattern) {
    data = std::make_shared<Block>(std::move(name), std::move(pattern));
}

Symbol::Symbol(const char name[], const char pattern[]) {
    data = std::make_shared<Block>(std::string(name), std::string(pattern));
}

Symbol::Symbol(const Symbol &s) : data(s.data)
{}


Symbol::Symbol(Symbol &&s) : data(std::move(s.data))
{}

Symbol Symbol::copy() const {
    return Symbol(name(), pattern());
}

bool Symbol::isSame(const Symbol &s) const {
    return data == s.data;
}

const std::string& Symbol::name() const {
    return data->name;
}

const std::string& Symbol::pattern() const {
    return data->pattern;
}

bool Symbol::operator==(const Symbol &s) const {
    return (data == s.data) || ((data->name == s.data->name) && (data->pattern == s.data->pattern));
}

bool Symbol::operator!=(const Symbol &s) const {
    return !(*this == s);
}

Symbol&  Symbol::operator=(const Symbol &s) {
    data = s.data;
}

Symbol&  Symbol::operator=(Symbol &&s) {
    data = std::move(s.data);
}


size_t std::hash<Symbol>::operator()(const Symbol &s) const {
    hash<std::string> h;
    return h(s.pattern())+h(s.name());
}


std::ostream& operator<<(std::ostream& stream, const Symbol &s) {
    stream << "<" << s.name() << ">";
    return stream;
}