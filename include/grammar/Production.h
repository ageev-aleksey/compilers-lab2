//
// Created by nrx on 15.04.2020.
//

#ifndef COMPILERS_LAB2_PRODUCTION_H
#define COMPILERS_LAB2_PRODUCTION_H

#include "grammar/Symbol.h"
#include "grammar/ProductionPart.h"
#include <vector>

class Production {
public:
    Production(const std::vector<Symbol> &left, const std::vector<Symbol> &right);
    Production(std::vector<Symbol> &&left, std::vector<Symbol> &&right);
    Production(const ProductionPart &left,const ProductionPart &right);
    Production(ProductionPart &&left, ProductionPart &&right);
    Production(const Production &p);
    Production(Production &&p);

    Production& operator=(Production &p);
    Production& operator=(const Production &p);

    const ProductionPart& left() const;
    ProductionPart& left();
    const ProductionPart& right() const;
    ProductionPart& right();
    bool operator==(const Production &p) const;
    bool operator!=(const Production &p) const;

private:
    ProductionPart _left;
    ProductionPart _right;
};

namespace std {
    template<>
    struct hash <Production> {
        size_t operator()(const Production& p) const;
    };
}

std::ostream& operator<<(std::ostream &stream, const Production &part);

#endif //COMPILERS_LAB2_PRODUCTION_H
