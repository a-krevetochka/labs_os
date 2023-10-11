//
// Created by meteo on 11.10.2023.
//
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>

const int MAX_SIZE = 100000;

int current_index_min_elements = 0;
int current_index_max_elements = 0;

HANDLE mutex;

using namespace std;

typedef struct thread_arguments {
    int size;
    int *part;
    int *max_elements;
    int *min_elements;
} thread_arguments;

DWORD WINAPI find_local_min_and_max_value(LPVOID lpParameter) {
    thread_arguments *arguments = static_cast<thread_arguments *>(lpParameter);

    int size = arguments->size;
    int *part = arguments->part;
    int *max_elements = arguments->max_elements;
    int *min_elements = arguments->min_elements;

    int local_min = part[0];
    int local_max = part[0];

    for (int index_of_element = 1; index_of_element < size; ++index_of_element) {
        if (part[index_of_element] > local_max) {
            local_max = part[index_of_element];
        }
        if (part[index_of_element] < local_min) {
            local_min = part[index_of_element];
        }
    }

    WaitForSingleObject(mutex, INFINITE);
    max_elements[current_index_max_elements] = local_max;
    ++current_index_max_elements;

    min_elements[current_index_min_elements] = local_min;
    ++current_index_min_elements;
    ReleaseMutex(mutex);

    return 0;
}

int main(int argc, char **argv) {
    int threads_count = atoi(argv[1]);
    int elements_quantity{0};
    int elements[MAX_SIZE];
    mutex = CreateMutexA(NULL, FALSE, NULL);

    cout << "do you wanna test me? y/n:\n";
    if (getchar() == 'y') {
        ifstream file("test.txt");
        if (!file.is_open()) {
            cout << "file was not open";
            return -1;
        }

        elements_quantity = MAX_SIZE;

        for (int index_of_element = 0; index_of_element < MAX_SIZE; ++index_of_element) {
            file >> elements[index_of_element];
        }
    } else {
        cout << "enter the number of elements, number can not be larger than 100000:" << endl;
        cin >> elements_quantity;

        cout << "Enter the elements:" << endl;
        for (int index_of_element = 0; index_of_element < elements_quantity; ++index_of_element) {
            cin >> elements[index_of_element];
        }
    }

    unsigned int startTime = clock();

    int size_whole_division = elements_quantity / threads_count;
    int size_rest = elements_quantity - (size_whole_division * (threads_count - 1));

    int min_elements[threads_count];
    int max_elements[threads_count];

    HANDLE handleThreadArray[threads_count];
    DWORD dwordThreadArray[threads_count];

    int part_start_index{0};

    for (int index_of_thread = 0; index_of_thread < threads_count; ++index_of_thread) {
        if (index_of_thread == threads_count - 1) {
            thread_arguments *threadArguments = new thread_arguments;
            threadArguments->max_elements = &max_elements[0];
            threadArguments->min_elements = &min_elements[0];
            threadArguments->size = size_rest;
            threadArguments->part = &elements[part_start_index];
            handleThreadArray[index_of_thread] = CreateThread(NULL,
                                                              0,
                                                              find_local_min_and_max_value,
                                                              threadArguments,
                                                              NULL,
                                                              &dwordThreadArray[index_of_thread]);
        } else {
            thread_arguments *threadArguments = new thread_arguments;
            threadArguments->max_elements = &max_elements[0];
            threadArguments->min_elements = &min_elements[0];
            threadArguments->size = size_whole_division;
            threadArguments->part = &elements[part_start_index];
            handleThreadArray[index_of_thread] = CreateThread(NULL,
                                                              0,
                                                              find_local_min_and_max_value,
                                                              threadArguments,
                                                              CREATE_SUSPENDED,
                                                              &dwordThreadArray[index_of_thread]);
            part_start_index += size_whole_division;
        }
    }

    for (int i = 0; i < threads_count; i++) ResumeThread(handleThreadArray[i]);

    WaitForMultipleObjects(threads_count, handleThreadArray, TRUE, INFINITE);
    for (int index_of_thread = 0; index_of_thread < threads_count; ++index_of_thread) {
        CloseHandle(handleThreadArray[index_of_thread]);
    }

    int absolute_max = max_elements[0];
    int absolute_min = min_elements[0];

    for (int index_of_element = 1; index_of_element < threads_count; ++index_of_element) {
        if (max_elements[index_of_element] > absolute_max) {
            absolute_max = max_elements[index_of_element];
        }
        if (min_elements[index_of_element] < absolute_min) {
            absolute_min = min_elements[index_of_element];
        }
    }

    cout << "minimal element is: " << absolute_min << endl;
    cout << "maximal element is: " << absolute_max << endl;

    unsigned int endTime = clock();
    cout << "program ran for " << (endTime - startTime) << " ms with " << threads_count << " threads";

    return 0;
}
