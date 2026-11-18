#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype> // Для работы с символами (toupper, tolower)

using namespace std;

struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;

    // Получение высоты узла (для пустого узла высота 0)
    int height(AVLNode* node) {
        return node ? node->height : 0;
    }

    // Вычисление фактора баланса (разница высот)
    int balanceFactor(AVLNode* node) {
        return height(node->left) - height(node->right);
    }

    // Обновление высоты узла
    void updateHeight(AVLNode* node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    // Правый поворот (Right Rotation)
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Переставляем указатели
        x->right = y;
        y->left = T2;

        // Обновляем высоты
        updateHeight(y);
        updateHeight(x);

        return x; // Новый корень поддерева
    }

    // Левый поворот (Left Rotation)
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Переставляем указатели
        y->left = x;
        x->right = T2;

        // Обновляем высоты
        updateHeight(x);
        updateHeight(y);

        return y; // Новый корень поддерева
    }

    // Балансировка узла (вызывается после вставки или удаления)
    AVLNode* balance(AVLNode* node) {
        if (node == nullptr) return nullptr;

        updateHeight(node);
        int bf = balanceFactor(node);

        // Левое поддерево выше
        if (bf > 1) {
            // Если левый потомок имеет отрицательный баланс — левый-правый поворот
            if (balanceFactor(node->left) < 0) {
                node->left = leftRotate(node->left);
            }
            // Иначе — просто правый поворот
            return rightRotate(node);
        }
        // Правое поддерево выше
        else if (bf < -1) {
            // Если правый потомок имеет положительный баланс — правый-левый поворот
            if (balanceFactor(node->right) > 0) {
                node->right = rightRotate(node->right);
            }
            // Иначе — просто левый поворот
            return leftRotate(node);
        }

        return node; // Баланс в норме
    }

    // Рекурсивная вставка
    AVLNode* insert(AVLNode* node, int key) {
        if (node == nullptr) return new AVLNode(key);

        if (key < node->key) {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key);
        }
        else {
            return node; // Дубликаты не вставляем
        }

        return balance(node); // Балансируем после вставки
    }

    // Поиск узла
    AVLNode* search(AVLNode* node, int key) {
        if (node == nullptr || node->key == key) return node;
        if (key < node->key) return search(node->left, key);
        return search(node->right, key);
    }

    // Поиск минимального узла (для удаления)
    AVLNode* findMin(AVLNode* node) {
        while (node->left != nullptr) node = node->left;
        return node;
    }

    // Рекурсивное удаление
    AVLNode* remove(AVLNode* node, int key) {
        if (node == nullptr) return nullptr;

        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else {
            // Нашли узел для удаления
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr) {
                AVLNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                AVLNode* temp = node->left;
                delete node;
                return temp;
            }
            else {
                // Два потомка: заменяем на минимум из правого поддерева
                AVLNode* minNode = findMin(node->right);
                node->key = minNode->key;
                node->right = remove(node->right, minNode->key);
            }
        }
        return balance(node); // Балансируем после удаления
    }

    // Визуализация дерева (повернуто на 90 градусов)
    void printTree(AVLNode* node, int level = 0) {
        if (node == nullptr) return;
        printTree(node->right, level + 1);
        for (int i = 0; i < level; i++) cout << "    ";
        cout << node->key << " (h=" << node->height << ", bf=" << balanceFactor(node) << ")" << endl;
        printTree(node->left, level + 1);
    }

    // Удаление всего дерева
    void destroy(AVLNode* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    void insert(int key) { root = insert(root, key); }
    bool search(int key) { return search(root, key) != nullptr; }
    void remove(int key) { root = remove(root, key); }

    // Публичный метод для визуализации
    void print() {
        cout << "Визуализация AVL-дерева:" << endl;
        printTree(root);
        cout << endl;
    }
};

class Trie {
private:
    struct TrieNode {
        TrieNode* children[52]; // 26 строчных (a-z) + 26 заглавных (A-Z)
        bool isEndOfWord;

        TrieNode() : isEndOfWord(false) {
            for (int i = 0; i < 52; i++) {
                children[i] = nullptr;
            }
        }
    };

    TrieNode* root;

    // Функция для получения индекса в массиве children
    // Строчные буквы: 0-25, Заглавные: 26-51
    int getIndex(char ch) {
        if (ch >= 'a' && ch <= 'z') return ch - 'a';
        if (ch >= 'A' && ch <= 'Z') return (ch - 'A') + 26;
        return -1; // Не буква
    }

    // Рекурсивное удаление всех узлов
    void destroy(TrieNode* node) {
        if (node == nullptr) return;
        for (int i = 0; i < 52; i++) {
            destroy(node->children[i]);
        }
        delete node;
    }

    // Вспомогательная функция для удаления слова
    bool removeHelper(TrieNode* node, const string& word, int depth) {
        if (node == nullptr) return false;

        // Если дошли до конца слова
        if (depth == word.length()) {
            if (!node->isEndOfWord) return false; // Слова не было
            node->isEndOfWord = false;            // Помечаем, что слово удалено

            // Если у узла нет потомков, его можно удалить
            for (int i = 0; i < 52; i++) {
                if (node->children[i] != nullptr) return false;
            }
            return true; // Узел можно удалить
        }

        int index = getIndex(word[depth]);
        if (index == -1) return false;

        bool shouldDeleteChild = removeHelper(node->children[index], word, depth + 1);

        if (shouldDeleteChild) {
            delete node->children[index];
            node->children[index] = nullptr;

            // Проверяем, можно ли удалить и текущий узел
            if (!node->isEndOfWord) {
                for (int i = 0; i < 52; i++) {
                    if (node->children[i] != nullptr) return false;
                }
                return true;
            }
        }
        return false;
    }

public:
    Trie() { root = new TrieNode(); }
    ~Trie() { destroy(root); }

    // Вставка слова (с поддержкой регистра)
    void insert(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = getIndex(ch);
            if (index == -1) continue; // Пропускаем не-буквы
            if (current->children[index] == nullptr) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEndOfWord = true;
    }

    // Поиск слова
    bool search(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            int index = getIndex(ch);
            if (index == -1) return false;
            if (current->children[index] == nullptr) return false;
            current = current->children[index];
        }
        return current->isEndOfWord;
    }

    // Удаление слова
    void remove(const string& word) {
        removeHelper(root, word, 0);
    }
};


int main() {
    setlocale(LC_ALL, "ru");

    // --- ЧАСТЬ 1: AVL-дерево ---
    cout << "=== ЧАСТЬ 1: AVL-дерево ===" << endl;
    AVLTree avl;
    avl.insert(50);
    avl.insert(30);
    avl.insert(70);
    avl.insert(20);
    avl.insert(40);
    avl.insert(60);
    avl.insert(80);
    avl.insert(10); // Вызовет дисбаланс и правый поворот

    avl.print();

    cout << "Поиск 40: " << (avl.search(40) ? "найден" : "не найден") << endl;
    cout << "Поиск 100: " << (avl.search(100) ? "найден" : "не найден") << endl;

    avl.remove(30);
    cout << "\nПосле удаления 30:" << endl;
    avl.print();


    // --- ЧАСТЬ 2: Trie ---
    cout << "=== ЧАСТЬ 2: Trie (с поддержкой регистра) ===" << endl;
    Trie trie;
    trie.insert("Apple");
    trie.insert("apple");
    trie.insert("Banana");
    trie.insert("Car");
    trie.insert("cat");

    cout << "Поиск 'Apple': " << (trie.search("Apple") ? "найдено" : "не найдено") << endl;
    cout << "Поиск 'apple': " << (trie.search("apple") ? "найдено" : "не найдено") << endl;
    cout << "Поиск 'APPLE': " << (trie.search("APPLE") ? "найдено" : "не найдено") << endl; // Не должно найтись
    cout << "Поиск 'Banana': " << (trie.search("Banana") ? "найдено" : "не найдено") << endl;

    cout << "\nУдаляем 'apple'..." << endl;
    trie.remove("apple");
    cout << "Поиск 'apple' после удаления: " << (trie.search("apple") ? "найдено" : "не найдено") << endl;
    cout << "Поиск 'Apple' (осталось): " << (trie.search("Apple") ? "найдено" : "не найдено") << endl;

    return 0;
}