//
// Created by nrx on 15.04.2020.
//

#include "grammar/GrammarException.h"
#include "grammar/Symbol.h"
#include "grammar/ProductionPart.h"
#include "gtest/gtest.h"

TEST(ProductionPartTest, CreateAndGetSymbols) {
    std::vector<Symbol> s{{"first", "1"}, {"second", "2"}, {"third", "3"}};
    ProductionPart p(s);

    for(auto pi = p.begin(), si = s.begin(); pi != p.end(); pi++, si++) {
        ASSERT_TRUE(pi->isSame(*si));
    }
}

TEST(ProductionPartTest, AddSymbols) {
    std::vector<Symbol> s{{"first", "1"}, {"second", "2"}, {"third", "3"}};
    ProductionPart p;
    for(const Symbol el : s) {
        p.addSymbol(el);
    }

    for(auto pi = p.begin(), si = s.begin(); pi != p.end(); pi++, si++) {
        ASSERT_TRUE(pi->isSame(*si));
    }
}


TEST(ProductionPartTest, SymbolContain) {
    ProductionPart p({{"first", "1"}, {"second", "2"}, {"third", "3"}});

    ASSERT_EQ(*p.isContain({"first", "1"}).result(), 0);
    ASSERT_EQ(*p.isContain({"second", "2"}).result(), 1);
    ASSERT_EQ(*p.isContain({"third", "3"}).result(), 2);
}

//TODO ПРОВЕРЯТЬ НА КОРРЕКТНОСТЬ МНОЖЕСТВА СИМВОЛОВ ЗДЕСЬ ИЛИ В ГРАММАТИКЕ?
//TEST(ProductionPartTest, addInvalidSymbols) {
//    ProductionPart p;
//    p.addSymbol({"first", "1"});
//    ASSERT_THROW(p.addSymbol({"first", "2"});, GrammarException);
//    p.addSymbol({"second", "1"});
//}

TEST(ProductionPartTest, CopyConstructorAndCompare) {
    std::vector<Symbol> s{{"first", "1"}, {"second", "2"}, {"third", "3"}};
    ProductionPart p1(s);
    ProductionPart p2 = p1;
    ASSERT_TRUE(p1 == p2);
}

TEST(ProductionPartTest, MoveConstructorAndCompare) {
    std::vector<Symbol> s{{"first", "1"}, {"second", "2"}, {"third", "3"}};
    ProductionPart p1(s);
    ProductionPart p2 = std::move(p1);
    ProductionPart p3(s);
    ASSERT_FALSE(p1 == p2);
    ASSERT_TRUE(p2 == p3);
}

TEST(ProductionPartTest, ForEach) {
    std::vector<Symbol> s{{"first", "1"}, {"second", "2"}, {"third", "3"}};
    ProductionPart p1(s);
    int i = 0;
    for(Symbol el : p1) {
        ASSERT_EQ(el, s[i]);
        i++;
    }

}

TEST(ProductionPartTest, ConstCopy) {
    ProductionPart p1({{"a", "a"}, {"b", "b"}});
    ProductionPart p2;
    p2 = p1;
}