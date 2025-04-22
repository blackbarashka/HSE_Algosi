//Musaev БПИ234.
#include "splay-tree.h"
#include <algorithm>
// Конструктор Node.
Node::Node(int key, Node *parent) : key(key), left(nullptr), right(nullptr), parent(parent) {}

// Конструктор SplayTree.
SplayTree::SplayTree() : root(nullptr) {}

// Деструктор SplayTree.
SplayTree::~SplayTree() {
    clear(root);
}

// Рекурсивная функция для очистки дерева.
void SplayTree::clear(Node *nd) {
    if (nd) {
        clear(nd->left);
        clear(nd->right);
        delete nd;
    }
}

// Левый поворот вокруг узла x.
void SplayTree::rotateLeft(Node *x) {
    Node *y = x->right;
    if (!y) return; // Невозможно выполнить левый поворот без правого дочернего узла.

    // Перемещение поддерева y->left в y->parent->right.
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }

    // Обновление родительских связей.
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

    // Завершение поворота.
    y->left = x;
    x->parent = y;
}

// Правый поворот вокруг узла x.
void SplayTree::rotateRight(Node *x) {
    Node *y = x->left;
    if (!y) return; // Невозможно выполнить правый поворот без левого дочернего узла.

    // Перемещение поддерева y->right в y->parent->left.
    x->left = y->right;
    if (y->right) {
        y->right->parent = x;
    }

    // Обновление родительских связей.
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

    // Завершение поворота.
    y->right = x;
    x->parent = y;
}

// Итеративный метод вставки ключа в дерево без балансировки.
void SplayTree::insert(int key) {
    if (!root) {
        root = new Node(key, nullptr);
        return;
    }

    Node *current = root;
    Node *parent = nullptr;

    while (current) {
        if (key == current->key) {
            // Ключ уже существует, ничего не делаем.
            return;
        }
        parent = current;
        if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    // Вставка нового узла как дочернего для parent.
    Node *newNode = new Node(key, parent);
    if (key < parent->key) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }
}

// Итеративный метод поиска ключа в дереве.
Node* SplayTree::find(int key) const {
    Node *current = root;
    while (current) {
        if (key == current->key) {
            return current;
        }
        if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return nullptr;
}

int SplayTree::splay(Node *nd) {
    if (!nd)
        return 0;

    int rotations = 0;

    while (nd->parent) {
        if (!nd->parent->parent) {
            if (nd->parent->left == nd)
                rotateRight(nd->parent);
            else
                rotateLeft(nd->parent);
            rotations++;
        } else if (nd->parent->left == nd && nd->parent->parent->left == nd->parent) {
            rotateRight(nd->parent->parent);
            rotateRight(nd->parent);
            rotations += 2;
        } else if (nd->parent->right == nd && nd->parent->parent->right == nd->parent) {
            rotateLeft(nd->parent->parent);
            rotateLeft(nd->parent);
            rotations += 2;
        } else {
            if (nd->parent->left == nd && nd->parent->parent->right == nd->parent) {
                rotateRight(nd->parent);
                rotateLeft(nd->parent);
            } else {
                rotateLeft(nd->parent);
                rotateRight(nd->parent);
            }
            rotations++;
        }
    }

    root = nd;
    return rotations;
}

// Публичный метод getHeight.
int SplayTree::getHeight() const {
    return getHeight(root);
}

// Рекурсивный вспомогательный метод для вычисления высоты дерева.
int SplayTree::getHeight(Node *nd) const {
    if (!nd) return -1;
    return 1 + std::max(getHeight(nd->left), getHeight(nd->right));
}