//Musaev БПИ234.
#include "btree.h"
#include <algorithm>
#include <numeric>
#include <cstdint>

// Рекурсивная функция для очистки памяти, занятой деревом.
void clear(Node* node) {
    if (!node) return;
    for (Node* rebenok : node->children)
        clear(rebenok); // Рекурсивно очищаем потомков.
    delete node; // Удаляем текущий узел.
}

// Функция для разделения заполненного дочернего узла.
void splitChild(Node* node, int i, Node* rebenok) {
    // Создаем новый узел, который будет правым потомком после разделения.
    Node* newChild = new Node(rebenok->t);
    newChild->isLeaf = rebenok->isLeaf;

    // Копируем последние t ключей из rebenok в newChild.
    newChild->key.assign(rebenok->key.begin() + node->t, rebenok->key.end());
    // Уменьшаем количество ключей в rebenok до t - 1.
    rebenok->key.resize(node->t - 1);

    // Если rebenok не является листом, копируем соответствующие потомки.
    if (!rebenok->isLeaf) {
        newChild->children.assign(rebenok->children.begin() + node->t, rebenok->children.end());
        // Обновляем родителя для новых потомков.
        for (Node* ch : newChild->children)
            ch->parent = newChild;
        // Уменьшаем количество потомков в rebenok до t.
        rebenok->children.resize(node->t);
    }

    // Вставляем нового потомка в список потомков текущего узла.
    node->children.insert(node->children.begin() + i + 1, newChild);
    // Перемещаем средний ключ из rebenok в текущий узел.
    node->key.insert(node->key.begin() + i, rebenok->key[node->t - 1]);
}

// Функция для вставки ключа в узел, который не заполнен.
int fuuil(Node* node, int key) {
    int i = node->key.size() - 1;
    int result = 0;

    // Если узел является листом, вставляем ключ непосредственно.
    if (node->isLeaf) {
        node->key.push_back(0); // Резервируем место для нового ключа.
        // Находим позицию для вставки ключа.
        while (i >= 0 && node->key[i] > key)
        {
            node->key[i + 1] = node->key[i];
            i--;
        }
        node->key[i + 1] = key; // Вставляем ключ.
    } else {
        // Находим дочерний узел, в который следует вставить ключ.
        while (i >= 0 && node->key[i] > key)
        {
            i--;
        }
        i++;
        // Проверяем, заполнен ли выбранный дочерний узел.
        if (node->children[i]->key.size() == 2 * node->t - 1) {
            // Разделяем заполненный дочерний узел.
            splitChild(node, i, node->children[i]);
            result++;
            // Определяем, в какой из двух разделенных узлов следует продолжить вставку.
            if (node->key[i] < key)
                {
                i++;
                }
        }
        // Рекурсивно вставляем ключ в подходящий дочерний узел.
        result += fuuil(node->children[i], key);
    }

    return result;
}

// Функция для расчета суммы ключей всех листовых узлов.
int64_t sumLeafNodes(const Node* node) {
    if (node->isLeaf) {
        // Если узел является листом, суммируем его ключи.
        return accumulate(node->key.begin(), node->key.end(), int64_t(0));
    }

    // Если узел не лист, рекурсивно суммируем ключи всех листовых потомков.
    return accumulate(node->children.begin(), node->children.end(), int64_t(0),[](int64_t sum, const Node* rebenok) {return sum + sumLeafNodes(rebenok);});
}

// Конструктор узла.
Node::Node(int t) : t(t), isLeaf(true), parent(nullptr) {}

// Метод для получения количества ключей в дереве.
size_t BTree::size() const {
    return size_;
}

// Метод для получения суммы ключей всех листовых узлов.
int64_t BTree::sum() const {
    return root ? sumLeafNodes(root) : 0;
}

// Конструктор B-дерева.
BTree::BTree(int t) : root(nullptr), t_(t), size_(0) {}

// Деструктор B-дерева.
BTree::~BTree() {
    clear(root); // Очищаем память начиная с корня.
}

// Метод для вставки ключа в B-дерево.
void BTree::insert(int key) {
    // Если дерево пусто, создаем новый корневой узел и вставляем ключ.
    if (!root) {
        root = new Node(t_);
        root->key.push_back(key);
        size_ = 1;
        return;
    }

    Node* current = root;
    // Ищем позицию для вставки ключа, избегая дубликатов.
    while (current) {
        if (std::find(current->key.begin(), current->key.end(), key) != current->key.end())
            {
            return; // Ключ уже существует, не вставляем.
            }

        if (current->isLeaf){
            break; // Достигли листового узла.
            }

        size_t i = 0;
        while (i < current->key.size() && key > current->key[i]) {
            i++;
            }
        current = current->children[i]; // Переходим к соответствующему потомку.
    }

    // Если корневой узел заполнен, необходимо его разделить.
    if (root->key.size() == 2 * t_ - 1) {
        Node* newRoot = new Node(t_);
        newRoot->isLeaf = false;
        newRoot->children.push_back(root);
        splitChild(newRoot, 0, root); // Разделяем текущий корень.
        size_ += 2; // Увеличиваем размер дерева на 2 (добавлены два ключа после разделения).
        root = newRoot; // Обновляем корень дерева.
    }

    // Вставляем ключ в корректный узел и обновляем размер дерева.
    size_ += fuuil(root, key);
}