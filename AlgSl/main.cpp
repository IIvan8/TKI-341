#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <locale>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

// 1. Пузырьковая сортировка
void bubbleSort(vector<int>& arr,
    long long& comparisons) {
    size_t n = arr.size();

    for (size_t i = 0; i < n - 1; ++i) {
        bool changed = false;

        for (size_t j = 0; j < n - 1 - i; ++j) {
            comparisons++;

            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                changed = true;
            }
        }

        // Если обменов не было, массив уже отсортирован
        if (!changed) {
            break;
        }
    }
}

// 2. Сортировка вставками с бинарным поиском
void insertionSort(vector<int>& arr,
    long long& comparisons) {
    vector<int> sorted;
    sorted.reserve(arr.size());

    for (size_t i = 0; i < arr.size(); ++i) {
        int key = arr[i];

        size_t left = 0;
        size_t right = sorted.size();

        // Бинарным поиском определяем место вставки
        while (left < right) {
            size_t mid = left + (right - left) / 2;
            comparisons++;

            if (sorted[mid] < key) {
                left = mid + 1;
            }
            else {
                right = mid;
            }
        }

        // Вставка элемента на найденное место
        sorted.insert(sorted.begin() + left, key);
    }

    arr = sorted;
}

// 3. Рекурсивная сортировка Хоара
void hoareSortExtraMemory(vector<int>& arr,
    long long& comparisons) {
    if (arr.size() <= 1) {
        return;
    }

    int pivot = arr[arr.size() / 2];

    vector<int> left;
    vector<int> equal;
    vector<int> right;

    // Разделение элементов относительно опорного
    for (size_t i = 0; i < arr.size(); ++i) {
        comparisons++;

        if (arr[i] < pivot) {
            left.push_back(arr[i]);
        }
        else {
            comparisons++;

            if (arr[i] > pivot) {
                right.push_back(arr[i]);
            }
            else {
                equal.push_back(arr[i]);
            }
        }
    }

    // Рекурсивная сортировка частей
    hoareSortExtraMemory(left, comparisons);
    hoareSortExtraMemory(right, comparisons);

    // Сборка отсортированного массива без перестановок
    size_t index = 0;

    for (size_t i = 0; i < left.size(); ++i) {
        arr[index++] = left[i];
    }

    for (size_t i = 0; i < equal.size(); ++i) {
        arr[index++] = equal[i];
    }

    for (size_t i = 0; i < right.size(); ++i) {
        arr[index++] = right[i];
    }
}

// Генерация случайного массива
vector<int> generateRandomArray(size_t size) {
    vector<int> arr(size);

    for (size_t i = 0; i < size; ++i) {
        arr[i] = rand() % 201 - 100;
    }

    // Гарантируем, что исходный массив не отсортирован
    if (is_sorted(arr.begin(), arr.end())) {
        if (arr.size() >= 2) {
            int temp = arr[0];
            arr[0] = arr[1];
            arr[1] = temp;
        }
    }

    return arr;
}

// Проверка результата сортировки
bool checkSorted(const vector<int>& arr) {
    return is_sorted(arr.begin(), arr.end());
}

int main() {
    setlocale(LC_ALL, "");

    srand(static_cast<unsigned int>(time(nullptr)));

    const size_t N = 10000;

    cout << "Генерация массива из " << N
        << " элементов..." << endl;

    vector<int> originalArray = generateRandomArray(N);

    // Сортировка пузырьком
    vector<int> arr1 = originalArray;
    long long comp1 = 0;

    auto start1 = steady_clock::now();

    bubbleSort(arr1, comp1);

    auto end1 = steady_clock::now();

    double time1 =
        duration<double, milli>(end1 - start1).count();

    // Сортировка вставками
    vector<int> arr2 = originalArray;
    long long comp2 = 0;

    auto start2 = steady_clock::now();

    insertionSort(arr2, comp2);

    auto end2 = steady_clock::now();

    double time2 =
        duration<double, milli>(end2 - start2).count();

    // Сортировка Хоара
    vector<int> arr3 = originalArray;
    long long comp3 = 0;

    auto start3 = steady_clock::now();

    hoareSortExtraMemory(arr3, comp3);

    auto end3 = steady_clock::now();

    double time3 =
        duration<double, milli>(end3 - start3).count();

    // Вывод результатов
    cout << "\n===== РЕЗУЛЬТАТЫ СОРТИРОВКИ =====\n";

    cout << "\n1. Пузырьковая сортировка\n";
    cout << "Сравнений: " << comp1 << endl;
    cout << "Время: " << time1 << " мс\n";
    cout << "Массив отсортирован: "
        << (checkSorted(arr1) ? "да" : "нет") << endl;

    cout << "\n2. Сортировка вставками\n";
    cout << "Сравнений: " << comp2 << endl;
    cout << "Время: " << time2 << " мс\n";
    cout << "Массив отсортирован: "
        << (checkSorted(arr2) ? "да" : "нет") << endl;

    cout << "\n3. Сортировка Хоара\n";
    cout << "Сравнений: " << comp3 << endl;
    cout << "Время: " << time3 << " мс\n";
    cout << "Массив отсортирован: "
        << (checkSorted(arr3) ? "да" : "нет") << endl;

    // Сравнение результатов
    cout << "\n===== АНАЛИЗ =====\n";

    if (time1 <= time2 && time1 <= time3) {
        cout << "По времени быстрее пузырьковая сортировка.\n";
    }
    else if (time2 <= time1 && time2 <= time3) {
        cout << "По времени быстрее сортировка вставками.\n";
    }
    else {
        cout << "По времени быстрее сортировка Хоара.\n";
    }

    if (comp1 <= comp2 && comp1 <= comp3) {
        cout << "Меньше сравнений у пузырьковой сортировки.\n";
    }
    else if (comp2 <= comp1 && comp2 <= comp3) {
        cout << "Меньше сравнений у сортировки вставками.\n";
    }
    else {
        cout << "Меньше сравнений у сортировки Хоара.\n";
    }

    if (arr1 == arr2 && arr2 == arr3) {
        cout << "Все три алгоритма дали одинаковый результат.\n";
    }
    else {
        cout << "Ошибка: результаты сортировок различаются!\n";
    }

    return 0;
}