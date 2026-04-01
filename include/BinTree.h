#pragma once
#include <iostream>
#include <stack>
#include <string>
#include "polynom.h"

using namespace std;

class BinTree {
private:
    struct Node {
        string key;
        polinom data;
        Node* left;
        Node* right;

        Node(string _key, polinom _data)
            : key(_key), data(_data), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insert(Node* node, const string& key, const polinom& data) {
        if (node == nullptr) {
            return new Node(key, data);
        }

        if (key < node->key) {
            node->left = insert(node->left, key, data);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key, data);
        }
        else {
            node->data = data;
        }

        return node;
    }

    Node* find(Node* node, const string& key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (key < node->key) {
            return find(node->left, key);
        }
        else if (key > node->key) {
            return find(node->right, key);
        }
        else {
            return node;
        }
    }

    Node* findMin(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, const string& key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }

            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* minNode = findMin(node->right);
            node->key = minNode->key;
            node->data = minNode->data;
            node->right = remove(node->right, minNode->key);
        }

        return node;
    }

    void clear(Node* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    void print(Node* node) const {
        if (node == nullptr) return;
        print(node->left);
        cout << node->key << " ";
        print(node->right);
    }

public:
    BinTree() : root(nullptr) {}

    ~BinTree() {
        clear(root);
    }

    void insert(const string& key, const polinom& data) {
        root = insert(root, key, data);
    }

    polinom* find(const string& key) {
        Node* result = find(root, key);
        return result ? &result->data : nullptr;
    }

    void remove(const string& key) {
        root = remove(root, key);
    }

    void print() const {
        print(root);
        cout << endl;
    }

    bool empty() const {
        return root == nullptr;
    }

    class Iterator {
    private:
        Node* current;
        stack<Node*> stack;

        void pushLeft(Node* node) {
            while (node) {
                stack.push(node);
                node = node->left;
            }
        }

    public:
        Iterator(Node* root) {
            pushLeft(root);
            if (!stack.empty()) {
                current = stack.top();
                stack.pop();
            }
            else {
                current = nullptr;
            }
        }

        Iterator() : current(nullptr) {}

        Iterator& operator++() {
            if (current->right) {
                pushLeft(current->right);
            }
            if (!stack.empty()) {
                current = stack.top();
                stack.pop();
            }
            else {
                current = nullptr;
            }
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        pair<string, polinom> operator*() const {
            return { current->key, current->data };
        }

        Node* operator->() const {
            return current;
        }
    };

    Iterator begin() const {
        return Iterator(root);
    }

    Iterator end() const {
        return Iterator();
    }
};