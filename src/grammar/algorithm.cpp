//
// Created by nrx on 16.04.2020.
//

#include "grammar/algorithm.h"
#include "grammar/GrammarException.h"
#include <list>
bool contain( const std::unordered_set<Symbol> &set, const Symbol &s) {
    return set.find(s) != set.end();
}

std::unordered_set<Symbol> algorithm::removeBarrenSymbols(const Grammar &g) {
    std::unordered_set<Symbol> oldSet;
    std::unordered_set<Symbol> newSet;
    do{
        for(Symbol el : newSet) {
            oldSet.insert(el);
        }
        newSet.clear();
        for(Production p : g.productions()) {
            bool allRightBelong = true;
            for(const Symbol &s : p.right()) {
                if(!(contain(oldSet, s) || contain(g.terminals(), s))) {
                    allRightBelong = false;
                    break;
                }
            }
            if(allRightBelong) {
                newSet.insert(*p.left().begin());
            }
        }
        for(Symbol st : oldSet) {
            newSet.insert(st);
        }
    } while(oldSet.size() != newSet.size());
    return newSet;
}

std::unordered_set<Symbol> algorithm::removeUnreachableSymbols(const Grammar &g) {
    std::unordered_set<Symbol> oldSet;
    std::unordered_set<Symbol> newSet;
    newSet.insert(g.axiom());
    do{
        oldSet = newSet;
        newSet.clear();
        for(const Symbol& A : oldSet) {
            for(const Production &p : g.productions()) {
                if(p.left().isContain(A)) {
                    for(const Symbol &X : p.right()) {
                           newSet.insert(X);
                    }
                }
            }
        }
        for(const Symbol &s : oldSet) {
            newSet.insert(s);
        }
    } while(oldSet.size() != newSet.size());

    return newSet;
}

Grammar algorithm::deleteDummySymbols(const Grammar &g) {
    std::unordered_set<Symbol> withoutBarren = removeBarrenSymbols(g);
    Grammar::Builder b;
    b.setAxiom(g.axiom());
    for(const Symbol &s : withoutBarren) {
        b.addNonTerminal(s);
    }
    for(const Symbol &s: g.terminals()) {
        b.addTerminal(s);
    }

    for(const Production &p : g.productions()) {
        bool prodWithoutBarren = true;
        for(Symbol A : p.left()) {
            if (!contain(withoutBarren, A)) {
                prodWithoutBarren = false;
                break;
            }
        }
        if(!prodWithoutBarren) {
            continue;
        }
        for(Symbol A : p.right()) {
            if ( g.isNonTerminal(A) && !contain(withoutBarren, A)) {
                prodWithoutBarren = false;
                break;
            }
        }
        if(!prodWithoutBarren) {
            continue;
        }
        b.addProduction(p);
    }
    Grammar g1 = b.build();

    std::unordered_set<Symbol> withoutUnreachable = removeUnreachableSymbols(g1);

    Grammar::Builder b2;
    b2.setAxiom(g1.axiom());
    for(const Symbol &s : withoutUnreachable) {
     if(g1.isTerminal(s)) {
         b2.addTerminal(s);
     } else {
         b2.addNonTerminal(s);
     }

    }
    for(const Production &p : g1.productions()) {
        bool consistFromReachable = true;
        for(const Symbol &s: p.left()) {
            if(!contain(withoutUnreachable, s)) {
                consistFromReachable = false;
                break;
            }
        }
        if(!consistFromReachable) {
            continue;
        }
        for(const Symbol &s: p.right()) {
            if(!contain(withoutUnreachable, s)) {
                consistFromReachable = false;
                break;
            }
        }
        if(!consistFromReachable) {
            continue;
        }
        b2.addProduction(p);
    }
    return b2.build();
}

std::vector<Production> algorithm::removeDirectRecursion(const std::vector<Production> &prods, const Symbol &epsilon, const Symbol &target) {
    std::vector<Production> res_p;
    std::unordered_set<Symbol> processedNonTerminal;
    for(Production p : prods) {
        Symbol left = *p.left().begin();//current nonTerminal
//        if(contain(processedNonTerminal, left)) {//if this terminal was be processing
//            continue;
//        }

        std::vector<size_t> recursiveProd;//indexes of recursive rules;
        std::vector<size_t> nonRecursiveProd;//indexes of non recursive rules;
        //find all rules with left part: "left"
        for(size_t i = 0; i < prods.size(); i++) {
            if(*prods[i].left().begin() == left) {
                if(*prods[i].right().begin() == left) {//this rule is recursive
                    recursiveProd.push_back(i);
                } else { //this rule note recursive
                    nonRecursiveProd.push_back(i);
                }
            }
        }
        if(left != target) {
            //if this rule don't processed
            for(size_t nri : recursiveProd) {
                res_p.push_back(prods[nri]);
            }
            for(size_t nri : nonRecursiveProd) {
                res_p.push_back(prods[nri]);
            }
            continue;
        }
        if(recursiveProd.empty()) {
            //not found recursive rule for left
            //copy non recursive rule without changes
            for(size_t nri : nonRecursiveProd) {
                res_p.push_back(prods[nri]);
            }
            continue;
        }
        Symbol stroke(left.name()+"'", left.pattern()+"'");
        //create new rules from recursive productions
        for(size_t ri : recursiveProd) {
            std::vector<Symbol> leftPartRule;
            std::vector<Symbol> rightPartRule;
            leftPartRule.push_back(stroke);
            for(size_t indexPR = 1; indexPR < prods[ri].right().size(); indexPR++) {
                rightPartRule.push_back(prods[ri].right().get(indexPR));
            }
            rightPartRule.push_back(stroke);
            res_p.emplace_back(leftPartRule, rightPartRule);
        }
        //add epsilon-rule
        res_p.emplace_back(std::vector{stroke}, std::vector{epsilon});
        //create new rules from nonRecursive productions
        for(size_t nri : nonRecursiveProd) {
            std::vector<Symbol> leftPartRule;
            std::vector<Symbol> rightPartRule;
            leftPartRule.push_back(left);
            for(const Symbol &s : prods[nri].right()) {
                rightPartRule.push_back(s);
            }
            rightPartRule.push_back(stroke);
            res_p.emplace_back(leftPartRule, rightPartRule);
        }

        processedNonTerminal.insert(left);
    }
    return res_p;
}
//std::vector<size_t> findRulesByLeftCFGrammar(const std::vector<Production> &prod, const Symbol &left) {
//    std::vector<size_t> res;
//    for(size_t i = 0; i < prod.size(); i++) {
//        if(left == *prod[i].left().begin()) {
//            res.push_back(i);
//        }
//    }
//    return res;
//}
//
//Option<size_t> findSymbol(const std::vector<Symbol> &c, const Symbol &s) {
//    for(size_t i = 0; i < c.size(); i++) {
//        if(c[i] == s) {
//            return Option<size_t>(i);
//        }
//    }
//    return Option<size_t>();
//}
//
//Grammar changeRuleByOrderNonTerminals(const Grammar &g) {
//    std::vector<Symbol> nonTerminals;
//    for(const Symbol &s : g.nonTerminals()) {
//        nonTerminals.push_back(s);
//    }
//    for(size_t i = 0; i < nonTerminals.size(); i++) {
//        auto irules = findRulesByLeftCFGrammar(g.productions(), nonTerminals[i]);
//        //find rule whith right rulu which contain a first nonTerminal with less index the current element
//        std::vector<size_t> willBeChangeRules;
//        for(size_t ir : irules) {
//            Option<size_t> r = findSymbol(nonTerminals, g.productions()[i].right().get(0));
//            if(r) {
//                if(*r.result() <= i) {
//                    willBeChangeRules.push_back(ir);
//                }
//            } else {
//                throw GrammarException("Invalid grammar: Grammar have production witch contain not exist Symbol");
//            }
//        }
//
//
//    }
//}

std::vector<std::list<Production>::iterator> findProductionsByLeft(Symbol left, std::list<Production> prods) {
    std::vector<std::list<Production>::iterator> res;
    for(std::list<Production>::iterator itr = prods.begin(); itr != prods.end(); ++itr) {
        if(left == itr->left().get(0)) {
            res.push_back(itr);
        }
    }
    return res;
}

Option<size_t> findSymbol(const std::vector<Symbol> &c, const Symbol &s) {
    for(size_t i = 0; i < c.size(); i++) {
        if(c[i] == s) {
            return Option<size_t>(i);
        }
    }
    return Option<size_t>();
}


Production substitutionFirstSymbolInRight(const Production &main, const Production &sub) {
    std::vector<Symbol> rightPart = sub.right().get();
    for(size_t i = 1; i < main.right().size(); i++) {
        rightPart.push_back(main.right().get(i));
    }
    return Production(main.left(), ProductionPart(rightPart));
}

Grammar algorithm::deleteRecursion(const Grammar &g) {
    using lItr = std::list<Production>::iterator;
    std::vector<Symbol> nonTerminals;
    std::list<Production> productions;
    for (const Symbol &s : g.terminals()) {
        nonTerminals.push_back(s);
    }
    for (const Production &p : g.productions()) {
        productions.push_back(p);
    }
    //Iterable on Ai
    for (size_t i = 0; i < nonTerminals.size(); i++) {
        std::vector<lItr> rules = findProductionsByLeft(nonTerminals[i], productions);
        //Iterable on Aj
        std::vector<lItr> mustRemoveRule;
        for (size_t j = 0; j < i; j++) {
            //Iterable on all rules type: Ai -> *
            for (lItr p : rules) {
                //detect rule type: Ai -> Aj g
                if (p->right().get(0) == nonTerminals[j]) {
                    std::vector<lItr> subRules = findProductionsByLeft(nonTerminals[j], productions);
                    for(const lItr &sub : subRules) {
                        //create new rule from current rule(p): apply substitution
                        Production newProd = substitutionFirstSymbolInRight(*p, *sub);
                        productions.push_back(newProd);
                    }
                   mustRemoveRule.push_back(p);
                }
            }
        }
        for(lItr &p : mustRemoveRule) {
            productions.erase(p);
        }
        productions = removeDirectRecursion(productions, g.epsilon(), nonTerminals[i]);
    }
}
