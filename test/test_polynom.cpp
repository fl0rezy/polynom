#include "gtest.h"
#include "polynom.h"

TEST(test, constr)
{
    ASSERT_ANY_THROW(polinom p("-1x^2y^7z3"));
}

TEST(test, add)
{
    polinom p1("2x^3y^7z^2+x^9");
    polinom p2("4x^9");
    polinom p3("2x^3y^7z^2+5x^9");
    EXPECT_EQ(p1 + p2, p3);
}

TEST(test, sub)
{
    polinom p1("2x^3y^7z^2+x^9");
    polinom p2("4x^9");
    polinom p3("2x^3y^7z^2-3x^9");
    EXPECT_EQ(p1 - p2, p3);
}

TEST(test, mul)
{
    polinom p1("2x^3y^7z^2+x^8");
    polinom p2("4x^9");
    polinom p3("8x^4y^7z^2+4x^9");
    polinom p4("4x");
    ASSERT_ANY_THROW(p1*p2);
    EXPECT_EQ(p1 * p4, p3);
}

TEST(test, div)
{
    polinom p1("2x^3y^7z^2+x^8");
    polinom p2("x");
    polinom p3("2x^2y^7z^2+x^7");
    EXPECT_EQ(p1 / p2, p3);
    ASSERT_ANY_THROW(p1/0);
}

TEST(test, mod)
{
    polinom p1("2x^3+3x^2+4x+5");
    polinom p2("x+2");
    polinom p3("-7");

    EXPECT_EQ(p1 % p2, p3);
}

TEST(test, eq)
{
    polinom p1("x+2");
    polinom p2("x+2");

    EXPECT_EQ(p1==p2, 1);
    EXPECT_EQ(p1 != p2, 0);
}

TEST(test, mul_const)
{
    polinom p1("2x^3+3x^2+4x+5");
    polinom p2("4x^3+6x^2+8x+10");

    EXPECT_EQ(p1 * 2, p2);
}

TEST(test, div_const)
{
    polinom p1("4x^3+6x^2+8x+10");
    polinom p2("2x^3+3x^2+4x+5");

    EXPECT_EQ(p1 / 2, p2);
}

