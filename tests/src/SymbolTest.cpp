//
// Created by nrx on 15.04.2020.
//
#include "grammar/Symbol.h"
#include "gtest/gtest.h"

TEST(SymbolTest, CreateCopy1) {
    Symbol s1("name", "pattern");
    Symbol s2 = s1;
    ASSERT_EQ(s1.name(), s2.name());
    ASSERT_EQ(s1.pattern(), s2.pattern());
    ASSERT_TRUE(s1 == s2);
}

TEST(SymbolTest, CreateDeepCopy1) {
    Symbol s1("name", "pattern");
    Symbol s2 = s1.copy();
    ASSERT_EQ(s1.name(), s2.name());
    ASSERT_EQ(s1.pattern(), s2.pattern());
    ASSERT_TRUE(s1 == s2);
}

TEST(SymbolTest, CreateCopy2) {
    Symbol s1(std::string("name"), std::string("pattern"));
    Symbol s2 = s1;
    ASSERT_EQ(s1.name(), s2.name());
    ASSERT_EQ(s1.pattern(), s2.pattern());
    ASSERT_TRUE(s1 == s2);
}

TEST(SymbolTest, CreateDeepCopy2) {
    Symbol s1(std::string("name"),std::string("pattern"));
    Symbol s2 = s1.copy();
    ASSERT_EQ(s1.name(), s2.name());
    ASSERT_EQ(s1.pattern(), s2.pattern());
    ASSERT_TRUE(s1 == s2);
}

TEST(SymbolTest, DeepCopy) {
    Symbol s1("name", "pattern");
    Symbol copy = s1;
    Symbol deepCopy = s1.copy();

    ASSERT_TRUE(s1.isSame(copy));
    ASSERT_FALSE(s1.isSame(deepCopy));
}


