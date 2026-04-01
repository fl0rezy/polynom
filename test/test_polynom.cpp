#include "gtest.h"
#include "polynom.h"
#include "unordered_table.h"
#include "ordered_table.h"

TEST(polynom, constr)
{
    ASSERT_ANY_THROW(polinom p("-1x^2y^7z3"));
}

TEST(polynom, add)
{
    polinom p1("2x^3y^7z^2+x^9");
    polinom p2("4x^9");
    polinom p3("2x^3y^7z^2+5x^9");
    EXPECT_EQ(p1 + p2, p3);
}

TEST(polynom, sub)
{
    polinom p1("2x^3y^7z^2+x^9");
    polinom p2("4x^9");
    polinom p3("2x^3y^7z^2-3x^9");
    EXPECT_EQ(p1 - p2, p3);
}

TEST(polynom, mul)
{
    polinom p1("2x^3y^7z^2+x^8");
    polinom p2("4x^9");
    polinom p3("8x^4y^7z^2+4x^9");
    polinom p4("4x");
    ASSERT_ANY_THROW(p1*p2);
    EXPECT_EQ(p1 * p4, p3);
}

TEST(polynom, div)
{
    polinom p1("2x^3y^7z^2+x^8");
    polinom p2("x");
    polinom p3("2x^2y^7z^2+x^7");
    EXPECT_EQ(p1 / p2, p3);
    ASSERT_ANY_THROW(p1/0);
}

TEST(polynom, mod)
{
    polinom p1("2x^3+3x^2+4x+5");
    polinom p2("x+2");
    polinom p3("-7");

    EXPECT_EQ(p1 % p2, p3);
}

TEST(polynom, eq)
{
    polinom p1("x+2");
    polinom p2("x+2");

    EXPECT_EQ(p1==p2, 1);
    EXPECT_EQ(p1 != p2, 0);
}

TEST(polynom, mul_const)
{
    polinom p1("2x^3+3x^2+4x+5");
    polinom p2("4x^3+6x^2+8x+10");

    EXPECT_EQ(p1 * 2, p2);
}

TEST(polynom, div_const)
{
    polinom p1("4x^3+6x^2+8x+10");
    polinom p2("2x^3+3x^2+4x+5");

    EXPECT_EQ(p1 / 2, p2);
}


TEST(ordered_table, empty_size) {
    ordered_table t;
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(ordered_table, insert_find) {
    ordered_table t;
    ASSERT_NO_THROW(t.insert("a", polinom("x+2")));
    EXPECT_EQ(t.size(), 1);
    EXPECT_FALSE(t.empty());
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("b") == t.end());
}

TEST(ordered_table, insert_many) {
    ordered_table t;
    ASSERT_NO_THROW(t.insert("b", polinom("x")));
    ASSERT_NO_THROW(t.insert("a", polinom("2x^3+1")));
    ASSERT_NO_THROW(t.insert("c", polinom("4x^9")));
    EXPECT_EQ(t.size(), 3);
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("b") != t.end());
    EXPECT_TRUE(t.find("c") != t.end());
}

TEST(ordered_table, insert_dupli) {
    ordered_table t;
    ASSERT_NO_THROW(t.insert("k", polinom("x+2")));
    EXPECT_EQ(t.size(), 1);

    ASSERT_NO_THROW(t.insert("k", polinom("2x^3+1")));
    EXPECT_EQ(t.size(), 1);
    EXPECT_TRUE(t.find("k") != t.end());
}

TEST(ordered_table, erase_exist) {
    ordered_table t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    t.insert("c", polinom("2x^3+1"));

    ASSERT_NO_THROW(t.erase("b"));
    EXPECT_EQ(t.size(), 2);
    EXPECT_TRUE(t.find("b") == t.end());
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("c") != t.end());
}

TEST(ordered_table, erase_not_exist) {
    ordered_table t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));

    ASSERT_NO_THROW(t.erase("nope"));
    EXPECT_EQ(t.size(), 2);
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("b") != t.end());
}

TEST(ordered_table, clear) {
    ordered_table t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));

    ASSERT_NO_THROW(t.clear());
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
    EXPECT_TRUE(t.begin() == t.end());
    EXPECT_TRUE(t.find("a") == t.end());
}

TEST(unordered_table, empty_size) {
    unordered_table t;
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(unordered_table, insert_find) {
    unordered_table t;
    ASSERT_NO_THROW(t.insert("a", polinom("x+2")));
    EXPECT_EQ(t.size(), 1);
    EXPECT_FALSE(t.empty());
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("b") == t.end());
}

TEST(unordered_table, insert_many) {
    unordered_table t;
    ASSERT_NO_THROW(t.insert("b", polinom("x")));
    ASSERT_NO_THROW(t.insert("a", polinom("2x^3+1")));
    ASSERT_NO_THROW(t.insert("c", polinom("4x^9")));
    EXPECT_EQ(t.size(), 3);
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("b") != t.end());
    EXPECT_TRUE(t.find("c") != t.end());
}

TEST(unordered_table, insert_dupli) {
    unordered_table t;
    ASSERT_NO_THROW(t.insert("k", polinom("x+2")));
    EXPECT_EQ(t.size(), 1);

    ASSERT_NO_THROW(t.insert("k", polinom("2x^3+1")));
    EXPECT_EQ(t.size(), 1);
    EXPECT_TRUE(t.find("k") != t.end());
}

TEST(unordered_table, erase_exist) {
    unordered_table t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    t.insert("c", polinom("2x^3+1"));

    ASSERT_NO_THROW(t.erase("b"));
    EXPECT_EQ(t.size(), 2);
    EXPECT_TRUE(t.find("b") == t.end());
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("c") != t.end());
}

TEST(unordered_table, erase_not_exist) {
    unordered_table t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));

    ASSERT_NO_THROW(t.erase("nope"));
    EXPECT_EQ(t.size(), 2);
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("b") != t.end());
}

TEST(unordered_table, clear) {
    unordered_table t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));

    ASSERT_NO_THROW(t.clear());
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.size(), 0);
    EXPECT_TRUE(t.begin() == t.end());
    EXPECT_TRUE(t.find("a") == t.end());
}
