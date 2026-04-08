#include "gtest.h"
#include "polynom.h"
#include "unordered_table.h"
#include "ordered_table.h"
#include "AVLTree.h"
#include "BinTree.h"
#include "rb_tree.h"
#include "hash_table.h"

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





TEST(bst, empty_bst) {
    BinTree t;
    EXPECT_TRUE(t.empty());
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(bst, insert_) {
    BinTree t;
    ASSERT_NO_THROW(t.insert("a", polinom("x+2")));
    EXPECT_FALSE(t.empty());
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("b") == nullptr);
}

TEST(bst, insert_many) {
    BinTree t;
    t.insert("b", polinom("x"));
    t.insert("a", polinom("2x^3+1"));
    t.insert("c", polinom("4x^9"));
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("b") != nullptr);
    EXPECT_TRUE(t.find("c") != nullptr);
}

TEST(bst, insert_dupli) {
    BinTree t;
    t.insert("k", polinom("x+2"));
    t.insert("k", polinom("2x^3+1"));
    polinom* p = t.find("k");
    EXPECT_EQ(*p, polinom("2x^3+1"));
}

TEST(bst, erase_) {
    BinTree t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    t.insert("c", polinom("2x^3+1"));
    t.remove("b");
    EXPECT_TRUE(t.find("b") == nullptr);
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("c") != nullptr);
}

TEST(bst, erase_2) {
    BinTree t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    ASSERT_NO_THROW(t.remove("nope"));
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("b") != nullptr);
}

TEST(bst, stress) {
    BinTree t;
    for (int i = 0; i < 100000; i++) {
        t.insert(to_string(i), polinom("x"));
    }
    for (int i = 0; i < 10000; i += 2) {
        t.remove(to_string(i));
    }
    EXPECT_TRUE(t.find("9999") != nullptr);
    EXPECT_TRUE(t.find("0") == nullptr);
}






TEST(avl, empty_) {
    AVLTree t;
    EXPECT_TRUE(t.empty());
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(avl, insert_) {
    AVLTree t;
    ASSERT_NO_THROW(t.insert("a", polinom("x+2")));
    EXPECT_FALSE(t.empty());
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("b") == nullptr);
}

TEST(avl, insert_many) {
    AVLTree t;
    t.insert("b", polinom("x"));
    t.insert("a", polinom("2x^3+1"));
    t.insert("c", polinom("4x^9"));
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("b") != nullptr);
    EXPECT_TRUE(t.find("c") != nullptr);
}

TEST(avl, insert_dupli) {
    AVLTree t;
    t.insert("k", polinom("x+2"));
    t.insert("k", polinom("2x^3+1"));
    polinom* p = t.find("k");
    EXPECT_EQ(*p, polinom("2x^3+1"));
}

TEST(avl, erase_) {
    AVLTree t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    t.insert("c", polinom("2x^3+1"));
    t.remove("b");
    EXPECT_TRUE(t.find("b") == nullptr);
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("c") != nullptr);
}

TEST(avl, erase_2) {
    AVLTree t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    ASSERT_NO_THROW(t.remove("nope"));
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("b") != nullptr);
}

TEST(avl, stress) {
    AVLTree t;
    for (int i = 0; i < 10000; i++) {
        t.insert(to_string(i), polinom("x"));
    }
    for (int i = 0; i < 10000; i += 2) {
        t.remove(to_string(i));
    }
    EXPECT_TRUE(t.find("9999") != nullptr);
    EXPECT_TRUE(t.find("0") == nullptr);
}

TEST(avl, iterator_empty) {
    AVLTree t;
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(avl, iterator) {
    AVLTree t;
    t.insert("banana", polinom("1"));
    t.insert("apple", polinom("2"));
    t.insert("cherry", polinom("3"));
    t.insert("apricot", polinom("4"));

    vector<string> keys;
    for (auto it = t.begin(); it != t.end(); ++it) {
        keys.push_back((*it).first);
    }

    EXPECT_EQ(keys.size(), 4);
    EXPECT_TRUE(find(keys.begin(), keys.end(), "banana") != keys.end());
    EXPECT_TRUE(find(keys.begin(), keys.end(), "apple") != keys.end());
    EXPECT_TRUE(find(keys.begin(), keys.end(), "cherry") != keys.end());
    EXPECT_TRUE(find(keys.begin(), keys.end(), "apricot") != keys.end());
}





TEST(rb, empty_) {
    rb_tree<string, polinom> t;
    EXPECT_TRUE(t.empty());
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(rb, insert_) {
    rb_tree<string, polinom> t;
    ASSERT_NO_THROW(t.insert("a", polinom("x+2")));
    EXPECT_FALSE(t.empty());
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("b") == t.end());
}

TEST(rb, insert_many) {
    rb_tree<string, polinom> t;
    t.insert("b", polinom("x"));
    t.insert("a", polinom("2x^3+1"));
    t.insert("c", polinom("4x^9"));
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("b") != t.end());
    EXPECT_TRUE(t.find("c") != t.end());
}

TEST(rb, insert_dupli) {
    rb_tree<string, polinom> t;
    t.insert("k", polinom("x+2"));
    t.insert("k", polinom("2x^3+1"));
    EXPECT_EQ(t.size(), 1);
    EXPECT_TRUE(t.find("k") != t.end());
}

TEST(rb, erase_) {
    rb_tree<string, polinom> t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    t.insert("c", polinom("2x^3+1"));
    t.erase("b");
    EXPECT_TRUE(t.find("b") == t.end());
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("c") != t.end());
}

TEST(rb, erase_2) {
    rb_tree<string, polinom> t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    ASSERT_NO_THROW(t.erase("nope"));
    EXPECT_TRUE(t.find("a") != t.end());
    EXPECT_TRUE(t.find("b") != t.end());
}

TEST(rb, stress) {
    rb_tree<string, polinom> t;
    for (int i = 0; i < 10000; i++) {
        t.insert(to_string(i), polinom("x"));
    }
    for (int i = 0; i < 10000; i += 2) {
        t.erase(to_string(i));
    }
    EXPECT_TRUE(t.find("9999") != t.end());
    EXPECT_TRUE(t.find("0") == t.end());
}

TEST(rb, iterator_empty) {
    rb_tree<string, polinom> t;
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(rb, iterator) {
    rb_tree<string, polinom> t;
    t.insert("banana", polinom("1"));
    t.insert("apple", polinom("2"));
    t.insert("cherry", polinom("3"));
    t.insert("apricot", polinom("4"));

    vector<string> keys;
    for (auto it = t.begin(); it != t.end(); ++it) {
        keys.push_back((*it).first);
    }

    EXPECT_EQ(keys.size(), 4);
    EXPECT_TRUE(find(keys.begin(), keys.end(), "banana") != keys.end());
    EXPECT_TRUE(find(keys.begin(), keys.end(), "apple") != keys.end());
    EXPECT_TRUE(find(keys.begin(), keys.end(), "cherry") != keys.end());
    EXPECT_TRUE(find(keys.begin(), keys.end(), "apricot") != keys.end());
}






TEST(hash, empty_) {
    HashTable t;
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.getSize(), 0);
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(hash, insert_) {
    HashTable t;
    ASSERT_NO_THROW(t.insert("a", polinom("x+2")));
    EXPECT_FALSE(t.empty());
    EXPECT_EQ(t.getSize(), 1);
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("b") == nullptr);
}

TEST(hash, insert_many) {
    HashTable t;
    t.insert("b", polinom("x"));
    t.insert("a", polinom("2x^3+1"));
    t.insert("c", polinom("4x^9"));
    EXPECT_EQ(t.getSize(), 3);
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("b") != nullptr);
    EXPECT_TRUE(t.find("c") != nullptr);
}

TEST(hash, insert_duplicate) {
    HashTable t;
    t.insert("k", polinom("x+2"));
    bool ok = t.insert("k", polinom("2x^3+1"));
    EXPECT_FALSE(ok);
    EXPECT_EQ(t.getSize(), 1);
    polinom* p = t.find("k");
    EXPECT_EQ(*p, polinom("x+2"));
}

TEST(hash, erase_) {
    HashTable t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    t.insert("c", polinom("2x^3+1"));
    bool ok = t.erase("b");
    EXPECT_TRUE(ok);
    EXPECT_EQ(t.getSize(), 2);
    EXPECT_TRUE(t.find("b") == nullptr);
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("c") != nullptr);
}

TEST(hash, erase_2) {
    HashTable t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    bool ok = t.erase("nope");
    EXPECT_FALSE(ok);
    EXPECT_EQ(t.getSize(), 2);
    EXPECT_TRUE(t.find("a") != nullptr);
    EXPECT_TRUE(t.find("b") != nullptr);
}

TEST(hash, clear_) {
    HashTable t;
    t.insert("a", polinom("x"));
    t.insert("b", polinom("x+2"));
    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.getSize(), 0);
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(hash, stress) {
    HashTable t;
    for (int i = 0; i < 10000; i++) {
        t.insert(to_string(i), polinom("x"));
    }
    for (int i = 0; i < 10000; i += 2) {
        t.erase(to_string(i));
    }
    EXPECT_TRUE(t.find("9999") != nullptr);
    EXPECT_TRUE(t.find("0") == nullptr);
}


TEST(hash, iterator_empty) {
    HashTable t;
    EXPECT_TRUE(t.begin() == t.end());
}

TEST(hash, iterator) {
    HashTable t;
    t.insert("banana", polinom("1"));
    t.insert("apple", polinom("2"));
    t.insert("cherry", polinom("3"));
    t.insert("apricot", polinom("4"));

    vector<string> keys;
    for (auto it = t.begin(); it != t.end(); ++it) {
        keys.push_back((*it).first);
    }

    EXPECT_EQ(keys.size(), 4);
    EXPECT_TRUE(find(keys.begin(), keys.end(), "banana") != keys.end());
    EXPECT_TRUE(find(keys.begin(), keys.end(), "apple") != keys.end());
    EXPECT_TRUE(find(keys.begin(), keys.end(), "cherry") != keys.end());
    EXPECT_TRUE(find(keys.begin(), keys.end(), "apricot") != keys.end());
}