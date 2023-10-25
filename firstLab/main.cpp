#include <cstdio>
#include "windows.h"
#include "iostream"
#include <iostream>
#include <unistd.h>
#include <fstream>


using namespace std;

const int MAX_WORDS = 100;
const int MAX_LENGTH = 20;
char first_file_name[MAX_LENGTH] = "first.txt";
char second_file_name[MAX_LENGTH] = "second.txt";
const char CHILD[] = "child.exe";

int length_of_word(char *word) {
    int length = 0;
    while (word[length] != '\0') {
        length++;
    }
    return length;
}

void copy_word(char *source, char *target) {
    int index = 0;
    while (source[index] != '\0') {
        target[index] = source[index];
        index++;
    }
    target[index + 1] = '\0';
}


int main(){
    char words[MAX_WORDS][MAX_LENGTH];
    int counter_of_words;
    if (getchar() == 'y'){
        std::ifstream file("test.txt");
        counter_of_words = MAX_WORDS;
        if (file.is_open()) {
            for (int index_of_words = 0; index_of_words < MAX_WORDS; ++index_of_words) {
                file >> words[index_of_words];
            }
            file.close();
        }
    }
    else{
        cout << "Enter the number of words: ";
        cin >> counter_of_words;

        cout << ("Enter the words: ");
        for (int i = 0; i < counter_of_words; i++) {
            cin >>  words[i];
        }
    }


    char short_words[MAX_WORDS][MAX_LENGTH];
    char long_words[MAX_WORDS][MAX_LENGTH];

    int short_count = 0, long_count = 0;

    for (int index_of_word = 0; index_of_word < counter_of_words; index_of_word++) {
        if (length_of_word(words[index_of_word]) < 10) {
            copy_word(words[index_of_word], short_words[short_count]);
            short_count++;
        } else {
            copy_word(words[index_of_word], long_words[long_count]);
            long_count++;
        }
    }
    short_words[short_count][0] = EOF;
    long_words[long_count][0] = EOF;

    HANDLE first_file_handler = CreateFileMappingA(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            sizeof(first_file_name) + sizeof(long_words), // Размер memory map
            "MyMemoryMap"
    );

    if (first_file_handler == NULL) {
        std::cerr << "cant create memory map." << std::endl;
        return 1;
    }

    // Получаем указатель на memory map
    LPVOID first_map_pointer = MapViewOfFile(
            first_file_handler,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(first_file_name) + sizeof(long_words)
    );

    if (first_map_pointer == NULL) {
        std::cerr << "cant get pointer to memory map." << std::endl;
        CloseHandle(first_file_handler);
        return 1;
    }

    memcpy(first_map_pointer, first_file_name, sizeof(first_file_name));

    char* long_words_pointer = (char*)first_map_pointer + sizeof(first_file_name);
    memcpy(long_words_pointer, long_words, sizeof(long_words));

    STARTUPINFOA startup_info_for_first_process;
    PROCESS_INFORMATION first_process_information;
    ZeroMemory(&startup_info_for_first_process, sizeof(startup_info_for_first_process));
    ZeroMemory(&first_process_information, sizeof(first_process_information));
    startup_info_for_first_process.cb = sizeof(startup_info_for_first_process);

    if (!CreateProcessA(
            "child.exe", // Имя второй программы
            NULL,
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &startup_info_for_first_process,
            &first_process_information
    )) {
        std::cerr << "cant create child process" << std::endl;
        return 1;
    }

    WaitForSingleObject(first_process_information.hProcess, INFINITE);

    UnmapViewOfFile(first_map_pointer);
    CloseHandle(first_file_handler);

    CloseHandle(first_process_information.hProcess);
    CloseHandle(first_process_information.hThread);

    HANDLE second_file_handler = CreateFileMappingA(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            sizeof(second_file_name) + sizeof(short_words), // Размер memory map
            "MyMemoryMap"
    );

    LPVOID second_map_pointer = MapViewOfFile(
            second_file_handler,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(second_file_name) + sizeof(short_words)
    );

    if (second_map_pointer == NULL) {
        std::cerr << "cant get pointer to memory map." << std::endl;
        CloseHandle(first_file_handler);
        return 1;
    }

    memcpy(first_map_pointer, second_file_name, sizeof(first_file_name));

    char* short_words_pointer = (char*)first_map_pointer + sizeof(first_file_name);
    memcpy(short_words_pointer, short_words, sizeof(short_words));

    STARTUPINFOA startup_info_for_second_process;
    PROCESS_INFORMATION second_process_information;
    ZeroMemory(&startup_info_for_second_process, sizeof(startup_info_for_second_process));
    ZeroMemory(&second_process_information, sizeof(second_process_information));
    startup_info_for_second_process.cb = sizeof(startup_info_for_second_process);

    if (!CreateProcessA(
            "child.exe", // Имя второй программы
            NULL,
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &startup_info_for_second_process,
            &second_process_information
    )) {
        std::cerr << "cant create child process" << std::endl;
        return 1;
    }

    WaitForSingleObject(second_process_information.hProcess, INFINITE);

    UnmapViewOfFile(second_map_pointer);
    CloseHandle(second_file_handler);

    CloseHandle(second_process_information.hProcess);
    CloseHandle(second_process_information.hThread);

    return 0;
}