//
// Created by nrx on 16.04.2020.
//

#ifndef LABA2_ALGORITHM_H
#define LABA2_ALGORITHM_H
#include "grammar/Symbol.h"
#include "grammar/Grammar.h"
#include <unordered_set>

namespace algorithm {
    std::unordered_set<Symbol> removeBarrenSymbols(const Grammar &g);
    std::unordered_set<Symbol> removeUnreachableSymbols(const Grammar &g);

    std::vector<Production> removeDirectRecursion(const std::vector<Production> &prods, const Symbol &epsilon, const Symbol &target);
    Grammar deleteRecursion(const Grammar &g);
    Grammar deleteDummySymbols(const Grammar &g);

}
#endif //LABA2_ALGORITHM_H
