//
// Created by nrx on 16.04.2020.
//
#include "grammar/Production.h"

Production::Production(const std::vector<Symbol> &left, const std::vector<Symbol> &right)
: _left(left), _right(right)
{}

Production::Production(std::vector<Symbol> &&left, std::vector<Symbol> &&right)
        : _left(std::move(left)), _right(std::move(right))
{}

Production::Production(const ProductionPart &left,const ProductionPart &right)
        : _left(std::move(left)), _right(std::move(right))
{}

Production::Production(ProductionPart &&left, ProductionPart &&right)
: _left(std::move(left)), _right(std::move(right))
{}

Production::Production(const Production &p)
: _left(p._left), _right(p._right)
{}

Production::Production(Production &&p)
: _left(std::move(p._left)), _right(std::move(p._right))
{}

const ProductionPart& Production::left() const {
    return _left;
}

ProductionPart& Production::left() {
    return _left;
}

const ProductionPart& Production::right() const {
    return _right;
}

ProductionPart& Production::right() {
    return _right;
}

bool Production::operator==(const Production &p) const {
    return (_left == p._left) && (_right == p._right);
}

bool Production::operator!=(const Production &p) const {
    return !(*this == p);
}

Production& Production::operator=(Production &p) {
    _left = p._left;
    _right = p._right;
    return *this;
}

Production& Production::operator=(const Production &p) {
    _left = p._left;
    _right = p._right;
    return *this;
}

size_t std::hash<Production>::operator()(const Production &p) const {
    hash<ProductionPart> h;
    return h(p.left()) + h(p.right());
}


std::ostream& operator<<(std::ostream &stream, const Production &p) {
    stream << p.left() << " --> " << p.right();
    return stream;
}