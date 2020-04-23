//
// Created by nrx on 15.04.2020.
//

#ifndef COMPILERS_LAB2_PRODUCTIONPART_H
#define COMPILERS_LAB2_PRODUCTIONPART_H
#include "grammar/Symbol.h"
#include "grammar/Option.h"

class ProductionPart {
public:

    using iterator = std::vector<Symbol>::iterator;
    using const_iterator = std::vector<Symbol>::const_iterator;

    ProductionPart();

    ProductionPart(std::vector<Symbol> symbols);
    ProductionPart(const ProductionPart &part);
    ProductionPart(ProductionPart &&part);

    void addSymbol(const Symbol &s);
    Option<size_t> isContain(const Symbol &s) const;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    size_t size() const;
    const Symbol& get(size_t index) const;
    Symbol& get(size_t index);
    const std::vector<Symbol>& get() const;
    std::vector<Symbol>& get();
    bool operator==(const ProductionPart &part) const;
    bool operator!=(const ProductionPart &part) const;

    ProductionPart& operator=(const ProductionPart &p);
    ProductionPart& operator=(ProductionPart &p);

private:
    std::vector<Symbol> data;
};

namespace std {
    template<>
    struct hash <ProductionPart> {
        size_t operator()(const ProductionPart &pp) {
            hash<Symbol> h;
            size_t res = 0;
            for(const Symbol &s : pp){
                res += h(s);
            }
            return res;
        }
    };

}

#endif //COMPILERS_LAB2_PRODUCTIONPART_H
