#pragma once
#include "polynom.h"
#include <iostream>
#include <vector>

using namespace std;

class unordered_table {
private:
	vector<std::pair<std::string, polinom>> data;
public:

	class iterator {
	public:
		pair<string, polinom>* ptr;

		iterator() :ptr(nullptr) {}

		iterator(pair<string, polinom>* pair) : ptr(pair) {};


		iterator& operator=(const iterator& other) {
			if (this != &other) {
				ptr = other.ptr;
			}
			return *this;
		}

		iterator operator++() {
			ptr++;
			return *this;
		}

		iterator operator--() {
			ptr--;
			return *this;
		}

		iterator operator++(int) {
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

		iterator operator--(int) {
			iterator tmp = *this;
			--(*this);
			return tmp;
		}

		bool operator==(const iterator& a) {
			return ptr == a.ptr;
		}

		bool operator!=(const iterator& a) {
			return ptr != a.ptr;
		}

		pair<string, polinom>& operator*() const {
			return *ptr;
		}

		pair<string, polinom>* operator->() const {
			return ptr;
		}

		
	};

	unordered_table() {};

	iterator insert(const string& str, const polinom& pol) {
		for (auto it = begin(); it != end(); it++) {
			if (it->first == str) {
				return end();
			}
		}
		data.push_back({ str, pol });
		return end();
	}

	iterator erase(const string& str) {
		for (auto it = begin(); it != end(); ++it) {
			if ((*it).first == str) {
				auto next_it = data.erase(data.begin() + (it.ptr - data.data()));
				return iterator(&(*next_it));
			}
		}
		return end();
	}

	iterator find(const string& str) {
		for (auto it = begin(); it != end(); it++) {
			if (it->first == str) {
				return it;
			}

		}
		return end();
	}

	iterator begin() {
		return data.empty() ? iterator(nullptr) : iterator(data.data());
	}

	iterator end() {
		return data.empty() ? iterator(nullptr) : iterator(data.data() + data.size());
	}

	int size() const { return (int)data.size(); }
	bool empty() const { return data.empty(); }
	void clear() { data.clear(); }
};