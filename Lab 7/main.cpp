#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <iomanip> // Для красивого вывода таблицы

using namespace std;
using namespace std::chrono;

// Функция разделения массива относительно опорного элемента
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Опорный элемент (последний)
    int i = (low - 1);     // Индекс для меньшего элемента

    for (int j = low; j <= high - 1; j++) {
        // Если текущий элемент меньше или равен опорному
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// Основная рекурсивная функция быстрой сортировки
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        // pi - индекс опорного элемента после разделения
        int pi = partition(arr, low, high);

        // Рекурсивно сортируем левую и правую части
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


// Функция слияния двух подмассивов
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Создаем временные массивы
    int* L = new int[n1];
    int* R = new int[n2];

    // Копируем данные во временные массивы
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Сливаем временные массивы обратно в arr
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Копируем оставшиеся элементы L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Копируем оставшиеся элементы R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

// Основная рекурсивная функция сортировки слиянием
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Сортируем первую и вторую половины
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Сливаем их
        merge(arr, left, mid, right);
    }
}


// Функция для построения кучи (просеивание вниз)
void heapify(int arr[], int n, int i) {
    int largest = i;       // Инициализируем наибольший как корень
    int left = 2 * i + 1;  // Левый потомок
    int right = 2 * i + 2; // Правый потомок

    // Если левый потомок больше корня
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // Если правый потомок больше наибольшего на данный момент
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // Если наибольший не корень
    if (largest != i) {
        swap(arr[i], arr[largest]);
        // Рекурсивно просеиваем вниз затронутое поддерево
        heapify(arr, n, largest);
    }
}

// Основная функция пирамидальной сортировки
void heapSort(int arr[], int n) {
    // 1. Построение max-heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // 2. Извлечение элементов из кучи по одному
    for (int i = n - 1; i > 0; i--) {
        // Перемещаем текущий корень в конец
        swap(arr[0], arr[i]);
        // Вызываем heapify на уменьшенной куче
        heapify(arr, i, 0);
    }
}


// Обертка для замера времени (принимает лямбда-функцию)
template <typename Func>
long long measureTime(Func sortFunc, int arr[], int n) {
    // Создаем копию массива, чтобы каждый раз сортировать одинаковые данные
    int* copy = new int[n];
    for (int i = 0; i < n; i++) copy[i] = arr[i];

    auto start = high_resolution_clock::now();
    sortFunc(copy, n); // Вызов функции сортировки
    auto end = high_resolution_clock::now();

    delete[] copy;
    return duration_cast<microseconds>(end - start).count();
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(0)); // Инициализация генератора случайных чисел

    cout << "=== Лабораторная работа №7 (Средний уровень, Вариант 10) ===" << endl;
    cout << "Сравнение времени работы сортировок на случайных массивах" << endl << endl;

    // Размеры массивов для тестирования
    vector<int> sizes = { 1000, 2000, 5000, 10000 };

    // Вывод заголовка таблицы
    cout << left << setw(10) << "Размер (N)"
        << setw(20) << "Quick Sort (мкс)"
        << setw(20) << "Merge Sort (мкс)"
        << setw(20) << "Heap Sort (мкс)" << endl;
    cout << string(70, '-') << endl;

    for (int n : sizes) {
        // Создаем и заполняем исходный массив случайными числами
        int* arr = new int[n];
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % (n * 10); // Числа от 0 до N*10
        }

        // Замер времени для Quick Sort
        long long timeQuick = measureTime([](int a[], int size) {
            quickSort(a, 0, size - 1);
            }, arr, n);

        // Замер времени для Merge Sort
        long long timeMerge = measureTime([](int a[], int size) {
            mergeSort(a, 0, size - 1);
            }, arr, n);

        // Замер времени для Heap Sort
        long long timeHeap = measureTime([](int a[], int size) {
            heapSort(a, size);
            }, arr, n);

        // Вывод строки таблицы
        cout << left << setw(10) << n
            << setw(20) << timeQuick
            << setw(20) << timeMerge
            << setw(20) << timeHeap << endl;

        delete[] arr; // Освобождаем память
    }

    cout << endl;
    cout << "Анализ завершен. Данные можно использовать для построения графика." << endl;

    return 0;
}