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


TEST(buildTreeOfSymbolsTest, wq) {
    std::vector<Symbol> PRORABOTAT{{"П"}, {"Р"},{"О"},{"Р"},{"А"},{"Б"},{"О"},{"Т"},{"А"},{"Т"}, {"Ь"}};
    std::vector<Symbol> PRORIT{{"П"}, {"Р"},{"О"},{"Р"},{"Ы"},{"Т"},{"Ь"}};
    std::vector<Symbol> PROBUDIT{{"П"}, {"Р"},{"О"},{"Б"},{"У"},{"Д"},{"И"}, {"Т"}, {"Ь"}};
    std::vector<Symbol> PROBURIT{{"П"}, {"Р"},{"О"},{"Б"},{"У"},{"Р"},{"И"}, {"Т"}, {"Ь"}};
    std::vector<Symbol> PRODAT{{"П"}, {"Р"},{"О"},{"Д"},{"А"},{"Т"},{"Ь"}};
    std::vector<Symbol> KUPIT{{"К"}, {"У"},{"П"},{"И"},{"Т"},{"Ь"}};
    std::vector<Symbol> KRUTIT{{"К"}, {"Р"},{"У"},{"Т"},{"И"},{"Т"},{"Ь"}};
    std::vector<Symbol> KREPIT{{"К"}, {"Р"},{"Е"},{"П"},{"И"},{"Т"},{"Ь"}};
    std::vector<Symbol> KRESTIT{{"К"}, {"Р"},{"Е"},{"С"},{"Т"},{"И"},{"Т"},{"Ь"}};
    std::vector<Symbol> KRASIT{{"К"}, {"Р"},{"А"},{"С"},{"И"},{"Т"},{"Ь"}};
    std::vector<Symbol> KRAST{{"К"}, {"Р"},{"А"},{"С"},{"Т"},{"Ь"}};
    std::vector<Symbol> KOVIRAT{{"К"}, {"О"},{"В"},{"Ы"},{"Р"},{"Я"}, {"Т"},{"Ь"}};
    std::vector<Symbol> KOLUPAT{{"К"}, {"О"},{"Л"},{"У"},{"П"},{"А"}, {"Т"},{"Ь"}};
    std::vector<Symbol> PADAT{{"П"}, {"А"},{"Д"},{"А"},{"Т"},{"Ь"}};
    std::vector<Symbol> PLAKAT{{"П"}, {"Л"},{"А"},{"К"},{"А"},{"Т"},{"Ь"}};
    std::vector<std::vector<Symbol>> v{PRORABOTAT, PRORIT, PROBUDIT, PROBURIT, PRODAT, KUPIT, KRUTIT, KREPIT, KRESTIT,
            KRASIT, KRAST, KOVIRAT, KOLUPAT, PADAT, PLAKAT};
    Graph<Empty, Symbol> g = algorithm::buildTreeOfSymbols(v);
    std::cout << graphToDOT(g) << std::endl;
    std::cout << std::endl;
}