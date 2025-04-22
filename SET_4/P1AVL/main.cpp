//Мусаев БПИ234.
#include <iostream>
#include <algorithm>
using namespace std;

// Структура узла дерева.
struct Node {
    int value;
    Node* left;
    Node* right;

    Node(int val) : value(val), left(nullptr), right(nullptr) {}
};

// Бинарное дерево поиска.
class BinaryTree {
public:
    BinaryTree() : root(nullptr) {}

    // Метод вставки нового узла.
    void insert(int value) {
        root = insertN(root, value);
    }

    // Проверка сбалансированности дерева.
    bool isBalanced() {
        return checkB(root) != -1;
    }

private:
    Node* root;

    // Вставка узла.
    Node* insertN(Node* node, int value) {
        if (node == nullptr) {
            return new Node(value);
        }
        if (value < node->value) {
            node->left = insertN(node->left, value);
        } else if (value > node->value) {
            node->right = insertN(node->right, value);
        }
        return node;
    }

    // Проверка AVL-сбалансированности.
    int checkB(Node* node) {
        if (node == nullptr) {
            return 0;
        }

        int leftH = checkB(node->left);
        if (leftH == -1) return -1; // Если левое поддерево несбалансировано.

        int rightH = checkB(node->right);
        if (rightH == -1) return -1; // Если правое поддерево несбалансировано.

        if (abs(leftH - rightH) > 1) {
            return -1; // Если текущий узел несбалансирован.
        }

        return max(leftH, rightH) + 1; // Высота текущего поддерева.
    }
};

int main() {
    BinaryTree tree;
    int value;

    while (cin >> value && value != 0) {
        tree.insert(value);
    }
    if (tree.isBalanced()) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

    return 0;
}