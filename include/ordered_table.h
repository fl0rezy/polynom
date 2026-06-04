#pragma once
#include <vector>
#include <string>
#include "polynom.h"  

using namespace std;

class ordered_table {
private:
    vector<pair<string, polinom>> data;


    int findIndex(const string& key) const {
        if (data.empty()) return -1;

        int left = 0;
        int right = (int)data.size() - 1;

        while (left <= right) {
            int mid = (left + right) / 2;

            if (data[mid].first < key) left = mid + 1;
            else if (data[mid].first > key) right = mid - 1;
            else return mid;
        }
        return -1;
    }


    int lowerBoundIndex(const string& key) const {
        int l = 0;
        int r = (int)data.size();

        while (l < r) {
            int m = (l + r) / 2;
            if (data[m].first < key) l = m + 1;
            else r = m;
        }
        return l;
    }

public:
    class iterator {
    public:
        pair<string, polinom>* ptr;

        iterator() : ptr(nullptr) {}
        iterator(pair<string, polinom>* p) : ptr(p) {}

        iterator& operator=(const iterator& other) {
            if (this != &other) ptr = other.ptr;
            return *this;
        }

        pair<string, polinom>& operator*() const { return *ptr; }
        pair<string, polinom>* operator->() const { return ptr; }


        iterator& operator++() { ++ptr; return *this; }
        iterator& operator--() { --ptr; return *this; }

        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

        iterator operator+(int k) const { return iterator(ptr + k); }
        iterator operator-(int k) const { return iterator(ptr - k); }

        iterator& operator+=(int k) { ptr += k; return *this; }
        iterator& operator-=(int k) { ptr -= k; return *this; }

        bool operator==(const iterator& a) const { return ptr == a.ptr; }
        bool operator!=(const iterator& a) const { return ptr != a.ptr; }
    };

    ordered_table() = default;

    iterator insert(const string& str, const polinom& pol) {

        if (findIndex(str) != -1) return end();

        int pos = lowerBoundIndex(str);
        data.insert(data.begin() + pos, { str, pol });

        return iterator(data.empty() ? nullptr : data.data() + pos);
    }

    iterator erase(const string& str) {
        int idx = findIndex(str);
        if (idx == -1) return end();

        data.erase(data.begin() + idx);

        if (data.empty()) return iterator(nullptr);
        if (idx >= (int)data.size()) return end();
        return iterator(data.data() + idx);
    }

    iterator find(const string& str) {
        int idx = findIndex(str);
        if (idx == -1) return end();
        return iterator(data.data() + idx);
    }

    bool exist(const string& str) const {
        return findIndex(str) != -1;
    }

    polinom& operator[](const string& str) {
        int idx = findIndex(str);
        if (idx == -1) throw "The key is not in the table";
        return data[idx].second;
    }

    const polinom& operator[](const string& str) const {
        int idx = findIndex(str);
        if (idx == -1) throw "The key is not in the table";
        return data[idx].second;
    }

    iterator begin() {
        if (data.empty()) return iterator(nullptr);
        return iterator(data.data());
    }

    iterator end() {
        if (data.empty()) return iterator(nullptr);
        return iterator(data.data() + data.size());
    }

    int size() const { return (int)data.size(); }
    bool empty() const { return data.empty(); }
    void clear() { data.clear(); }
};
