//
// Created by nrx on 15.04.2020.
//

#include "grammar/Grammar.h"
#include "grammar/GrammarException.h"

Grammar::Grammar() : _axiom(EMPTY_SYMBOL), _epsilon(EMPTY_SYMBOL)
{}

Grammar::Grammar(const Grammar &g) = default;

Grammar::Grammar(Grammar &&g) = default;

const std::vector<Production>& Grammar::productions() const {
    return _productions;
}

const std::unordered_set<Symbol>& Grammar::terminals() const {
    return _terminals;
}

const std::unordered_set<Symbol>& Grammar::nonTerminals() const {
    return _nonTerminals;
}

const Symbol& Grammar::axiom() const {
    return _axiom;
}

const Symbol& Grammar::epsilon() const {
    return _epsilon;
}

std::vector<Production> Grammar::findProductions(const ProductionPart &left) const {
    std::vector<Production> res;
    for(const Production &el : _productions) {
        if(el.left() == left) {
            res.push_back(el);
        }
    }
    return res;
}

bool Grammar::isNonTerminal(const Symbol &s) const {
    for(const Symbol &el : _nonTerminals) {
        if(s == el) {
            return true;
        }

    }
    return false;
}

bool Grammar::isTerminal(const Symbol &s) const {
    for(const Symbol &el : _terminals) {
        if(s == el) {
            return true;
        }
    }
    return false;
}

bool Grammar::operator==(const Grammar &g) const {
    struct less_production {
        bool operator()(const Production &f, const Production &s) {
            return h(f) < h(s);
        }
    private:
        std::hash<Production> h;
    };

    if((_axiom == g._axiom) && (_nonTerminals == g._nonTerminals) && (_terminals == g._terminals)) {
        std::vector<Production> p1 = _productions;
        std::vector<Production> p2 = g._productions;
        std::sort(p1.begin(), p1.end(), less_production());
        std::sort(p2.begin(), p2.end(), less_production());
        return p1 == p2;
    } else {
        return false;
    }
}

Grammar::Builder::Builder() {
    g = new Grammar();
}

Grammar::Builder::~Builder() {
    if(g != nullptr) {
        delete g;
    }
}

void Grammar::Builder::addProduction(ProductionPart left, ProductionPart right) {
    g->_productions.emplace_back(std::move(left), std::move(right));
}

void Grammar::Builder::addTerminal(Symbol s) {
    g->_terminals.insert(std::move(s));
}

void Grammar::Builder::addNonTerminal(Symbol s) {
    g->_nonTerminals.insert(std::move(s));
}

void Grammar::Builder::setAxiom(Symbol s) {
    g->_axiom = std::move(s);
    g->_nonTerminals.insert(g->_axiom);
}

void Grammar::Builder::setEpsilon(Symbol s) {
    g->_epsilon = std::move(s);
}

void Grammar::Builder::addProduction(Production p) {
    g->_productions.push_back(std::move(p));
}

Grammar Grammar::Builder::build() {
    if(g->_axiom == EMPTY_SYMBOL) {
        throw GrammarException("Axiom of Grammar don't set");
    }
    for(Symbol el : g->_terminals) {
        if(g->isNonTerminal(el)) {
            throw GrammarException("Symbol cannot belong a two alphabets of grammar");
        }
    }

    for(Production p : g->_productions) {
        for(Symbol s : p.left()) {
            if(!(g->isTerminal(s) || g->isNonTerminal(s))) {
                throw GrammarException("Production contain symbol that don't belong alphabet of grammar");
            }
        }
        for(Symbol s : p.right()) {
            if(!(g->isTerminal(s) || g->isNonTerminal(s))) {
                throw GrammarException("Production contain symbol that don't belong alphabet of grammar");
            }
        }
    }
    return *g;
}


std::ostream& operator<<(std::ostream &stream, const Grammar &g) {
    stream << "Axiom: " << g.axiom() << "\n";
    stream << "Epsilon: " << g.epsilon() << "\n";
    stream << "Productions:\n";
    for(const Production &p : g.productions()) {
        stream << "-- " << p << "\n";
    }
    return stream;
}