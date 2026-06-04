#pragma once
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include "polynom.h"
#include "ordered_table.h"
#include "unordered_table.h"
#include "hash_table.h"
#include "hash_table_chained.h"
#include "BinTree.h"
#include "AVLTree.h"
#include "rb_tree.h"

struct Record
{
    std::string key;
    polinom value;
};

enum class StorageType
{
    OrderedTable,
    UnorderedTable,
    HashOpen,
    HashChained,
    BinTree,
    AVLTree,
    RBTree
};

class ITableAdapter
{
public:
    virtual ~ITableAdapter() = default;
    virtual bool insert(const std::string& key, const polinom& value) = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual bool find(const std::string& key, polinom& out) = 0;
    virtual std::vector<Record> all() = 0;
    virtual void clear() = 0;
    virtual const char* name() const = 0;
};

class OrderedTableAdapter : public ITableAdapter
{
    ordered_table table;
public:
    bool insert(const std::string& key, const polinom& value) override
    {
        return table.insert(key, value) != table.end();
    }

    bool remove(const std::string& key) override
    {
        if (table.find(key) == table.end()) return false;
        table.erase(key);
        return true;
    }

    bool find(const std::string& key, polinom& out) override
    {
        auto it = table.find(key);
        if (it == table.end()) return false;
        out = it->second;
        return true;
    }

    std::vector<Record> all() override
    {
        std::vector<Record> result;
        for (auto it = table.begin(); it != table.end(); ++it)
            result.push_back({ it->first, it->second });
        return result;
    }

    void clear() override { table.clear(); }
    const char* name() const override { return "ordered_table"; }
};

class UnorderedTableAdapter : public ITableAdapter
{
    unordered_table table;
public:
    bool insert(const std::string& key, const polinom& value) override
    {
        if (table.find(key) != table.end()) return false;
        table.insert(key, value);
        return true;
    }

    bool remove(const std::string& key) override
    {
        if (table.find(key) == table.end()) return false;
        table.erase(key);
        return true;
    }

    bool find(const std::string& key, polinom& out) override
    {
        auto it = table.find(key);
        if (it == table.end()) return false;
        out = it->second;
        return true;
    }

    std::vector<Record> all() override
    {
        std::vector<Record> result;
        for (auto it = table.begin(); it != table.end(); ++it)
            result.push_back({ it->first, it->second });
        return result;
    }

    void clear() override { table.clear(); }
    const char* name() const override { return "unordered_table"; }
};

class HashOpenAdapter : public ITableAdapter
{
    HashTable table;
public:
    bool insert(const std::string& key, const polinom& value) override
    {
        return table.insert(key, value);
    }

    bool remove(const std::string& key) override
    {
        return table.erase(key);
    }

    bool find(const std::string& key, polinom& out) override
    {
        polinom* p = table.find(key);
        if (!p) return false;
        out = *p;
        return true;
    }

    std::vector<Record> all() override
    {
        std::vector<Record> result;
        for (auto it = table.begin(); it != table.end(); ++it)
        {
            auto pair = *it;
            result.push_back({ pair.first, pair.second });
        }
        return result;
    }

    void clear() override { table.clear(); }
    const char* name() const override { return "HashTable(open addressing)"; }
};

class HashChainedAdapter : public ITableAdapter
{
    hash_table table;
public:
    bool insert(const std::string& key, const polinom& value) override
    {
        return table.insert(key, value) != table.end();
    }

    bool remove(const std::string& key) override
    {
        if (table.find(key) == table.end()) return false;
        table.erase(key);
        return true;
    }

    bool find(const std::string& key, polinom& out) override
    {
        auto it = table.find(key);
        if (it == table.end()) return false;
        out = it->second;
        return true;
    }

    std::vector<Record> all() override
    {
        std::vector<Record> result;
        for (auto it = table.begin(); it != table.end(); ++it)
            result.push_back({ it->first, it->second });
        return result;
    }

    void clear() override { table.clear(); }
    const char* name() const override { return "hash_table(chaining)"; }
};

class BinTreeAdapter : public ITableAdapter
{
    BinTree tree;
public:
    bool insert(const std::string& key, const polinom& value) override
    {
        if (tree.find(key) != nullptr) return false;
        tree.insert(key, value);
        return true;
    }

    bool remove(const std::string& key) override
    {
        if (tree.find(key) == nullptr) return false;
        tree.remove(key);
        return true;
    }

    bool find(const std::string& key, polinom& out) override
    {
        polinom* p = tree.find(key);
        if (!p) return false;
        out = *p;
        return true;
    }

    std::vector<Record> all() override
    {
        std::vector<Record> result;
        for (auto it = tree.begin(); it != tree.end(); ++it)
        {
            auto pair = *it;
            result.push_back({ pair.first, pair.second });
        }
        return result;
    }

    void clear() override
    {
        BinTree empty;
        tree = std::move(empty);
    }

    const char* name() const override { return "BinTree"; }
};

class AVLTreeAdapter : public ITableAdapter
{
    AVLTree tree;
public:
    bool insert(const std::string& key, const polinom& value) override
    {
        if (tree.find(key) != nullptr) return false;
        tree.insert(key, value);
        return true;
    }

    bool remove(const std::string& key) override
    {
        if (tree.find(key) == nullptr) return false;
        tree.remove(key);
        return true;
    }

    bool find(const std::string& key, polinom& out) override
    {
        polinom* p = tree.find(key);
        if (!p) return false;
        out = *p;
        return true;
    }

    std::vector<Record> all() override
    {
        std::vector<Record> result;
        for (auto it = tree.begin(); it != tree.end(); ++it)
        {
            auto pair = *it;
            result.push_back({ pair.first, pair.second });
        }
        return result;
    }

    void clear() override
    {
        AVLTree empty;
        tree = std::move(empty);
    }

    const char* name() const override { return "AVLTree"; }
};

class RBTreeAdapter : public ITableAdapter
{
    rb_tree<std::string, polinom> tree;
public:
    bool insert(const std::string& key, const polinom& value) override
    {
        return tree.insert(key, value) != tree.end();
    }

    bool remove(const std::string& key) override
    {
        if (tree.find(key) == tree.end()) return false;
        tree.erase(key);
        return true;
    }

    bool find(const std::string& key, polinom& out) override
    {
        auto it = tree.find(key);
        if (it == tree.end()) return false;
        out = it->second;
        return true;
    }

    std::vector<Record> all() override
    {
        std::vector<Record> result;
        for (auto it = tree.begin(); it != tree.end(); ++it)
            result.push_back({ it->first, it->second });
        return result;
    }

    void clear() override { tree.clear(); }
    const char* name() const override { return "rb_tree"; }
};

inline std::unique_ptr<ITableAdapter> CreateAdapter(StorageType type)
{
    switch (type)
    {
    case StorageType::OrderedTable:   return std::make_unique<OrderedTableAdapter>();
    case StorageType::UnorderedTable: return std::make_unique<UnorderedTableAdapter>();
    case StorageType::HashOpen:       return std::make_unique<HashOpenAdapter>();
    case StorageType::HashChained:    return std::make_unique<HashChainedAdapter>();
    case StorageType::BinTree:        return std::make_unique<BinTreeAdapter>();
    case StorageType::AVLTree:        return std::make_unique<AVLTreeAdapter>();
    case StorageType::RBTree:         return std::make_unique<RBTreeAdapter>();
    default:                          return std::make_unique<OrderedTableAdapter>();
    }
}

inline std::vector<std::pair<StorageType, std::string>> GetStorageItems()
{
    return {
        {StorageType::OrderedTable,   "Ordered"},
        {StorageType::UnorderedTable, "Unordered"},
        {StorageType::HashOpen,       "Hash Open"},
        {StorageType::HashChained,    "Hash Chained"},
        {StorageType::BinTree,        "BST"},
        {StorageType::AVLTree,        "AVL"},
        {StorageType::RBTree,         "RB"}
    };
}
