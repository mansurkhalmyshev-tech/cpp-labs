#include <iostream>
#include <chrono>
#include <stdexcept>
#include <string>

using namespace std;
using namespace std::chrono;

// =========================================================================
// БАЗОВЫЙ УРОВЕНЬ
// =========================================================================

// Шаблонная функция согласно варианту 10
// Назначение: Сравнивает два значения на равенство
// Сложность: O(1)
template <typename T>
bool isEqual(T a, T b) {
    return a == b;
}

// =========================================================================
// СРЕДНИЙ УРОВЕНЬ
// Шаблонный класс Deque (Дек) - двусторонняя очередь
// =========================================================================

template <typename T>
class Deque {
private:
    T* data;
    int capacity;
    int frontIndex;
    int backIndex;
    int count;

    // Вспомогательный метод для расширения массива
    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];

        // Копируем элементы в правильном порядке
        for (int i = 0; i < count; i++) {
            newData[i] = data[(frontIndex + i) % capacity];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
        frontIndex = 0;
        backIndex = count - 1;
    }

public:
    // Конструктор
    Deque(int initialCapacity = 10) {
        capacity = initialCapacity;
        data = new T[capacity];
        frontIndex = 0;
        backIndex = -1;
        count = 0;
    }

    // Деструктор
    ~Deque() {
        delete[] data;
    }

    // Добавление в начало
    // Сложность: Амортизированно O(1), в худшем случае O(n) при расширении
    void push_front(const T& value) {
        if (count == capacity) resize();
        frontIndex = (frontIndex - 1 + capacity) % capacity;
        data[frontIndex] = value;
        count++;
    }

    // Добавление в конец
    // Сложность: Амортизированно O(1), в худшем случае O(n) при расширении
    void push_back(const T& value) {
        if (count == capacity) resize();
        backIndex = (backIndex + 1) % capacity;
        data[backIndex] = value;
        count++;
    }

    // Удаление из начала
    // Сложность: O(1)
    void pop_front() {
        if (isEmpty()) throw out_of_range("Deque is empty");
        frontIndex = (frontIndex + 1) % capacity;
        count--;
    }

    // Удаление с конца
    // Сложность: O(1)
    void pop_back() {
        if (isEmpty()) throw out_of_range("Deque is empty");
        backIndex = (backIndex - 1 + capacity) % capacity;
        count--;
    }

    // Получение первого элемента
    // Сложность: O(1)
    T& front() {
        if (isEmpty()) throw out_of_range("Deque is empty");
        return data[frontIndex];
    }

    // Получение последнего элемента
    // Сложность: O(1)
    T& back() {
        if (isEmpty()) throw out_of_range("Deque is empty");
        return data[backIndex];
    }

    // Проверка на пустоту
    // Сложность: O(1)
    bool isEmpty() const {
        return count == 0;
    }

    // Получение размера
    // Сложность: O(1)
    int size() const {
        return count;
    }
};

// =========================================================================
// ЭКСПЕРИМЕНТАЛЬНОЕ СРАВНЕНИЕ (Оценка производительности)
// =========================================================================

// Функция для измерения времени
template <typename Func>
long long measureTime(Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "=== ТЕСТ БАЗОВОГО УРОВНЯ (Шаблонная функция isEqual) ===" << endl;
    cout << "Сравнение 5 и 5: " << (isEqual(5, 5) ? "true" : "false") << endl;
    cout << "Сравнение 3.14 и 2.71: " << (isEqual(3.14, 2.71) ? "true" : "false") << endl;
    cout << "Сравнение 'A' и 'A': " << (isEqual('A', 'A') ? "true" : "false") << endl;
    cout << "Сравнение 'Hello' и 'World': " << (isEqual(string("Hello"), string("World")) ? "true" : "false") << endl;
    cout << endl;

    cout << "=== ТЕСТ СРЕДНЕГО УРОВНЯ (Шаблонный класс Deque) ===" << endl;
    Deque<int> myDeque(4);

    myDeque.push_back(10);
    myDeque.push_front(20);
    myDeque.push_back(30);
    myDeque.push_front(40);

    cout << "Размер дека: " << myDeque.size() << endl;
    cout << "Первый элемент (front): " << myDeque.front() << endl;
    cout << "Последний элемент (back): " << myDeque.back() << endl;

    myDeque.pop_front();
    myDeque.pop_back();

    cout << "После удаления крайних элементов:" << endl;
    cout << "Первый элемент: " << myDeque.front() << endl;
    cout << "Последний элемент: " << myDeque.back() << endl;
    cout << endl;

    cout << "=== ЭКСПЕРИМЕНТАЛЬНОЕ СРАВНЕНИЕ ===" << endl;
    // Сравниваем добавление в конец и добавление в начало
    const int N = 100000;

    long long timePushBack = measureTime([&]() {
        Deque<int> d(N);
        for (int i = 0; i < N; i++) d.push_back(i);
        });

    long long timePushFront = measureTime([&]() {
        Deque<int> d(N);
        for (int i = 0; i < N; i++) d.push_front(i);
        });

    cout << "Время push_back " << N << " элементов: " << timePushBack << " мкс" << endl;
    cout << "Время push_front " << N << " элементов: " << timePushFront << " мкс" << endl;
    cout << "Вывод: Обе операции работают за амортизированное O(1), время должно быть примерно одинаковым." << endl;

    return 0;
}