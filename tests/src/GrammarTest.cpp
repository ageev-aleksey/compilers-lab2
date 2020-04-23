//
// Created by nrx on 16.04.2020.
//

#include "grammar/Grammar.h"
#include "gtest/gtest.h"

TEST(GrammarTest, Building) {
    Grammar::Builder b;
    Symbol epsilon{"epsilon", "e"};
    Symbol axiom{"axiom", "A"};
    Symbol nt1{"nt1", "Thirst"};
    Symbol t1{"terminal1", "1"};
    Symbol t2{"terminal2", "2"};

    b.setEpsilon(epsilon);
    b.setAxiom(axiom);
    b.addNonTerminal(nt1);
    b.addTerminal(t1);
    b.addTerminal(t2);
    Grammar g = b.build();

    ASSERT_EQ(g.axiom(), axiom);
    ASSERT_TRUE(g.terminals().find(t1) != g.terminals().cend());
    ASSERT_TRUE(g.terminals().find(t2) != g.terminals().cend());
    ASSERT_TRUE(g.nonTerminals().find(nt1) != g.terminals().cend());
    ASSERT_TRUE(g.nonTerminals().find(axiom) != g.terminals().cend());
    ASSERT_FALSE(g.nonTerminals().find(t1) != g.terminals().cend());
    ASSERT_FALSE(g.nonTerminals().find(t2) != g.terminals().cend());
    ASSERT_FALSE(g.terminals().find(nt1) != g.terminals().cend());
    ASSERT_FALSE(g.terminals().find(axiom) != g.terminals().cend());
}

TEST(GrammarTest, GetCopyAxiom) {
    Grammar::Builder b;
    b.setAxiom({"axiom", "s"});
    Grammar g = b.build();
    Symbol s = g.axiom();
}

TEST(GrammarTest, Compare) {

    Grammar::Builder b;
    Symbol epsilon{"epsilon", "e"};
    Symbol axiom{"axiom", "A"};
    Symbol nt1{"nt1", "Thirst"};
    Symbol t1{"terminal1", "1"};
    Symbol t2{"terminal2", "2"};

    b.setEpsilon(epsilon);
    b.setAxiom(axiom);
    b.addNonTerminal(nt1);
    b.addTerminal(t1);
    b.addTerminal(t2);
    b.addProduction({{axiom}}, {{t1}});
    b.addProduction({{nt1}}, {{t1, axiom, t2}});
    Grammar g1 = b.build();
    const Grammar g2(g1);
    ASSERT_TRUE(g1 == g2);
}