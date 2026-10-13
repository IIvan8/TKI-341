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
    long long& comparisons,
    long long& assignments) {
    size_t n = arr.size();

    for (size_t i = 0; i < n - 1; ++i) {
        bool changed = false;

        for (size_t j = 0; j < n - 1 - i; ++j) {
            comparisons++;

            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                assignments += 3;
                changed = true;
            }
        }

        // Если обменов не было, массив уже отсортирован
        if (!changed) {
            break;
        }
    }
}

// 2. Сортировка вставками
void insertionSort(vector<int>& arr,
    long long& comparisons,
    long long& assignments) {
    size_t n = arr.size();

    for (size_t i = 1; i < n; ++i) {
        int key = arr[i];
        assignments++;

        size_t j = i;

        while (j > 0) {
            comparisons++;

            if (arr[j - 1] <= key) {
                break;
            }

            arr[j] = arr[j - 1];
            assignments++;
            --j;
        }

        arr[j] = key;
        assignments++;
    }
}

// 3. Рекурсивная сортировка Хоара
// Используется дополнительная память.
// Обменов (swap) нет.
void hoareSortExtraMemory(vector<int>& arr,
    long long& comparisons,
    long long& assignments) {
    if (arr.size() <= 1) {
        return;
    }

    int pivot = arr[arr.size() / 2];
    assignments++;

    vector<int> left;
    vector<int> equal;
    vector<int> right;

    // Разделение элементов относительно опорного
    for (size_t i = 0; i < arr.size(); ++i) {
        comparisons++;

        if (arr[i] < pivot) {
            left.push_back(arr[i]);
            assignments++;
        }
        else {
            comparisons++;

            if (arr[i] > pivot) {
                right.push_back(arr[i]);
                assignments++;
            }
            else {
                equal.push_back(arr[i]);
                assignments++;
            }
        }
    }

    // Рекурсивная сортировка частей
    hoareSortExtraMemory(left, comparisons, assignments);
    hoareSortExtraMemory(right, comparisons, assignments);

    // Сборка отсортированного массива без перестановок
    size_t index = 0;

    for (size_t i = 0; i < left.size(); ++i) {
        arr[index++] = left[i];
        assignments++;
    }

    for (size_t i = 0; i < equal.size(); ++i) {
        arr[index++] = equal[i];
        assignments++;
    }

    for (size_t i = 0; i < right.size(); ++i) {
        arr[index++] = right[i];
        assignments++;
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
    long long assign1 = 0;

    auto start1 = steady_clock::now();

    bubbleSort(arr1, comp1, assign1);

    auto end1 = steady_clock::now();

    double time1 =
        duration<double, milli>(end1 - start1).count();

    // Сортировка вставками
    vector<int> arr2 = originalArray;
    long long comp2 = 0;
    long long assign2 = 0;

    auto start2 = steady_clock::now();

    insertionSort(arr2, comp2, assign2);

    auto end2 = steady_clock::now();

    double time2 =
        duration<double, milli>(end2 - start2).count();

    // Сортировка Хоара
    vector<int> arr3 = originalArray;
    long long comp3 = 0;
    long long assign3 = 0;

    auto start3 = steady_clock::now();

    hoareSortExtraMemory(arr3, comp3, assign3);

    auto end3 = steady_clock::now();

    double time3 =
        duration<double, milli>(end3 - start3).count();

    // Вывод результатов
    cout << "\n===== РЕЗУЛЬТАТЫ СОРТИРОВКИ =====\n";

    cout << "\n1. Пузырьковая сортировка\n";
    cout << "Сравнений: " << comp1 << endl;
    cout << "Присваиваний: " << assign1 << endl;
    cout << "Время: " << time1 << " мс\n";
    cout << "Массив отсортирован: "
        << (checkSorted(arr1) ? "да" : "нет") << endl;

    cout << "\n2. Сортировка вставками\n";
    cout << "Сравнений: " << comp2 << endl;
    cout << "Присваиваний: " << assign2 << endl;
    cout << "Время: " << time2 << " мс\n";
    cout << "Массив отсортирован: "
        << (checkSorted(arr2) ? "да" : "нет") << endl;

    cout << "\n3. Сортировка Хоара\n";
    cout << "Сравнений: " << comp3 << endl;
    cout << "Присваиваний: " << assign3 << endl;
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

    if (arr1 == arr2 && arr2 == arr3) {
        cout << "Все три алгоритма дали одинаковый результат.\n";
    }
    else {
        cout << "Ошибка: результаты сортировок различаются!\n";
    }

    return 0;
}