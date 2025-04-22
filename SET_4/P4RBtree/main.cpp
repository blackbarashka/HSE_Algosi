//Musaev БПИ234.
#include "rbtree.h"
#include <initializer_list>
#include <functional>
#include <algorithm>
#include <cstddef>

// Конструктор узла
Node::Node(int key)
    : key(key), height(1), size(1), left(nullptr), right(nullptr), parent(nullptr), color(Color::RED) {}

// Вспомогательные функции объявлены в анонимном пространстве имён
namespace {
    // Обновление атрибутов size и height узла
    void upd(Node* node) {
        if (node) {
            node->size = 1;
            node->height = 1;
            if (node->left) {
                node->size += node->left->size;
                node->height = std::max(node->height, node->left->height + 1);
            }
            if (node->right) {
                node->size += node->right->size;
                node->height = std::max(node->height, node->right->height + 1);
            }
        }
    }

    // Левое вращение
    void leftRotate(RBTree& tree, Node* x) {
        Node* y = x->right;
        if (!y) return; // Без вращения, если y == nullptr

        x->right = y->left;
        if (y->left)
            y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent)
            tree.root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;

        upd(x);
        upd(y);
    }

    // Правое вращение
    void rightRotate(RBTree& tree, Node* y) {
        Node* x = y->left;
        if (!x) return; // Без вращения, если x == nullptr

        y->left = x->right;
        if (x->right)
            x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent)
            tree.root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        x->right = y;
        y->parent = x;

        upd(y);
        upd(x);
    }

    // Восстановление свойств после вставк.
    void iFU(RBTree& tree, Node* z) {
        while (z->parent && z->parent->color == Color::RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y && y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(tree, z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rightRotate(tree, z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y && y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(tree, z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    leftRotate(tree, z->parent->parent);
                }
            }
        }
        tree.root->color = Color::BLACK;
    }

    // Поиск минимального узла в поддереве.
    Node* minimum(Node* node) {
        while (node->left)
            node = node->left;
        return node;
    }

    // Трансплантация поддеревьев.
    void transplant(RBTree& tree, Node* u, Node* v) {
        if (!u->parent)
            tree.root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v)
            v->parent = u->parent;
    }

    // Восстановление свойств после удаления.
    void eraseFixUp(RBTree& tree, Node* x) {
        while (x != tree.root && (!x || x->color == Color::BLACK)) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w && w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    leftRotate(tree, x->parent);
                    w = x->parent->right;
                }
                if ((!w->left || w->left->color == Color::BLACK) &&
                    (!w->right || w->right->color == Color::BLACK)) {
                    if(w) w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (!w->right || w->right->color == Color::BLACK) {
                        if(w->left) w->left->color = Color::BLACK;
                        if(w) w->color = Color::RED;
                        rightRotate(tree, w);
                        w = x->parent->right;
                    }
                    if(w){
                        w->color = x->parent->color;
                        x->parent->color = Color::BLACK;
                        if(w->right) w->right->color = Color::BLACK;
                        leftRotate(tree, x->parent);
                    }
                    x = tree.root;
                }
            } else {
                Node* w = x->parent->left;
                if (w && w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rightRotate(tree, x->parent);
                    w = x->parent->left;
                }
                if ((!w->right || w->right->color == Color::BLACK) &&
                    (!w->left || w->left->color == Color::BLACK)) {
                    if(w) w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (!w->left || w->left->color == Color::BLACK) {
                        if(w->right) w->right->color = Color::BLACK;
                        if(w) w->color = Color::RED;
                        leftRotate(tree, w);
                        w = x->parent->left;
                    }
                    if(w){
                        w->color = x->parent->color;
                        x->parent->color = Color::BLACK;
                        if(w->left) w->left->color = Color::BLACK;
                        rightRotate(tree, x->parent);
                    }
                    x = tree.root;
                }
            }
        }
        if(x) x->color = Color::BLACK;
    }
}

// Конструктор по умолчанию.
RBTree::RBTree() : root(nullptr) {}

// Конструктор с инициализатором списка.
RBTree::RBTree(std::initializer_list<int> list) : root(nullptr) {
    for (const int& key : list) {
        insert(key);
    }
}

// Деструктор.
RBTree::~RBTree() {
    // Рекурсивно удалить все узлы.
    std::function<void(Node*)> deleteNodes = [&](Node* node) {
        if (node) {
            deleteNodes(node->left);
            deleteNodes(node->right);
            delete node;
        }
    };
    deleteNodes(root);
}

// Метод вставки ключа.
void RBTree::insert(int key) {
    Node* z = new Node(key);
    Node* y = nullptr;
    Node* x = root;

    // Поиск позиции для вставки.
    while (x != nullptr) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else if (z->key > x->key)
            x = x->right;
        else {
            // Если ключ уже существует, не вставляем дубликат.
            delete z;
            return;
        }
    }

    z->parent = y;
    if (!y)
        root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    // Восстановление свойств красно-черного дерева.
    iFU(*this, z);

    // Обновление размеров и высот по пути к корню.
    Node* current = z;
    while (current) {
        upd(current);
        current = current->parent;
    }
}

// Метод поиска узла по ключу.
int* RBTree::find(int key) {
    Node* current = root;
    while (current) {
        if (key == current->key)
            return &(current->key);
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

// Метод возвращает размер дерева.
int RBTree::size() const {
    return root ? root->size : 0;
}

// Метод поиска нижней границы.
int* RBTree::lowerBound(int key) {
    Node* current = root;
    Node* result = nullptr;
    while (current) {
        if (current->key >= key) {
            result = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return result ? &(result->key) : nullptr;
}

// Метод проверки на пустоту.
bool RBTree::empty() const {
    return root == nullptr;
}

// Метод получения высоты дерева.
int RBTree::height() const {
    return root ? root->height : 0;
}

// Метод удаления узла по ключу.
void RBTree::erase(int key) {
    Node* z = root;
    // Поиск узла с ключом.
    while (z) {
        if (key == z->key)
            break;
        else if (key < z->key)
            z = z->left;
        else
            z = z->right;
    }
    if (!z)
        return; // Ключ не найден.

    Node* y = z;
    Color y_original_color = y->color;
    Node* x = nullptr;

    if (!z->left) {
        x = z->right;
        transplant(*this, z, z->right);
    } else if (!z->right) {
        x = z->left;
        transplant(*this, z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x)
                x->parent = y;
        } else {
            transplant(*this, y, y->right);
            y->right = z->right;
            if (y->right)
                y->right->parent = y;
        }
        transplant(*this, z, y);
        y->left = z->left;
        if (y->left)
            y->left->parent = y;
        y->color = z->color;
        upd(y);
    }

    // Обновление размеров и высот по пути к корню.
    Node* current = y ? y->parent : nullptr;
    while (current) {
        upd(current);
        current = current->parent;
    }

    delete z;

    if (y_original_color == Color::BLACK && x)
        eraseFixUp(*this, x);
    else if (y_original_color == Color::BLACK && !x && current)
        eraseFixUp(*this, current);
}
