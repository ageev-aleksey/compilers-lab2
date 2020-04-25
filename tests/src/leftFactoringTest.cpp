//
// Created by nrx on 23.04.2020.
//

#include "grammar/algorithm.h"
#include "Graph.h"
#include "GraphToDOT.h"
#include "gtest/gtest.h"
#include <algorithm>


template<typename NodeType, typename LinkType>
std::vector<std::vector<size_t>> buildAdjacencyMatrix(Graph<NodeType, LinkType> g) {
    size_t n = g.getNodes().size();
    std::vector<std::vector<size_t>> res;
    res.resize(n);
    for(size_t i = 0; i < n; i++) {
        res[i].resize(n);
        std::fill_n(res[i].begin(), 0, n);
    }

    for(typename Graph<NodeType, LinkType>::Node &node : g.getNodes()) {
        for(typename Graph<NodeType, LinkType>::Link &link : node.links) {
            res[node.index][link.node.getIndex()] = 1;
        }
    }
    return res;
}


TEST(buildTreeOfSymbolsTest, OneWord) {
    std::vector<std::vector<Symbol>> v{{{"D", "D"}, {"V", "V"}, {"O", "O"}, {"R", "R"}}};
    Graph<Empty, Symbol> g = algorithm::buildTreeOfSymbols(v);
    std::vector<std::vector<size_t>> am = buildAdjacencyMatrix(g);
    std::vector<std::vector<size_t>> ok{{0, 1, 0, 0, 0},
                                        {0, 0, 1, 0, 0},
                                        {0, 0, 0, 1, 0},
                                        {0, 0, 0, 0, 1},
                                        {0, 0, 0, 0, 0}};
    ASSERT_EQ(am, ok);
}

TEST(buildTreeOfSymbolsTest, ThreeWord) {
    std::vector<Symbol> dvor{{"D", "D"}, {"V", "V"}, {"O", "O"}, {"R", "R"}};
    std::vector<Symbol> dver{{"D", "D"}, {"V", "V"}, {"E", "E"}, {"R", "R"}};
    std::vector<Symbol> kod{{"K", "K"}, {"O", "O"}, {"D", "D"}};
    std::vector<std::vector<Symbol>> v{dvor, dver, kod};
    Graph<Empty, Symbol> g = algorithm::buildTreeOfSymbols(v);
    std::vector<std::vector<size_t>> am = buildAdjacencyMatrix(g);
    std::vector<std::vector<size_t>> ok{{0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
                                        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    std::cout << graphToDOT(g) << std::endl;
    ASSERT_EQ(am, ok);
}


//TEST(buildTreeOfSymbolsTest, wq) {
//    std::vector<Symbol> PRORABOTAT{{"П"}, {"Р"},{"О"},{"Р"},{"А"},{"Б"},{"О"},{"Т"},{"А"},{"Т"}, {"Ь"}};
//    std::vector<Symbol> PRORIT{{"П"}, {"Р"},{"О"},{"Р"},{"Ы"},{"Т"},{"Ь"}};
//    std::vector<Symbol> PROBUDIT{{"П"}, {"Р"},{"О"},{"Б"},{"У"},{"Д"},{"И"}, {"Т"}, {"Ь"}};
//    std::vector<Symbol> PROBURIT{{"П"}, {"Р"},{"О"},{"Б"},{"У"},{"Р"},{"И"}, {"Т"}, {"Ь"}};
//    std::vector<Symbol> PRODAT{{"П"}, {"Р"},{"О"},{"Д"},{"А"},{"Т"},{"Ь"}};
//    std::vector<Symbol> KUPIT{{"К"}, {"У"},{"П"},{"И"},{"Т"},{"Ь"}};
//    std::vector<Symbol> KRUTIT{{"К"}, {"Р"},{"У"},{"Т"},{"И"},{"Т"},{"Ь"}};
//    std::vector<Symbol> KREPIT{{"К"}, {"Р"},{"Е"},{"П"},{"И"},{"Т"},{"Ь"}};
//    std::vector<Symbol> KRESTIT{{"К"}, {"Р"},{"Е"},{"С"},{"Т"},{"И"},{"Т"},{"Ь"}};
//    std::vector<Symbol> KRASIT{{"К"}, {"Р"},{"А"},{"С"},{"И"},{"Т"},{"Ь"}};
//    std::vector<Symbol> KRAST{{"К"}, {"Р"},{"А"},{"С"},{"Т"},{"Ь"}};
//    std::vector<Symbol> KOVIRAT{{"К"}, {"О"},{"В"},{"Ы"},{"Р"},{"Я"}, {"Т"},{"Ь"}};
//    std::vector<Symbol> KOLUPAT{{"К"}, {"О"},{"Л"},{"У"},{"П"},{"А"}, {"Т"},{"Ь"}};
//    std::vector<Symbol> PADAT{{"П"}, {"А"},{"Д"},{"А"},{"Т"},{"Ь"}};
//    std::vector<Symbol> PLAKAT{{"П"}, {"Л"},{"А"},{"К"},{"А"},{"Т"},{"Ь"}};
//    std::vector<std::vector<Symbol>> v{PRORABOTAT, PRORIT, PROBUDIT, PROBURIT, PRODAT, KUPIT, KRUTIT, KREPIT, KRESTIT,
//            KRASIT, KRAST, KOVIRAT, KOLUPAT, PADAT, PLAKAT};
//    Graph<Empty, Symbol> g = algorithm::buildTreeOfSymbols(v);
//    std::cout << graphToDOT(g) << std::endl;
//    std::cout << std::endl;
//}


TEST(findDeepestForkInTreeSymbolsTest, OneFork) {
    Graph<Empty, Symbol> g;
    auto v0 = g.addNodeInBack(Empty{});
    auto v1 = g.addNodeInBack(Empty{});
    auto v2 = g.addNodeInBack(Empty{});
    g.addLink(v0, v1, Symbol("s"));
    g.addLink(v0, v2, Symbol("s"));
    auto res = algorithm::findDeepestForkInTreeSymbols(g);
    ASSERT_EQ(res, v0);
}


TEST(findDeepestForkInTreeSymbolsTest, TwoFork) {
    Graph<Empty, Symbol> g;
    auto v0 = g.addNodeInBack(Empty{});
    auto v1 = g.addNodeInBack(Empty{});
    auto v2 = g.addNodeInBack(Empty{});
    auto v3 = g.addNodeInBack(Empty{});
    auto v4 = g.addNodeInBack(Empty{});
    auto v5 = g.addNodeInBack(Empty{});
    g.addLink(v0, v1, Symbol("s"));
    g.addLink(v1, v2, Symbol("s"));
    g.addLink(v1, v3, Symbol("s"));
    g.addLink(v3, v4, Symbol("s"));
    g.addLink(v3, v5, Symbol("s"));
    auto res = algorithm::findDeepestForkInTreeSymbols(g);
    ASSERT_EQ(res, v3);
}

TEST(buildParenTableOfTreeTest, ThreeNode) {
    Graph<Empty, Symbol> g;
    auto v0 = g.addNodeInBack(Empty{});
    auto v1 = g.addNodeInBack(Empty{});
    auto v2 = g.addNodeInBack(Empty{});
    g.addLink(v0, v1, Symbol("s"));
    g.addLink(v0, v2, Symbol("s"));
    std::vector<Graph<Empty, Symbol>::iterator> table = algorithm::buildParenTableOfTree(g);
    std::vector<Graph<Empty, Symbol>::iterator> ok{g.end(), v0, v0};
    ASSERT_EQ(table, ok);
}


TEST(findDeepestForkInTreeSymbolsTest, FiveNode) {
    Graph<Empty, Symbol> g;
    auto v0 = g.addNodeInBack(Empty{});
    auto v1 = g.addNodeInBack(Empty{});
    auto v2 = g.addNodeInBack(Empty{});
    auto v3 = g.addNodeInBack(Empty{});
    auto v4 = g.addNodeInBack(Empty{});
    auto v5 = g.addNodeInBack(Empty{});
    g.addLink(v0, v1, Symbol("s"));
    g.addLink(v1, v2, Symbol("s"));
    g.addLink(v1, v3, Symbol("s"));
    g.addLink(v3, v4, Symbol("s"));
    g.addLink(v3, v5, Symbol("s"));
    std::vector<Graph<Empty, Symbol>::iterator> table = algorithm::buildParenTableOfTree(g);
    std::vector<Graph<Empty, Symbol>::iterator> ok{g.end(), v0, v1, v1, v3, v3};
    ASSERT_EQ(table, ok);
}

TEST(leftFactoringTest, one) {
    Symbol S("S");
    Symbol Ss("S'");
    Symbol a("a");
    Symbol b("b");
    Symbol c("c");

    Grammar::Builder builder;
    builder.setAxiom(S);
    builder.addTerminal(a);
    builder.addTerminal(b);
    builder.addTerminal(c);
    builder.addProduction({{S}}, {{a, b}});
    builder.addProduction({{S}}, {{a, c}});
    Grammar g = builder.build();
    Grammar res = algorithm::leftFactoring(g);

    Grammar::Builder okb;
    okb.setAxiom(S);
    okb.addTerminal(a);
    okb.addTerminal(b);
    okb.addTerminal(c);
    okb.addNonTerminal(Ss);
    okb.addProduction({{S}}, {{a, Ss}});
    okb.addProduction({{Ss}}, {{b}});
    okb.addProduction({{Ss}}, {{c}});
    Grammar ok = okb.build();
   // ASSERT_EQ(res, ok);
}


TEST(leftFactoringTest, two) {
    Symbol S("S");
    Symbol Ss("S'");
    Symbol A("A");
    Symbol As("A'");
    Symbol a("a");
    Symbol b("b");
    Symbol c("c");


    Grammar::Builder builder;
    builder.setAxiom(S);
    builder.addNonTerminal(A);
    builder.addTerminal(a);
    builder.addTerminal(b);
    builder.addTerminal(c);
    builder.addProduction({{S}}, {{a,b,c,A}});
    builder.addProduction({{S}}, {{a,b,c,c,A}});
    builder.addProduction({{S}}, {{a,b,A}});
    builder.addProduction({{S}}, {{a,b,b,A}});
    builder.addProduction({{S}}, {{b,a,A}});
    builder.addProduction({{A}}, {{c, b}});
    builder.addProduction({{A}}, {{c, b, a,c}});
    builder.addProduction({{A}}, {{c, c}});
    Grammar g = builder.build();
    Grammar res = algorithm::leftFactoring(g);
   std::cout << std::endl;
}