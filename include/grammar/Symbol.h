//
// Created by nrx on 15.04.2020.
//

#ifndef COMPILERS_LAB2_SYMBOL_H
#define COMPILERS_LAB2_SYMBOL_H

#include <functional>
#include <memory>

class Symbol {
public:

    Symbol(const std::string &name,const std::string &pattern);
    Symbol(std::string &&name, std::string &&pattern);
    Symbol(const char name[], const char pattern[]);
    Symbol(const Symbol &s);
    Symbol(Symbol &&s);

    Symbol copy() const;
    bool isSame(const Symbol &s) const;

    const std::string& name() const;
    const std::string& pattern() const;
    Symbol& operator=(const Symbol &s);
    Symbol& operator=(Symbol &&s);


    bool operator==(const Symbol &s) const;
    bool operator!=(const Symbol &s) const;
private:
    struct Block{
        Block(const std::string &n, const std::string &p);
        Block(std::string &&n, std::string &&p);
        std::string name;
        std::string pattern;
    };
    std::shared_ptr<Block> data;
};

namespace std {
    template<>
    struct hash<Symbol> {
        size_t operator() (const Symbol &s) const;
    };
}

#endif //COMPILERS_LAB2_SYMBOL_H
