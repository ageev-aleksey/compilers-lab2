//
// Created by nrx on 16.04.2020.
//

#include "grammar/GrammarException.h"
#include "grammar/ProductionPart.h"

#include <utility>

ProductionPart::ProductionPart(){}

ProductionPart::ProductionPart(std::vector<Symbol> symbols) : data(std::move(symbols))
{}

ProductionPart::ProductionPart(const ProductionPart &part) : data(part.data)
{}

ProductionPart::ProductionPart(ProductionPart &&part) : data(std::move(part.data))
{}

void ProductionPart::addSymbol(const Symbol &s) {
    data.push_back(s);
}

Option<size_t> ProductionPart::isContain(const Symbol &s) const {
    for(size_t i = 0; i < data.size(); i++) {
        if (data[i] == s) {
            return Option(i);
        }
    }
    return Option<size_t>();
}

ProductionPart::iterator  ProductionPart::begin() {
    return data.begin();
}

ProductionPart::iterator ProductionPart::end() {
    return data.end();
}

ProductionPart::const_iterator ProductionPart::begin() const {
    return data.cbegin();
}

ProductionPart::const_iterator ProductionPart::end() const {
    return data.cend();
}

size_t ProductionPart::size() const {
    return  data.size();
}

const Symbol& ProductionPart::get(size_t index) const {
    return data[index];
}

Symbol& ProductionPart::get(size_t index) {
    return data[index];
}

const std::vector<Symbol>& ProductionPart::get() const {
    return data;
}

std::vector<Symbol>& ProductionPart::get() {
    return data;
}

bool ProductionPart::operator==(const ProductionPart &part) const {
    if(data.size() == part.data.size()) {
        for(size_t i = 0; i < data.size(); i++) {
            if(data[i] != part.data[i]) {
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}

ProductionPart& ProductionPart::operator=(ProductionPart &p) {
    data = p.data;
    return *this;
}

ProductionPart& ProductionPart::operator=(const ProductionPart &p) {
    for(const Symbol &s : p) {
        data.push_back(s.copy());
    }
    return *this;
}

std::ostream& operator<<(std::ostream &stream, const ProductionPart &part) {
    for(const Symbol &s : part) {
        stream << s;
    }
    return stream;
}

