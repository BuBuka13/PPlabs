const int NUM_THREAD[] = { 1, 2, 4, 10 };
const int NUM_STRING = 300;
const int LENGTH = 200000;

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
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

// Функция, которую будут выполнять потоки
void ThreadFunction(const vector<string>& strings, int start, int end, vector<int>& results) {
    for (int i = start; i < end; ++i) {
        results[i] = CountNines(strings[i]);
    }
}

int main() {
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
        vector<thread> threads;
        auto start_time = chrono::high_resolution_clock::now();

        // Распределяем строки между потоками
        int chunk_size = NUM_STRING / num_threads;
        for (int i = 0; i < num_threads; ++i) {
            int start = i * chunk_size;
            int end = (i == num_threads - 1) ? NUM_STRING : (i + 1) * chunk_size;
            threads.emplace_back(ThreadFunction, ref(strings), start, end, ref(results));
        }

        // Дожидаемся завершения всех потоков
        for (thread& thread : threads) {
            thread.join();
        }

        auto end_time = chrono::high_resolution_clock::now();
        auto execution_time_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        double execution_time_seconds = static_cast<double>(execution_time_ms.count()) / 1000.0;

        cout << "Время выполнения для " << num_threads << " потоков: " << execution_time_seconds << " секунд\n";
    }

    return 0;
}
