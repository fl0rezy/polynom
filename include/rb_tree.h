#pragma once
#include <utility>
#include <stdexcept>
#include <cstddef>

template <typename Key, typename Value>
class rb_tree {
private:
    enum Color { RED, BLACK };

    struct Node {
        std::pair<Key, Value> data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(const Key& key, const Value& value)
            : data(key, value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root = nullptr;
    std::size_t count = 0;

private:
    static Color getColor(Node* node) {
        return node ? node->color : BLACK;
    }

    static Node* minimum(Node* node) {
        if (!node) return nullptr;
        while (node->left) node = node->left;
        return node;
    }

    static Node* maximum(Node* node) {
        if (!node) return nullptr;
        while (node->right) node = node->right;
        return node;
    }

    static Node* successor(Node* node) {
        if (!node) return nullptr;
        if (node->right) return minimum(node->right);

        Node* p = node->parent;
        while (p && node == p->right) {
            node = p;
            p = p->parent;
        }
        return p;
    }

    static Node* predecessor(Node* node) {
        if (!node) return nullptr;
        if (node->left) return maximum(node->left);

        Node* p = node->parent;
        while (p && node == p->left) {
            node = p;
            p = p->parent;
        }
        return p;
    }

    Node* findNode(const Key& key) const {
        Node* cur = root;
        while (cur) {
            if (key < cur->data.first) {
                cur = cur->left;
            }
            else if (key > cur->data.first) {
                cur = cur->right;
            }
            else {
                return cur;
            }
        }
        return nullptr;
    }

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;

        y->parent = x->parent;
        if (!x->parent) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right) x->right->parent = y;

        x->parent = y->parent;
        if (!y->parent) {
            root = x;
        }
        else if (y == y->parent->left) {
            y->parent->left = x;
        }
        else {
            y->parent->right = x;
        }

        x->right = y;
        y->parent = x;
    }

    void fixInsert(Node* z) {
        while (z != root && getColor(z->parent) == RED) {
            Node* parent = z->parent;
            Node* grand = parent->parent;

            if (parent == grand->left) {
                Node* uncle = grand->right;

                if (getColor(uncle) == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grand->color = RED;
                    z = grand;
                }
                else {
                    if (z == parent->right) {
                        z = parent;
                        rotateLeft(z);
                        parent = z->parent;
                        grand = parent->parent;
                    }
                    parent->color = BLACK;
                    grand->color = RED;
                    rotateRight(grand);
                }
            }
            else {
                Node* uncle = grand->left;

                if (getColor(uncle) == RED) {
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grand->color = RED;
                    z = grand;
                }
                else {
                    if (z == parent->left) {
                        z = parent;
                        rotateRight(z);
                        parent = z->parent;
                        grand = parent->parent;
                    }
                    parent->color = BLACK;
                    grand->color = RED;
                    rotateLeft(grand);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node* u, Node* v) {
        if (!u->parent) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }

        if (v) v->parent = u->parent;
    }

    void fixErase(Node* x, Node* parent) {
        while (x != root && getColor(x) == BLACK) {
            if (x == (parent ? parent->left : nullptr)) {
                Node* w = parent ? parent->right : nullptr;

                if (getColor(w) == RED) {
                    w->color = BLACK;
                    parent->color = RED;
                    rotateLeft(parent);
                    w = parent->right;
                }

                if (getColor(w ? w->left : nullptr) == BLACK &&
                    getColor(w ? w->right : nullptr) == BLACK) {
                    if (w) w->color = RED;
                    x = parent;
                    parent = x ? x->parent : nullptr;
                }
                else {
                    if (getColor(w ? w->right : nullptr) == BLACK) {
                        if (w && w->left) w->left->color = BLACK;
                        if (w) w->color = RED;
                        if (w) rotateRight(w);
                        w = parent ? parent->right : nullptr;
                    }

                    if (w) w->color = parent ? parent->color : BLACK;
                    if (parent) parent->color = BLACK;
                    if (w && w->right) w->right->color = BLACK;
                    if (parent) rotateLeft(parent);
                    x = root;
                    break;
                }
            }
            else {
                Node* w = parent ? parent->left : nullptr;

                if (getColor(w) == RED) {
                    w->color = BLACK;
                    parent->color = RED;
                    rotateRight(parent);
                    w = parent->left;
                }

                if (getColor(w ? w->right : nullptr) == BLACK &&
                    getColor(w ? w->left : nullptr) == BLACK) {
                    if (w) w->color = RED;
                    x = parent;
                    parent = x ? x->parent : nullptr;
                }
                else {
                    if (getColor(w ? w->left : nullptr) == BLACK) {
                        if (w && w->right) w->right->color = BLACK;
                        if (w) w->color = RED;
                        if (w) rotateLeft(w);
                        w = parent ? parent->left : nullptr;
                    }

                    if (w) w->color = parent ? parent->color : BLACK;
                    if (parent) parent->color = BLACK;
                    if (w && w->left) w->left->color = BLACK;
                    if (parent) rotateRight(parent);
                    x = root;
                    break;
                }
            }
        }

        if (x) x->color = BLACK;
    }

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    class iterator {
    private:
        Node* node = nullptr;

    public:
        iterator() = default;
        explicit iterator(Node* n) : node(n) {}

        std::pair<Key, Value>& operator*() const {
            if (!node) throw std::out_of_range("iterator is end()");
            return node->data;
        }

        std::pair<Key, Value>* operator->() const {
            if (!node) throw std::out_of_range("iterator is end()");
            return &(node->data);
        }

        iterator& operator++() {
            node = successor(node);
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator& operator--() {
            node = predecessor(node);
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return node == other.node;
        }

        bool operator!=(const iterator& other) const {
            return node != other.node;
        }

        friend class rb_tree;
    };

    rb_tree() = default;

    ~rb_tree() {
        clear();
    }

    iterator insert(const Key& key, const Value& value) {
        Node* y = nullptr;
        Node* x = root;

        while (x) {
            y = x;
            if (key < x->data.first) {
                x = x->left;
            }
            else if (key > x->data.first) {
                x = x->right;
            }
            else {
                return end();
            }
        }

        Node* z = new Node(key, value);
        z->parent = y;

        if (!y) {
            root = z;
        }
        else if (key < y->data.first) {
            y->left = z;
        }
        else {
            y->right = z;
        }

        ++count;
        fixInsert(z);
        return iterator(z);
    }

    iterator find(const Key& key) {
        return iterator(findNode(key));
    }

    iterator find(const Key& key) const {
        return iterator(findNode(key));
    }

    iterator erase(const Key& key) {
        Node* z = findNode(key);
        if (!z) return end();

        iterator next = iterator(successor(z));

        Node* y = z;
        Node* x = nullptr;
        Node* xParent = nullptr;
        Color yOriginalColor = y->color;

        if (!z->left) {
            x = z->right;
            xParent = z->parent;
            transplant(z, z->right);
        }
        else if (!z->right) {
            x = z->left;
            xParent = z->parent;
            transplant(z, z->left);
        }
        else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                xParent = y;
                if (x) x->parent = y;
            }
            else {
                xParent = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        --count;

        if (yOriginalColor == BLACK) {
            fixErase(x, xParent);
        }

        return next;
    }

    iterator begin() {
        return iterator(minimum(root));
    }

    iterator end() {
        return iterator(nullptr);
    }

    int size() const {
        return static_cast<int>(count);
    }

    bool empty() const {
        return count == 0;
    }

    void clear() {
        clear(root);
        root = nullptr;
        count = 0;
    }
};