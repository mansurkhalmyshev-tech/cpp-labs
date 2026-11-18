#include <iostream>     
#include <vector>       
#include <string>      
#include <chrono>       
#include <algorithm>    
#include <cstdlib>      
#include <iomanip>      

using namespace std;    
using namespace std::chrono; 

// Функция поиска ближайшего элемента в отсортированном массиве
int findClosest(const vector<int>& arr, int target) {
    if (arr.empty()) return -1; // Если массив пуст, возвращаем -1 (ошибка)

    int left = 0;               // Указатель на начало массива
    int right = arr.size() - 1; // Указатель на конец массива

    // Если искомое число меньше или равно первому элементу,
    // то ближайший элемент - это первый (самый левый)
    if (target <= arr[left]) return arr[left];

    // Если искомое число больше или равно последнему,
    // то ближайший - последний (самый правый)
    if (target >= arr[right]) return arr[right];

    // Бинарный поиск границ
    while (left <= right) {     // Пока левая граница не пересекла правую
        int mid = left + (right - left) / 2; // Вычисляем середину 

        if (arr[mid] == target) return arr[mid]; // Если точное совпадение, возвращаем его

        if (arr[mid] < target) { // Если середина меньше искомого
            left = mid + 1;      // Сдвигаем левую границу вправо
        }
        else {                 // Если середина больше искомого
            right = mid - 1;     // Сдвигаем правую границу влево
        }
    }

  
    // Вычисляем абсолютную разницу до обоих элементов
    int diffLeft = abs(target - arr[right]);
    int diffRight = abs(target - arr[left]);

    // Возвращаем тот элемент, разница с которым меньше (или равный, если расстояния равны)
    return (diffLeft <= diffRight) ? arr[right] : arr[left];
}

class HashTable {
private:
    // Структура для хранения одной ячейки таблицы
    struct Entry {
        string key;     // Ключ 
        string value;   // Значение
        bool occupied;  // Флаг: занята ли ячейка 
        bool deleted;   // Флаг: удалена ли ячейка 

        // Конструктор ячейки по умолчанию
        Entry() : occupied(false), deleted(false) {}
    };

    vector<Entry> table; // Вектор (массив) для хранения всех ячеек
    int capacity;        // Максимальный размер таблицы
    int count;           // Текущее количество элементов в таблице

    // Хеш-функция для строк (полиномиальный хеш)
    int hash(const string& key) const {
        int hashVal = 0;                    // Начальное значение хеша
        for (char c : key) {                // Проходим по каждому символу строки
            // Умножаем текущий хеш на 31 
            hashVal = (hashVal * 31 + c) % capacity;
        }
        return abs(hashVal);                
    }

public:
    // Конструктор хеш-таблицы
    HashTable(int size = 11) : capacity(size), count(0) {
        table.resize(capacity); // Выделяем память под массив ячеек
    }

    // Вставка элемента (с использованием линейного пробирования)
    void insert(const string& key, const string& value) {
        int index = hash(key); // Вычисляем идеальный индекс для ключа
        int i = 0;             // Счетчик попыток (шаг пробирования)

        while (i < capacity) { // Пока не проверим всю таблицу
            int pos = (index + i) % capacity; // Вычисляем текущую позицию (линейное пробирование)

            // Если нашли ячейку с таким же ключом (и она не удалена), обновляем значение
            if (table[pos].occupied && !table[pos].deleted && table[pos].key == key) {
                table[pos].value = value;
                return; // Выходим, обновление завершено
            }

            // Если нашли свободную или удаленную ячейку, вставляем туда
            if (!table[pos].occupied || table[pos].deleted) {
                table[pos].key = key;
                table[pos].value = value;
                table[pos].occupied = true;  // Помечаем, что ячейка занята
                table[pos].deleted = false;  // Снимаем флаг удаления (если был)
                count++;                     // Увеличиваем счетчик элементов
                return; // Выходим, вставка завершена
            }
            i++; // Если ячейка занята другим ключом - переходим к следующей
        }
        cout << "Ошибка: таблица заполнена, не удалось вставить " << key << endl;
    }

    // Поиск элемента по ключу
    bool find(const string& key, string& result) const {
        int index = hash(key); // Вычисляем идеальный индекс
        int i = 0;             // Счетчик попыток

        while (i < capacity) {
            int pos = (index + i) % capacity; 

            // Если наткнулись на пустую ячейку значит ключа тут точно нет
            if (!table[pos].occupied) return false;

            // Если нашли нужный ключ (и он не помечен как удаленный)
            if (!table[pos].deleted && table[pos].key == key) {
                result = table[pos].value; // Записываем найденное значение в result
                return true;             
            }
            i++; // Ищем дальше
        }
        return false; // Прошли всю таблицу, не нашли
    }

    // Удаление элемента по ключу
    bool remove(const string& key) {
        int index = hash(key);
        int i = 0;

        while (i < capacity) {
            int pos = (index + i) % capacity;

            if (!table[pos].occupied) return false; // Пустая ячейка - элемента нет

            // Если нашли ключ
            if (!table[pos].deleted && table[pos].key == key) {
                table[pos].deleted = true; // Логическое удаление ("могильная плита")
                count--;                   // Уменьшаем счетчик
                return true;
            }
            i++;
        }
        return false;
    }
};


// Итеративный двоичный поиск (для сравнения)
// Возвращает индекс элемента или -1, если не найден
int binarySearch(const vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2; // Середина
        if (arr[mid] == target) return mid;  // Нашли
        if (arr[mid] < target) left = mid + 1; // Ищем справа
        else right = mid - 1;                // Ищем слева
    }
    return -1; // Не нашли
}

int main() {
    setlocale(LC_ALL, "ru"); // Поддержка русского языка
    srand(time(0));          // Инициализация генератора случайных чисел

    cout << "=== Лабораторная работа №8 (Вариант 10) ===" << endl << endl;

    // --- ЧАСТЬ 1: Поиск ближайшего элемента ---
    cout << "--- ЧАСТЬ 1: Поиск ближайшего элемента ---" << endl;
    vector<int> sortedArr = { 10, 20, 30, 40, 50, 60, 70 }; // Отсортированный массив
    cout << "Массив: ";
    for (int x : sortedArr) cout << x << " "; // Вывод массива
    cout << endl;

    int targets[] = { 33, 48, 5, 75 }; // Тестовые значения для поиска
    for (int t : targets) {
        cout << "Ближайший к " << t << ": " << findClosest(sortedArr, t) << endl;
    }
    cout << endl;

    // --- ЧАСТЬ 2: Хеш-таблица ---
    cout << "--- ЧАСТЬ 2: Хеш-таблица (Линейное пробирование) ---" << endl;
    HashTable ht(10); // Создаем таблицу размером 10
    ht.insert("apple", "яблоко");
    ht.insert("banana", "банан");
    ht.insert("cherry", "вишня");
    ht.insert("date", "финик");
    ht.insert("elderberry", "бузина");

    string val; // Переменная для хранения найденного значения
    if (ht.find("cherry", val)) cout << "Найдено cherry: " << val << endl;
    if (ht.find("banana", val)) cout << "Найдено banana: " << val << endl;

    cout << "Удаляем banana..." << endl;
    ht.remove("banana"); // Удаляем элемент

    // Проверяем, что удаление сработало
    if (ht.find("banana", val)) cout << "Найдено banana: " << val << endl;
    else cout << "banana не найдено (успешно удалено)" << endl;
    cout << endl;

    // --- ЧАСТЬ 3: Сравнительный анализ ---
    cout << "--- ЧАСТЬ 3: Сравнение производительности ---" << endl;
    vector<int> sizes = { 1000, 10000, 100000 }; // Размеры массивов для теста

    // Вывод заголовка таблицы с выравниванием
    cout << left << setw(10) << "Размер (N)"
        << setw(25) << "Бинарный поиск (мкс)"
        << setw(25) << "Хеш-таблица (мкс)" << endl;
    cout << string(60, '-') << endl;

    for (int n : sizes) {
        // 1. Подготовка данных для бинарного поиска
        vector<int> arr(n);
        for (int i = 0; i < n; i++) arr[i] = i * 2; // Заполняем четными числами (отсортировано)

        // 2. Подготовка хеш-таблицы (имитация)
        vector<int> hashTable(n, -1); // Массив-имитация хеш-таблицы
        for (int i = 0; i < n; i++) {
            hashTable[i] = i * 2; // Прямая адресация (ключ = значение)
        }

        // Генерируем 10000 случайных запросов
        vector<int> queries(10000);
        for (int i = 0; i < 10000; i++) {
            queries[i] = rand() % (n * 2);
        }

        // 3. Замер бинарного поиска
        auto start = high_resolution_clock::now(); // Засекаем начало
        volatile int sumBin = 0; // Переменная-заглушка (чтобы компилятор не оптимизировал цикл)
        for (int q : queries) {
            sumBin += binarySearch(arr, q); // Выполняем поиск
        }
        auto end = high_resolution_clock::now(); // Засекаем конец
        long long timeBin = duration_cast<microseconds>(end - start).count(); // Время в мкс

        // 4. Замер хеш-таблицы (прямое обращение к массиву)
        start = high_resolution_clock::now();
        volatile int sumHash = 0;
        for (int q : queries) {
            // Имитация поиска за O(1)
            if (q < n && hashTable[q] != -1) {
                sumHash += hashTable[q];
            }
        }
        end = high_resolution_clock::now();
        long long timeHash = duration_cast<microseconds>(end - start).count();

        // Вывод строки результатов
        cout << left << setw(10) << n
            << setw(25) << timeBin
            << setw(25) << timeHash << endl;
    }

    return 0; // Успешное завершение
}