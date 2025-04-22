//Musaev БПИ234.
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <climits>

using namespace std;
// Структура узла красно-черного дерева.
struct Node {
    int number;
    int key;
    int left;
    int right;
    char color;
    // Конструктор по умолчанию.
    Node() : number(0), key(0), left(0), right(0), color('B') {}
    // Пользовательский конструктор.
    Node(int num, int k, int l, int r, char c) : number(num), key(k), left(l), right(r), color(c) {}
};
// Класс, реализующий проверку красно-черного дерева.
class RedBlackTreeChecker {
private:
    unordered_map<int, Node> nodes; // Содержит узлы дерева (номер узла -> узел).
    int root; // Корень дерева.
    // Проверка свойств красно-черного дерева.
    bool validateR(int node, int& blackHeight, int minKey = INT_MIN, int maxKey = INT_MAX) {
        if (node == 0) {
            blackHeight = 1; // NIL узлы считаются черными.
            return true;
        }
        Node current = nodes[node];
        //Проверка свойства 1: Компаратор ключей (BST свойство).
        if (current.key <= minKey || current.key >= maxKey) {
            return false;
        }
        // Рекурсивно проверяем левое и правое поддерево.
        int lBH = 0, rBH = 0;
        bool iLV = validateR(current.left, lBH, minKey, current.key);
        bool isRightValid = validateR(current.right, rBH, current.key, maxKey);

        if (!iLV || !isRightValid) {
            return false;
        }

        // Проверка свойства 2: Все простые пути имеют одинаковое число черных узлов.
        if (lBH != rBH) {
            return false;
        }

        // Проверка свойства 3: Красный узел не должен иметь красного родителя.
        if (current.color == 'R') {
            if ((current.left != 0 && nodes[current.left].color == 'R') || (current.right != 0 && nodes[current.right].color == 'R')) {
                return false;
            }
            blackHeight = lBH; // Красный узел не увеличивает черную высоту.
        } else {
            blackHeight = lBH + 1; // Черный узел увеличивает черную высоту.
        }

        return true;
    }

public:
    RedBlackTreeChecker(int root_) : root(root_) {}

    // Добавление узла в дерево.
    void addNode(int number, int key, int left, int right, char color) {
        nodes[number] = Node(number, key, left, right, color);
    }

    // Проверка дерева на соответствие красно-черным требованиям.
    bool isValid() {
        if (root == 0) return false; // Если нет дерева, то оно не соответствует (n=0).
        if (nodes[root].color != 'B') return false; // Корень дерева должен быть черным.

        int blackHeight = 0;
        return validateR(root, blackHeight);
    }
};

int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, root;
    // Ввод количества узлов и корня.
    cin >> n;
    if (n == 0) {
        cout << "NO" << endl;
        return 0; // Если дерево пустое, то оно не является красно-черным.
    }
    cin >> root;
    RedBlackTreeChecker treeCj(root);
    // Ввод данных о каждом узле.
    for (int i = 0; i < n; ++i) {
        int number, key, left, right;
        char color;
        string leftStr, rightStr;

        cin >> number >> key >> leftStr >> rightStr >> color;

        left = (leftStr == "null") ? 0 : stoi(leftStr);
        right = (rightStr == "null") ? 0 : stoi(rightStr);

        treeCj.addNode(number, key, left, right, color);
    }
    // Вывод результата проверки.
    if (treeCj.isValid()) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

    return 0;
}