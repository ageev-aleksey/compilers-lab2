//
// Created by nrx on 16.04.2020.
//

#include "grammar/algorithm.h"
#include "gtest/gtest.h"
Grammar getGrammar1() {
    Grammar::Builder b;
    b.setAxiom({"axiom", "S"});
    b.addNonTerminal({"A", "A"});
    b.addNonTerminal({"B", "B"});
    b.addTerminal({"a", "a"});
    b.addTerminal({"b", "b"});
    b.addProduction({{ {"axiom", "S"} }}, {{ {"a", "a"} }});
    b.addProduction({{ {"axiom", "S"} }}, {{ {"A", "A"} }});
    b.addProduction({{ {"A", "A"} }}, {{ {"A", "A"}, {"B", "B"} }});
    b.addProduction({{ {"B", "B"} }}, {{ {"b", "b"} }});

    return b.build();
}

Grammar getGrammar2() {
    Grammar::Builder b;
    b.setAxiom({"axiom", "S"});
    b.addNonTerminal({"A", "A"});
    b.addNonTerminal({"B", "B"});
    b.addTerminal({"a", "a"});
    b.addTerminal({"b", "b"});
    b.addProduction({{{"axiom", "S"}}}, {{{"A", "A"}}});
    b.addProduction({{{"axiom", "S"}}}, {{{"B", "B"}}});
    b.addProduction({{{"A", "A"}}}, {{{"a", "a"}, {"B", "B"}}});
    b.addProduction({{{"A", "A"}}}, {{{"b", "b"}, {"axiom", "S"}}});
    b.addProduction({{{"A", "A"}}}, {{{"b", "b"}}});
    b.addProduction({{{"B", "B"}}}, {{{"A", "A"}, {"B", "B"}}});
    b.addProduction({{{"B", "B"}}}, {{{"B", "B"}, {"a", "a"}}});
    b.addProduction({{{"B", "B"}}}, {{{"A", "A"}, {"axiom", "S"}}});
    b.addProduction({{{"B", "B"}}}, {{{"b", "b"}}});

    return b.build();
}

TEST(RemoveBelong, AhoExample2_22_Page172) {
    Grammar g = getGrammar1();
    auto res = algorithm::removeBarrenSymbols(g);
    ASSERT_EQ(res, (std::unordered_set<Symbol>{{"axiom", "S"}, {"B", "B"}}));
}

TEST(RemoveBelongSymbolsTest, AhoExercise2_4_6_Page189) {
    Grammar g = getGrammar2();
    auto res = algorithm::removeBarrenSymbols(g);
    ASSERT_EQ(res, std::unordered_set<Symbol>({{"axiom", "S"}, {"A", "A"}, {"B", "B"}}));
}


TEST(RemoveUnrechebleSymbolsTest, AhoExample2_22_Page172) {
    Grammar::Builder b;
    b.setAxiom({"axiom", "S"});
    b.addNonTerminal({"B", "B"});
    b.addTerminal({"a", "a"});
    b.addTerminal({"b", "b"});
    b.addProduction({{ {"axiom", "S"} }},{{ {"a", "a"} }} );
    b.addProduction({{ {"B", "B"} }},{{ {"b", "b"} }} );
    Grammar g = b.build();

    auto res = algorithm::removeUnreachableSymbols(g);

    ASSERT_EQ(res, std::unordered_set<Symbol>({{"axiom", "S"}, {"a", "a"}}));
}

TEST(deleteDummySymbolsTest, AhoExample2_22_Page172) {
    Grammar g = getGrammar1();
    Grammar res = algorithm::deleteDummySymbols(g);
    Grammar::Builder b;
    b.setAxiom({"axiom", "S"});
    b.addTerminal({"a", "a"});
    b.addProduction({{ {"axiom", "S"} }}, {{ {"a", "a"} }});
    Grammar ok = b.build();
    ASSERT_EQ(res, ok);
}



TEST(removeDirectRecursion, oneRecursion) {
    Symbol S{"S", "S"};
    Symbol Ss{"S'", "S'"};
    Symbol b{"b", "b"};
    Symbol a{"a", "a"};
    Symbol e{"e", "e"};

    std::vector<Production> p{
        Production(ProductionPart({ S }), ProductionPart({ S, a })),
        Production(ProductionPart({ S }), ProductionPart({ b, a }))
    };
    std::vector<Production> res = algorithm::removeDirectRecursion(p, e, S);
    std::unordered_set<Production> ok {
        Production(ProductionPart({S}), ProductionPart({b, a, Ss})),
        Production(ProductionPart({Ss}), ProductionPart({ a, Ss })),
        Production(ProductionPart({Ss}), ProductionPart({ e }))
    };
    std::unordered_set<Production> resSet;
    for(const Production & prod : res) {
        resSet.insert(prod);
    }

    ASSERT_EQ(resSet, ok);
}

TEST(removeDirectRecursion, withoutRecursion) {
    Symbol S{"S", "S"};
    Symbol b{"b", "b"};
    Symbol a{"a", "a"};
    Symbol e{"e", "e"};

    std::vector<Production> p{
            Production(ProductionPart({ S }), ProductionPart({ a, a })),
            Production(ProductionPart({ S }), ProductionPart({ b, a }))
    };
    std::vector<Production> res = algorithm::removeDirectRecursion(p, e, S);
    std::unordered_set<Production> resSet;
    std::unordered_set<Production> ok;
    for(const Production & prod : res) {
        resSet.insert(prod);
    }
    for(const Production & prod : p) {
        ok.insert(prod);
    }

    ASSERT_EQ(resSet, ok);
}

TEST(removeDirectRecursion, rulesWithRecursionAndWithoutRecursion) {
    Symbol S{"S", "S"};
    Symbol Ss{"S'", "S'"};
    Symbol b{"b", "b"};
    Symbol a{"a", "a"};
    Symbol e{"e", "e"};

    std::vector<Production> p{
            Production(ProductionPart({ S }), ProductionPart({ a, a })),
            Production(ProductionPart({ S }), ProductionPart({ b, a })),
            Production(ProductionPart({ S }), ProductionPart({S, b, b })),
            Production(ProductionPart({ S }), ProductionPart({S, a, b })),
            Production(ProductionPart({ S }), ProductionPart({S, a, a }))
    };
    std::vector<Production> res = algorithm::removeDirectRecursion(p, e, S);
    std::unordered_set<Production> resSet;
    std::unordered_set<Production> ok{
            Production(ProductionPart({ S }), ProductionPart({ a, a, Ss})),
            Production(ProductionPart({ S }), ProductionPart({ b, a, Ss})),
            Production(ProductionPart({ Ss }), ProductionPart({ b, b, Ss })),
            Production(ProductionPart({ Ss }), ProductionPart({ a, b, Ss })),
            Production(ProductionPart({ Ss }), ProductionPart({ a, a, Ss })),
            Production(ProductionPart({ Ss }), ProductionPart({ e }))
    };
    for(const Production & prod : res) {
        resSet.insert(prod);
    }

    ASSERT_EQ(resSet, ok);
}

TEST(deleteRecurionTest, test) {
    Symbol A("A", "A");
    Symbol B("B", "B");
    Symbol C("C", "C");
    Symbol a("a", "a");
    Symbol b("b", "b");

    Grammar::Builder builder;
    builder.setAxiom(A);
    builder.setEpsilon({"e", "e"});
    builder.addNonTerminal(B);
    builder.addNonTerminal(C);
    builder.addTerminal(a);
    builder.addTerminal(b);
    builder.addProduction(ProductionPart({A}), ProductionPart({B, C}));
    builder.addProduction(ProductionPart({A}), ProductionPart({a}));
    builder.addProduction(ProductionPart({B}), ProductionPart({C, A}));
    builder.addProduction(ProductionPart({B}), ProductionPart({A, b}));
    builder.addProduction(ProductionPart({C}), ProductionPart({A, B}));
    builder.addProduction(ProductionPart({C}), ProductionPart({C, C}));
    builder.addProduction(ProductionPart({C}), ProductionPart({a}));
    Grammar g = builder.build();

    Grammar res = algorithm::deleteRecursion(g);
    std::cout << std::endl;
}

TEST(deleteRecurionTest, simple) {
    Symbol A("A", "A");
    Symbol S("S", "S");
    Symbol a("a", "a");
    Symbol b("b", "b");

    Grammar::Builder builder;
    builder.setAxiom(A);
    builder.setEpsilon({"e", "e"});
    builder.addNonTerminal(S);
    builder.addTerminal(a);
    builder.addTerminal(b);
    builder.addProduction(ProductionPart({A}), ProductionPart({S, a}));
    builder.addProduction(ProductionPart({S}), ProductionPart({A, b}));
    builder.addProduction(ProductionPart({S}), ProductionPart({b}));
    Grammar g = builder.build();

    Grammar res = algorithm::deleteRecursion(g);
    std::cout << std::endl;
}