//
// Created by nrx on 16.04.2020.
//

#include "grammar/algorithm.h"
#include "grammar/GrammarException.h"
#include <list>
#include <iostream>
#include "Graph.h"
#include <stack>
#include <GraphToDOT.h>

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

void algorithm::removeDirectRecursion(const std::unordered_set<Symbol> &nonTerminals, std::list<Production> &prods, const Symbol &epsilon, const Symbol &target) {
    using lItr = std::list<Production>::iterator;
    using clItr = std::list<Production>::const_iterator;
    //std::list<Production> res_p;
    std::unordered_set<Symbol> processedNonTerminal;
    for(Symbol left : nonTerminals) {
        //Symbol left = *p.left().begin();//current nonTerminal
//        if(contain(processedNonTerminal, left)) {//if this terminal was be processing
//            continue;
//        }

        std::vector<clItr> recursiveProd;//indexes of recursive rules;
        std::vector<clItr> nonRecursiveProd;//indexes of non recursive rules;
        //find all rules with left part: "left"
        for(clItr p = prods.begin(); p != prods.end(); ++p) {
            if(p->left().get(0) == left) {
                if(p->right().get(0) == left) {//this rule is recursive
                    recursiveProd.push_back(p);
                } else { //this rule note recursive
                    nonRecursiveProd.push_back(p);
                }
            }
        }
        if(left != target) {
            //if this rule don't processed
//            for(clItr &nri : recursiveProd) {
//                res_p.push_back(*nri);
//            }
//            for(clItr &nri : nonRecursiveProd) {
//                res_p.push_back(*nri);
//            }
            continue;
        }
        if(recursiveProd.empty()) {
            //not found recursive rule for left
            //copy non recursive rule without changes
//            for(clItr &nri : nonRecursiveProd) {
//                res_p.push_back(*nri);
//            }
            continue;
        }
        Symbol stroke(left.name()+"'", left.pattern()+"'");
        //create new rules from recursive productions
        std::cout << " > detected direct recursion: \n";
        for(clItr &ri : recursiveProd) {
            std::cout << "  - " << *ri << "\n";
        }
        for(clItr &ri : recursiveProd) {
            std::vector<Symbol> leftPartRule;
            std::vector<Symbol> rightPartRule;
            leftPartRule.push_back(stroke);
            for(size_t indexPR = 1; indexPR < ri->right().size(); indexPR++) {
                rightPartRule.push_back(ri->right().get(indexPR));
            }
            rightPartRule.push_back(stroke);
            //res_p.emplace_back(leftPartRule, rightPartRule);
            std::cout << "   - created new production: " << Production(leftPartRule, rightPartRule) << std::endl;
            prods.emplace_back(leftPartRule, rightPartRule);
        }
        //add epsilon-rule
        //res_p.emplace_back(std::vector{stroke}, std::vector{epsilon});
        prods.emplace_back(std::vector{stroke}, std::vector{epsilon});
        //create new rules from nonRecursive productions
        for(clItr &nri : nonRecursiveProd) {
            std::vector<Symbol> leftPartRule;
            std::vector<Symbol> rightPartRule;
            leftPartRule.push_back(left);
            for(const Symbol &s : nri->right()) {
                rightPartRule.push_back(s);
            }
            rightPartRule.push_back(stroke);
            std::cout << "   - created new production: " << Production(leftPartRule, rightPartRule) << std::endl;
            //res_p.emplace_back(leftPartRule, rightPartRule);
            prods.emplace_back(leftPartRule, rightPartRule);
        }
        //delete all old productions
        for(clItr &p : recursiveProd) {
            prods.erase(p);
        }
        for(clItr &p : nonRecursiveProd) {
            prods.erase(p);
        }
        processedNonTerminal.insert(left);
    }
    //return res_p;
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

std::vector<std::list<Production>::iterator> findProductionsByLeft(const Symbol &left, std::list<Production> &prods) {
    std::vector<std::list<Production>::iterator> res;
    for(auto itr = prods.begin(); itr != prods.end(); ++itr) {
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
    nonTerminals.push_back(g.axiom());
    std::list<Production> productions;
    for (const Symbol &s : g.nonTerminals()) {
        if(s == g.axiom()) continue;
        nonTerminals.push_back(s);
    }
    for (const Production &p : g.productions()) {
        productions.push_back(p);
    }
    //Iterable on Ai
    for (size_t i = 0; i < nonTerminals.size(); i++) {
        std::cout << "(i=" << i << ")" << std::endl;
        std::cout << "-----\n" << "CurrentNonTerminal: " << nonTerminals[i] << std::endl;

        std::vector<lItr> rules = findProductionsByLeft(nonTerminals[i], productions);
        std::cout << "-Rules: \n";
        for(lItr &p : rules) {
            std::cout << "   --" << *p << std::endl;
        }
        //Iterable on Aj
        std::vector<lItr> mustRemoveRule;
        for (size_t j = 0; j < i; j++) {
            std::cout << "(j=" << j << ")" << std::endl;
            //Iterable on all rules type: Ai -> *
            for (lItr &p : rules) {
                //detect rule type: Ai -> Aj g
                if (p->right().get(0) == nonTerminals[j]) {
                    std::cout << "  -- Processing Rule: " <<  *p << std::endl;
                    std::vector<lItr> subRules = findProductionsByLeft(nonTerminals[j], productions);
                    std::cout << "   --- substitution rules: \n";
                    for(lItr &p : subRules) {
                        std::cout << "    ---- " << *p << std::endl;
                    }
                    for(const lItr &sub : subRules) {
                        //create new rule from current rule(p): apply substitution
                        Production newProd = substitutionFirstSymbolInRight(*p, *sub);
                        std::cout << "   --- Created new Rule: " << newProd << std::endl;
                        productions.push_back(newProd);
                    }
                   mustRemoveRule.push_back(p);
                }
            }
        }
        for(lItr &p : mustRemoveRule) {
            productions.erase(p);
        }
       removeDirectRecursion(g.nonTerminals(), productions, g.epsilon(), nonTerminals[i]);
    }

    //build new equivalent grammar;
    Grammar::Builder b;
    b.setAxiom(g.axiom());
    b.setEpsilon(g.epsilon());
    for(const Production &p : productions) {
        b.addNonTerminal(p.left().get(0));
        b.addProduction(p);
    }
    for(const Symbol &s : g.terminals()) {
        b.addTerminal(s);
    }
    return b.build();
}


Graph<Empty, Symbol> algorithm::buildTreeOfSymbols(const std::vector<std::vector<Symbol>> &sArrays) {
    using GraphS = Graph<Empty, Symbol>;
    GraphS graph;
    GraphS::iterator root = graph.addNodeInBack(Empty{});
    for(const std::vector<Symbol> &array : sArrays) {
        GraphS::iterator pos = root;
        for(const Symbol &s : array) {
            bool isLinkFound = false;
            for(const GraphS::Link &l : pos.getLinks()) {
                if(l.data == s) {
                    isLinkFound = true;
                    pos = l.node;
                    break;
                }
            }
            if(isLinkFound) {
                continue;
            } else {
                GraphS::iterator newPosition = graph.addNodeInBack(Empty{});
                graph.addLink(pos, newPosition, s);
                pos = newPosition;
                continue;
            }
        }
        GraphS::iterator newPosition = graph.addNodeInBack(Empty{});
        graph.addLink(pos, newPosition, {""});
    }
    return graph;
}



//TODO make tests
Graph<Empty, Symbol>::iterator algorithm::findDeepestForkInTreeSymbols(Graph<Empty, Symbol> &graph) {
    using Graph_t = Graph<Empty, Symbol>;
    using NodeItr = Graph<Empty, Symbol>::iterator;
    std::stack<std::pair<NodeItr, size_t>> stack;
    std::list<std::pair<NodeItr, size_t>> forks;
    stack.push({graph.firstNode(), 0});
    while(!stack.empty()) {
        std::pair<NodeItr, size_t> p = stack.top();
        stack.pop();
        if(p.first.getLinks().size() > 1) {
            forks.push_back(p);
        }
        for(const Graph_t::Link &link : p.first.getLinks()) {
            stack.push({link.node, p.second+1});
        }
    }
    size_t maxDeep = 0;
    NodeItr itr = graph.end();
    for(std::pair<NodeItr, size_t> &f : forks) {
        if(f.second >= maxDeep) {
            maxDeep = f.second;
            itr = f.first;
        }
    }
    return itr;
}
//TODO make test
std::vector<Graph<Empty, Symbol>::iterator> algorithm::buildParenTableOfTree(Graph<Empty, Symbol> &graph) {
    std::vector<Graph<Empty, Symbol>::iterator> res(graph.getNodes().size(), graph.end());
    for(auto itr = graph.begin(); itr != graph.end(); itr++) {
        if(!itr.getLinks().empty()) {
            for(auto &link: itr.getLinks()) {
                res[link.node.getIndex()] = itr;
            }
        }
    }
    return res;
}


std::list<std::vector<Production>::const_iterator> findProductionsByLeft(const Symbol &s, const std::vector<Production> &prods) {
    std::list<std::vector<Production>::const_iterator> res;
    for(auto itr = prods.begin(); itr != prods.end(); itr++) {
        if(itr->left().get(0) == s) {
            res.push_back(itr);
        }
    }
    return res;
}

bool prefixCheck(const Production &p, const std::vector<Symbol> &reversPrefix) {
    if(p.right().size() >= reversPrefix.size()) {
        auto itr = p.right().begin();
        for(auto pItr = reversPrefix.crbegin(); pItr != reversPrefix.crend(); pItr++) {
            if(*itr != *pItr) {
                return false;
            }
            itr++;
        }
        return true;
    } else {
        return false;
    }
}
//TODO function generate new Symbol - необходима специальная функция, поскольку создавая сивол без проверки, можно наткнуться на то, тчо созданный символ на самом деле уже используется.

Symbol generateSymbol(const std::unordered_set<Symbol> &set, const Symbol &s) {
    std::string symbol = s.name();
    symbol.push_back('\'');
    Symbol newS = Symbol(symbol);
    while(contain(set, newS)) {
        symbol.push_back('\'');
        newS = Symbol(symbol);
    }
    return newS;
}



Grammar algorithm::leftFactoring(const Grammar &g) {
    std::list<Production> newRules;
    std::unordered_set<Symbol> allSymbols;
    for(const Production &p : g.productions()) {
        newRules.push_back(p);
    }
    for(const Symbol &p : g.terminals()) {
        allSymbols.insert(p);
    }
    for(const Symbol &p : g.nonTerminals()) {
        allSymbols.insert(p);
    }


    for(const Symbol &s : g.nonTerminals()) {
        //TODO WHILE до тех пор пытаться выделить прфеиксы, пока они не перестанут выделятся, при каждом измении правил
        while (true) {
            std::list<std::list<Production>::iterator> changeProduction;
            auto p = findProductionsByLeft(s, newRules);
            size_t n = p.size();
            if (n == 1) {
                break;
            }
            std::vector<std::vector<Symbol>> rightParts(n);
            auto pItr = p.begin();
            for (size_t i = 0; i < n; i++) {
                auto &cp = *pItr;
                //rightParts[i].resize(cp->right().size());
                std::copy(cp->right().begin(), cp->right().end(), std::back_inserter(rightParts[i]));
                pItr++;
            }
            Graph<Empty, Symbol> graph = buildTreeOfSymbols(rightParts);
            //std::cout << graphToDOT(graph);
            Graph<Empty, Symbol>::iterator deepestFork = findDeepestForkInTreeSymbols(graph);

            if (deepestFork == graph.firstNode()) {
                break;
            }

            std::vector<Graph<Empty, Symbol>::iterator> parentTable = buildParenTableOfTree(graph);
            //build prefix
            Graph<Empty, Symbol>::iterator node = deepestFork;
            std::vector<Symbol> r_prefix;
            while (node != graph.firstNode()) {
                node = parentTable[node.getIndex()];
                r_prefix.push_back(node.getLinks().begin()->data);
            }
            //find rules this prefix
            for (auto &ruleItr : p) {
                if (prefixCheck(*ruleItr, r_prefix)) {
                    changeProduction.push_back(ruleItr);
                }
            }
            //generate new rules
            Symbol stroke = generateSymbol(allSymbols, s);
            allSymbols.insert(stroke);
            for (auto &ruleItr : changeProduction) {
                //generate rule type: A' -> B
                std::vector<Symbol> rightPartStrokeRule;
                std::copy(ruleItr->right().begin() + r_prefix.size(), ruleItr->right().end(),
                          std::back_inserter(rightPartStrokeRule));
                if (rightPartStrokeRule.empty()) {
                    rightPartStrokeRule.push_back(g.epsilon());
                }
                newRules.emplace_back(ProductionPart({stroke}), ProductionPart(rightPartStrokeRule));
                newRules.erase(ruleItr);
            }
//        std::vector<Symbol> rightPartWithPrefix(r_prefix.size());
            std::vector<Symbol> rightPartWithPrefix;
            std::copy(r_prefix.crbegin(), r_prefix.crend(), std::back_inserter(rightPartWithPrefix));
            rightPartWithPrefix.push_back(stroke);
            newRules.emplace_back(ProductionPart({s}), ProductionPart(rightPartWithPrefix));
        }
    }

    Grammar::Builder b;
    b.setEpsilon(g.epsilon());
    b.setAxiom(g.axiom());
    for(const Production &p : newRules) {
        b.addProduction(p);
        b.addNonTerminal(p.left().get(0));
    }
    for(const Symbol &s : g.terminals()) {
        b.addTerminal(s);
    }
    return b.build();
}