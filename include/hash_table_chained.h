#pragma once
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <functional>
#include <stdexcept>

#include "polynom.h"

class hash_table {
private:
    using bucket_type = std::list<std::pair<std::string, polinom>>;

    std::vector<bucket_type> table;
    std::size_t count;
    double max_load_factor_value;

private:
    std::size_t get_index(const std::string& key) const {
        return std::hash<std::string>{}(key) % table.size();
    }

    void rehash_if_needed() {
        if (table.empty()) {
            rehash(8);
            return;
        }

        double current_load = static_cast<double>(count) / table.size();
        if (current_load > max_load_factor_value) {
            rehash(table.size() * 2);
        }
    }

    void rehash(std::size_t new_bucket_count) {
        std::vector<bucket_type> new_table(new_bucket_count);

        for (auto& bucket : table) {
            for (auto& elem : bucket) {
                std::size_t new_index = std::hash<std::string>{}(elem.first) % new_bucket_count;
                new_table[new_index].push_back(elem);
            }
        }

        table.swap(new_table);
    }

public:
    class iterator {
    private:
        hash_table* owner;
        std::size_t bucket_index;
        bucket_type::iterator list_it;

        void move_to_valid() {
            while (owner && bucket_index < owner->table.size()) {
                if (list_it != owner->table[bucket_index].end()) {
                    return;
                }
                ++bucket_index;
                if (bucket_index < owner->table.size()) {
                    list_it = owner->table[bucket_index].begin();
                }
            }

            if (owner && bucket_index >= owner->table.size()) {
                bucket_index = owner->table.size();
            }
        }

    public:
        iterator() : owner(nullptr), bucket_index(0) {}

        iterator(hash_table* ht, std::size_t idx, bucket_type::iterator it)
            : owner(ht), bucket_index(idx), list_it(it) {
            if (owner) {
                move_to_valid();
            }
        }

        std::pair<std::string, polinom>& operator*() const {
            if (!owner || bucket_index >= owner->table.size()) {
                throw std::out_of_range("iterator is end()");
            }
            return *list_it;
        }

        std::pair<std::string, polinom>* operator->() const {
            if (!owner || bucket_index >= owner->table.size()) {
                throw std::out_of_range("iterator is end()");
            }
            return &(*list_it);
        }

        iterator& operator++() {
            if (!owner || bucket_index >= owner->table.size()) {
                return *this;
            }

            ++list_it;
            move_to_valid();
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            if (owner != other.owner) return false;

            bool this_end = (!owner || bucket_index >= owner->table.size());
            bool other_end = (!other.owner || other.bucket_index >= other.owner->table.size());

            if (this_end && other_end) return true;
            if (this_end != other_end) return false;

            return bucket_index == other.bucket_index && list_it == other.list_it;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        friend class hash_table;
    };

    hash_table(std::size_t bucket_count = 8)
        : table(bucket_count), count(0), max_load_factor_value(0.75) {}

    iterator insert(const std::string& key, const polinom& value) {
        std::size_t index = get_index(key);

        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                return end();
            }
        }

        table[index].push_back({ key, value });
        ++count;

        rehash_if_needed();

        return find(key);
    }

    iterator find(const std::string& key) {
        std::size_t index = get_index(key);

        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                return iterator(this, index, it);
            }
        }

        return end();
    }

    iterator erase(const std::string& key) {
        std::size_t index = get_index(key);

        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                auto next_it = std::next(it);
                table[index].erase(it);
                --count;

                if (next_it != table[index].end()) {
                    return iterator(this, index, next_it);
                }

                ++index;
                while (index < table.size()) {
                    if (!table[index].empty()) {
                        return iterator(this, index, table[index].begin());
                    }
                    ++index;
                }

                return end();
            }
        }

        return end();
    }

    iterator begin() {
        for (std::size_t i = 0; i < table.size(); ++i) {
            if (!table[i].empty()) {
                return iterator(this, i, table[i].begin());
            }
        }
        return end();
    }

    iterator end() {
        return iterator(this, table.size(), {});
    }

    int size() const {
        return static_cast<int>(count);
    }

    bool empty() const {
        return count == 0;
    }

    void clear() {
        for (auto& bucket : table) {
            bucket.clear();
        }
        count = 0;
    }

    double load_factor() const {
        if (table.empty()) return 0.0;
        return static_cast<double>(count) / table.size();
    }

    std::size_t bucket_count() const {
        return table.size();
    }
};