//
// Created by meteo on 11.10.2023.
//
#include <iostream>
#include <thread>
#include <fstream>
#include <cstdlib>
#include <ctime>

const int MAX_ARRAY_SIZE = 50;
const int MINIMAL_MERGE_SIZE = 32;

int min(int first_operand, int second_operand) {
    return (first_operand < second_operand) ? first_operand : second_operand;
}

int minRunLength(int number) {
    int result = 0;
    while (number >= MINIMAL_MERGE_SIZE) {
        result |= (number & 1);
        number >>= 1;
    }
    return number + result;
}

void insertionSort(int* array, int left, int right) {
    for (int index = left + 1; index <= right; index++) {
        int key = array[index];
        int support_index = index - 1;
        while (support_index >= left && array[support_index] > key) {
            array[support_index + 1] = array[support_index];
            support_index--;
        }
        array[support_index + 1] = key;
    }
}

void merge(int* array, int left_start_index, int middle_index, int right_start_index) {
    int first_part_length = middle_index - left_start_index + 1;
    int second_part_length = right_start_index - middle_index;
    int* left = new int[first_part_length];
    int* right = new int[second_part_length];

    for (int index_of_element = 0; index_of_element < first_part_length; index_of_element++)
        left[index_of_element] = array[left_start_index + index_of_element];

    for (int i = 0; i < second_part_length; i++)
        right[i] = array[middle_index + 1 + i];

    int first_index = 0, second_index = 0, left_size_index = left_start_index;
    while (first_index < first_part_length && second_index < second_part_length) {
        if (left[first_index] <= right[second_index]) {
            array[left_size_index] = left[first_index];
            first_index++;
        }
        else {
            array[left_size_index] = right[second_index];
            second_index++;
        }
        left_size_index++;
    }

    while (first_index < first_part_length) {
        array[left_size_index] = left[first_index];
        left_size_index++;
        first_index++;
    }

    while (second_index < second_part_length) {
        array[left_size_index] = right[second_index];
        left_size_index++;
        second_index++;
    }
}

void timSort(int* array, int lenght) {

    int minimal_run = minRunLength(MINIMAL_MERGE_SIZE);

    for (int i = 0; i < lenght; i += minimal_run)
        insertionSort(array, i, min(i + minimal_run - 1, lenght - 1));

    for (int size = minimal_run; size < lenght; size = 2 * size) {
        for (int left = 0; left < lenght; left += 2 * size) {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (lenght - 1));
            merge(array, left, mid, right);
        }
    }
}

int main(int argc, char** argv) {
    int number_of_threads = atoi(argv[1]);
    int array[MAX_ARRAY_SIZE]; // Исходный массив для сортировки
    std::cout << "Do u want to test me? y/n" << std::endl;

    if (getchar() == 'y'){
        std::ifstream inputFile("test.txt");

        for (int i = 0; i < MAX_ARRAY_SIZE; ++i) {
            inputFile >> array[i];
        }
        inputFile.close();
    } else{
        std::cout << "Enter 50 numbers: ";
        for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
            std::cin >> array[i];
        }
    }

    std::thread threads[number_of_threads];
    int chunkSize = MAX_ARRAY_SIZE / number_of_threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int index_of_tread = 0; index_of_tread < number_of_threads; index_of_tread++) {
        int startIndex = index_of_tread * chunkSize;
        int endIndex = (index_of_tread == number_of_threads - 1) ? MAX_ARRAY_SIZE : (index_of_tread + 1) * chunkSize;

        threads[index_of_tread] = std::thread(timSort, array + startIndex, endIndex - startIndex);
    }

    // Ожидание завершения всех потоков
    for (int index_of_thread = 0; index_of_thread < number_of_threads; index_of_thread++) {
        threads[index_of_thread].join();
    }

    timSort(array, MAX_ARRAY_SIZE);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << "\nDuration: " << duration.count() << " ms" << std::endl;
}



