//
// Created by nrx on 15.04.2020.
//

#ifndef COMPILERS_LAB2_GRAMMAR_H
#define COMPILERS_LAB2_GRAMMAR_H

#include "grammar/Production.h"
#include "grammar/Symbol.h"
#include <unordered_set>
#include <vector>


class Grammar {
public:
    class Builder {
    public:
        Builder();
        ~Builder();
        virtual void addProduction(ProductionPart left, ProductionPart right);
        virtual void addProduction(Production p);
        virtual void addTerminal(Symbol s);
//        template <typename Itr>
//        void addTerminals();
        virtual void addNonTerminal(Symbol s);
        virtual void setAxiom(Symbol s);
        virtual void setEpsilon(Symbol s);
        virtual Grammar build();

    protected:
        Grammar *g;
    };
    inline static const Symbol EMPTY_SYMBOL = Symbol("", "");
    Grammar();
    Grammar(const Grammar &g);
    Grammar(Grammar &&g);

    bool isTerminal(const Symbol &s) const;
    bool isNonTerminal(const Symbol &s) const;
    const std::vector<Production>& productions() const;
    const std::unordered_set<Symbol>& terminals() const;
    const std::unordered_set<Symbol>& nonTerminals() const;
    const Symbol& axiom() const;
    const Symbol& epsilon() const;
    std::vector<Production> findProductions(const ProductionPart &left) const;

    bool operator==(const Grammar& g) const;

private:
    std::vector<Production> _productions;
    std::unordered_set<Symbol> _terminals;
    std::unordered_set<Symbol> _nonTerminals;
    Symbol _axiom;
    Symbol _epsilon;
};


#endif //COMPILERS_LAB2_GRAMMAR_H
