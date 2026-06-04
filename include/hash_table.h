#pragma once
#include <iostream>
#include <string>
#include "polynom.h"
#include <vector>

using namespace std;

class HashTable {
private:
    struct Cell {
        string key;  
        polinom value;
        bool isOccupied; 
        bool isDeleted;

        Cell() : key(""), value(polinom()), isOccupied(false), isDeleted(false) {}
        Cell(const std::string& k, const polinom& v): key(k), value(v), isOccupied(true), isDeleted(false) {}
    };

    vector<Cell> table;
    int size;
    int step = 7;

    int h(const string& key) const {
        int sum = 0;
        for (char ch : key) {
            sum = sum * 31 + ch;
        }
        return sum & (table.size() - 1);
    }

    int hh(int curInd) const{
        return (curInd + step) & (table.size() - 1);
    }

    int hashForSize(const string& key, int size) const {
        int sum = 0;
        for (char ch : key) {
            sum = sum * 31 + ch;
        }
        return sum & (size - 1);
    }

    int nextIndexForSize(int curInd, int size) const {
        return (curInd + step) & (size - 1);
    }

    double alfa()const {
        return (double)size / (table.size());
    }

    bool needRehash() const {
        return alfa() >= 0.8;
    }

    void rehash() {
        int oldsize = table.size();
        int newsize = oldsize * 2;
        vector<Cell> newtable(newsize);

        for (const auto& cell : table) {
            if (cell.isOccupied) {
                int ind = hashForSize(cell.key, newsize);
                while (newtable[ind].isOccupied) {
                    ind = nextIndexForSize(ind, newsize);
                }
                newtable[ind] = cell;
            }
        }
        table = move(newtable);
    }

public:
    HashTable(int initialCapacity = 16) : table(initialCapacity), size(0) {}

    bool insert(const string& key, const polinom& val) {
        if (needRehash()) {
            rehash();
        }

        int ind = h(key);

        while (table[ind].isOccupied) {
            if (table[ind].key == key) {
                return false;
            }
            ind = hh(ind);
        }

        table[ind] = Cell(key, val);
        size++;
        return true;
    }

    polinom* find(const string& key) {
        int ind = h(key);

        while (table[ind].isDeleted || table[ind].isOccupied) {
            if (table[ind].isOccupied && table[ind].key == key) {
                return &table[ind].value;
            }
            ind = hh(ind);
        }
        return nullptr;
    }

    bool erase(const string& key) {
        int ind = h(key);
        while (table[ind].isOccupied || table[ind].isDeleted) {
            if (table[ind].isOccupied && table[ind].key == key) {
                table[ind].isOccupied = false;
                table[ind].isDeleted = true;
                size--;
                return true;
            }
            ind = hh(ind);
        }
        return false;
    }

    bool empty() const { return size == 0; }

    int getSize() const { return size; }

    void clear() {
        table.clear();
        table.resize(16);
        size = 0;
    }

    class Iterator {
    private:
        const vector<Cell>* table_ptr;
        int index;

        void findNext() {
            while (index < (int)table_ptr->size() && !(*table_ptr)[index].isOccupied) {
                index++;
            }
        }

    public:
        Iterator(const vector<Cell>* ptr, int start): table_ptr(ptr), index(start) {
            findNext();
        }

        Iterator& operator++() {
            index++;
            findNext();
            return *this;
        }

        pair<string, polinom> operator*() const {
            return { (*table_ptr)[index].key, (*table_ptr)[index].value };
        }

        bool operator==(const Iterator& other) const {
            return index == other.index && table_ptr == other.table_ptr;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() const {
        return Iterator(&table, 0);
    }

    Iterator end() const {
        return Iterator(&table, table.size());
    }
};