﻿const int NUM_THREAD[] = { 1, 2, 4, 10 };
const int NUM_STRING = 300;
const int LENGTH = 200000;

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <omp.h>
using namespace std;

// Функция для подсчета количества цифр "9" в строке
int CountNines(const string& str) {
    int count = 0;
    for (char c : str) {
        if (c == '9') {
            count++;
        }
    }
    return count;
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<string> strings(NUM_STRING);
    vector<int> results(NUM_STRING);
    // Генерируем случайные строки
    for (string& str : strings) {
        str = "";
        for (int j = 0; j < LENGTH; ++j) {
            str += to_string(rand() % 10);
        }
    }
    // Измеряем время выполнения для разного числа потоков
    for (int num_threads : NUM_THREAD) {
        auto start_time = chrono::high_resolution_clock::now();
        // Открываем параллельную секцию с num_threads потоками
#pragma omp parallel num_threads(num_threads)
        {
            // Получаем номер текущего потока   
            int thread_id = omp_get_thread_num();
            // Рассчитываем размер данных для каждого потока
            int chunk_size = NUM_STRING / num_threads;
            int start = thread_id * chunk_size;
            int end = (thread_id == num_threads - 1) ? NUM_STRING : (thread_id + 1) * chunk_size;
            // Каждый поток выполняет подсчет в своем диапазоне строк
            for (int i = start; i < end; ++i) {
                results[i] = CountNines(strings[i]);
            }
        }//закрываем

        auto end_time = chrono::high_resolution_clock::now();
        auto execution_time_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        double execution_time_seconds = static_cast<double>(execution_time_ms.count()) / 1000.0;

        cout << "Время выполнения для " << num_threads << " потоков: " << execution_time_seconds << " секунд\n";
    }

    return 0;
}
