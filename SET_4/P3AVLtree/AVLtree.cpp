//Musaev БПИ234.
#include "avl_tree.h"
#include <algorithm>
Node::Node(int value) : value(value), left(nullptr), right(nullptr), height(1) {
}

Node::~Node() {}

AVLTree::AVLTree() : root_(nullptr), size_(0) {}

AVLTree::AVLTree(int value) : root_(new Node(value)), size_(0) {}

int AVLTree::getHeight() {return root_->height;}

void AVLTree::insert(int value) {
    root_ = insertNode(root_, value);
}

void AVLTree::erase(int value) {
    if (find(value) != nullptr) {
        root_ = removeNode(root_, value);
    }
}

int *AVLTree::find(int value) {
    Node* node = findNode(root_, value);
    if (node != nullptr) {
        return &(node->value);
    } else {
        return nullptr;
    }}

int *AVLTree::traversal() {
     int* arrayss = new int[size_];
    int index = 0;
    traversalInternal(root_, arrayss, &index);
    return arrayss;
}

int *AVLTree::lowerBound(int value) {
     Node* node = lowerBoundInternal(root_, value);
    if (node != nullptr) {
        return &(node->value);
    } else {
        return nullptr;
    }
}

bool AVLTree::empty() {
    return root_ == nullptr;
}

Node *AVLTree::getRoot() {
    return root_;
}

int AVLTree::getSize() {
    return  size_;
}

AVLTree::~AVLTree() {

}

int AVLTree::getNodeHeight(Node *node) {
    if (node == nullptr) return 0;
    return node->height;
}

int AVLTree::balanceFactor(Node *node) {
    return getNodeHeight(node->right) - getNodeHeight(node->left);
}

void AVLTree::balanceHeight(Node *node) {
    int leftHeight = getNodeHeight(node->left);
    int rightHeight = getNodeHeight(node->right);
    node->height = std::max(leftHeight, rightHeight) + 1;
}

Node *AVLTree::rotateRight(Node *node) {
    Node* leftChild = node->left;
    node->left = leftChild->right;
    leftChild->right = node;

    balanceHeight(node);
    balanceHeight(leftChild);

    return leftChild;
}

Node *AVLTree::rotateLeft(Node *node) {
    Node* rightChild = node->right;
    node->right = rightChild->left;
    rightChild->left = node;

    balanceHeight(node);
    balanceHeight(rightChild);

    return rightChild;
}

Node *AVLTree::balanceNode(Node *node) {
    balanceHeight(node);

    int bf = balanceFactor(node);

    // Правый случай.
    if (bf == 2) {
        if (balanceFactor(node->right) < 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    // Левый случай.
    if (bf == -2) {
        if (balanceFactor(node->left) > 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    return node; // Узел сбалансирован.
}

Node *AVLTree::insertNode(Node *node, int value) {
    if (node == nullptr) {
        size_++;
        return new Node(value);
    }
    if (value < node->value) {
        node->left = insertNode(node->left, value);
    } else if (value > node->value) {
        node->right = insertNode(node->right, value);
    } else {
        // Значение уже существует в дереве.
        return node;
    }
    return balanceNode(node);

}
//Минимальное это самый левый.
Node *AVLTree::findMinNode(Node *node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

Node *AVLTree::removeMinNode(Node *node) {
    if (node->left == nullptr) {
        // Если у узла нет левого ребенка, то сам узел является минимальным. Возвращаем правого ребенка, который будет новым поддеревом после удаления минимального узла.
        return node->right;
    }
    // Рекурсивно идем в левое поддерево.
    node->left = removeMinNode(node->left);
    // После удаления минимального элемента балансируем узел.
    return balanceNode(node);

}

Node *AVLTree::removeNode(Node *node, int value) {
    if (node == nullptr) return nullptr;

    if (value < node->value) {
        node->left = removeNode(node->left, value);
    } else if (value > node->value) {
        node->right = removeNode(node->right, value);
    } else {
        // Найден узел для удаления.
        size_--;
        Node* leftChild = node->left;
        Node* rightChild = node->right;
        delete node;

        if (rightChild == nullptr) {
            return leftChild;
        }
        // Находим минимальный узел в правом поддереве.
        Node* minNode = findMinNode(rightChild);
        // Заменяем удаленный узел минимальным узлом из правого поддерева.
        minNode->right = removeMinNode(rightChild);
        minNode->left = leftChild;

        return balanceNode(minNode);
    }
    return balanceNode(node);
}

Node *AVLTree::findNode(Node *node, int value) {
    if (node == nullptr) return nullptr;
    if (value == node->value) {
        return node;
    } else if (value < node->value) {
        return findNode(node->left, value);
    } else {
        return findNode(node->right, value);
    }
}

void AVLTree::traversalInternal(Node *node, int *arrayss, int *index) {
     if (node == nullptr) return;
    traversalInternal(node->left, arrayss, index);
    arrayss[(*index)++] = node->value;
    traversalInternal(node->right, arrayss, index);
}

Node *AVLTree::lowerBoundInternal(Node *current, int value) const {
    Node* result = nullptr;
    while (current != nullptr) {
        if (current->value >= value) {
            result = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return result;
}
