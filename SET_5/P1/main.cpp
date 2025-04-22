#include <vector>
#include <list>
#include <functional>
#include <stdexcept>
#include <iostream>

// Кастомный хеш-функтор, который хеширует все ключи в 0

template<typename KeyType, typename ValueType, typename Func = std::hash<KeyType>>
class HashTable {
public:
    struct Node {
        KeyType key;
        ValueType value;
        Node* next = nullptr;  // Указатель на следующий элемент в цепочке

        Node(const KeyType& key_, const ValueType& value_): key(key_), value(value_), next(nullptr) {}
    };

private:
    std::vector<Node*> table;
    size_t table_size;
    size_t table_capacity;
    double load_factor;
    Func hasher;

    // Перехеширование таблицы (увеличивает размер в 2 раза)
    void rehash() {
        size_t old_capacity = table_capacity;
        table_capacity *= 2;
        std::vector<Node*> new_table(table_capacity, nullptr);

        for (size_t i = 0; i < old_capacity; ++i) {
            Node* node = table[i];
            while (node) {
                size_t new_index = hasher(node->key) % table_capacity;
                Node* next_node = node->next;  // Сохраняем ссылку на следующий узел

                node->next = new_table[new_index];  // Переносим в новую таблицу
                new_table[new_index] = node;

                node = next_node;
            }
        }
        table.swap(new_table);
    }

public:
    // Конструкторы
    HashTable() : table_size(0), table_capacity(100), load_factor(0.5), hasher(Func()) {
        table.resize(table_capacity, nullptr);
    }

    explicit HashTable(Func hasher_) : table_size(0), table_capacity(100), load_factor(0.5), hasher(hasher_) {
        table.resize(table_capacity, nullptr);
    }

    explicit HashTable(size_t capacity_, double load_factor_ = 0.5, Func hasher_ = Func())
        : table_size(0), table_capacity(capacity_), load_factor(load_factor_), hasher(hasher_) {
        if (load_factor <= 0.0 || load_factor > 1.0) {
            load_factor = 0.5;
        }
        table.resize(table_capacity, nullptr);
    }

    ~HashTable() {
        for (size_t i = 0; i < table_capacity; ++i) {
            Node* node = table[i];
            while (node) {
                Node* temp = node;
                node = node->next;
                delete temp;
            }
        }
    }

    // Оператор []
    Node& operator[](size_t index) {
        if (index >= table_capacity) {
            throw std::out_of_range("Index out of range");
        }
        if (!table[index]) {
            throw std::runtime_error("Cell is empty");
        }
        return *table[index];  // Возвращаем ссылку на первый узел
    }

    Node at(size_t index) {
        if (index >= table_capacity) {
            throw std::out_of_range("Index out of range");
        }
        if (!table[index]) {
            throw std::runtime_error("Cell is empty");
        }
        return *table[index];  // Возвращаем копию первого узла
    }

    // Возвращает количество элементов в таблице
    [[nodiscard]] size_t size() const { return table_size; }

    // Возвращает текущую емкость таблицы
    [[nodiscard]] size_t capacity() const { return table_capacity; }

    // Вставка элемента
    void insert(const KeyType& key, const ValueType& value) {
        size_t index = hasher(key) % table_capacity;
        Node* node = table[index];

        // Проверка на существование ключа
        while (node) {
            if (node->key == key) {
                node->value = value;  // Обновление значения
                return;
            }
            node = node->next;
        }

        // Вставляем новый узел в начало цепочки
        Node* new_node = new Node(key, value);
        new_node->next = table[index];
        table[index] = new_node;
        ++table_size;

        // Проверка коэффициента заполнения
        if (static_cast<double>(table_size) / table_capacity > load_factor) {
            rehash();
        }
    }

    // Удаление элемента по ключу
    void erase(const KeyType& key) {
        size_t index = hasher(key) % table_capacity;
        Node* node = table[index];
        Node* prev = nullptr;

        while (node) {
            if (node->key == key) {
                if (prev) {
                    prev->next = node->next;
                } else {
                    table[index] = node->next;
                }
                delete node;
                --table_size;
                return;
            }
            prev = node;
            node = node->next;
        }
    }

    // Поиск элемента по ключу
    ValueType* find(const KeyType& key) {
        size_t index = hasher(key) % table_capacity;
        Node* node = table[index];

        while (node) {
            if (node->key == key) {
                return &(node->value);
            }
            node = node->next;
        }
        return nullptr;
    }
};
