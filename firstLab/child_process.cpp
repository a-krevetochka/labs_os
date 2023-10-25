//
// Created by anton on 20.09.23.
//
const int MAX_WORDS = 100;
const int MAX_LENGTH = 20;
const int FILE_NAME_SIZE = 20;

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstdio>
#include "windows.h"
#include <cctype>

using namespace std;

int isVowel(char letter) {
    switch (letter) {
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
        case 'Y':
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'y':
            return 1;

        default:
            return 0;
    }
}

int contain(char letter, char word[MAX_WORDS]) {
    for (int i = 0; i < MAX_LENGTH; ++i) {
        if (word[i] == letter) {
            return 1;
        }
    }
    return 0;
}

void write_to_file(char filename[FILE_NAME_SIZE], char words[MAX_WORDS][MAX_LENGTH]) {
    ofstream file (filename);
    if (!file.is_open()) {
        cerr << "file wasnt open" << endl;
        exit(-1);
    }
    for (int index_of_word = 0; index_of_word < MAX_WORDS && words[index_of_word][0] != EOF; ++index_of_word) {
        if (contain('_', words[index_of_word])) {
            for (int index_of_letters = 0;
                 index_of_letters < MAX_LENGTH && words[index_of_word][index_of_letters] != '\0'; ++index_of_letters) {
                if (words[index_of_word][index_of_letters] != '_') {
                    file << words[index_of_word][index_of_letters];
                    cout << words[index_of_word][index_of_letters];
                }
            }
            file << endl;
            cout << endl;
            continue;
        }
        file << words[index_of_word];
        cout << words[index_of_word] << endl;
    }
    file.close();
}

void remove_vowels(char filename[FILE_NAME_SIZE], char words[MAX_WORDS][MAX_LENGTH]) {
    for (int index_of_word = 0; index_of_word < MAX_WORDS && words[index_of_word][0] != '\0'; ++index_of_word) {
        for (int index_of_letter = 0;
             index_of_letter < MAX_LENGTH && words[index_of_word][index_of_letter] != '\0'; ++index_of_letter) {
            if (isVowel(words[index_of_word][index_of_letter])) {
                words[index_of_word][index_of_letter] = '_';
            }
        }
    }
}


int main() {
    HANDLE fileHandle = OpenFileMappingA(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            "MyMemoryMap"
    );

    if (fileHandle == NULL) {
        std::cerr << "Не удалось открыть memory map." << std::endl;
        return 1;
    }

    // Получаем указатель на memory map
    char* mapPointer = (char*)MapViewOfFile(
            fileHandle,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            0
    );

    if (mapPointer == NULL) {
        std::cerr << "Не удалось получить указатель на memory map." << std::endl;
        CloseHandle(fileHandle);
        return 1;
    }

    // Читаем имя файла из memory map
    char filename[MAX_LENGTH];
    strcpy_s(filename, sizeof(filename), mapPointer);

    // Читаем массив char[][] из memory map
    char(*words)[MAX_LENGTH] = (char(*)[MAX_LENGTH])(mapPointer + sizeof(filename));

    remove_vowels(filename, words);
    write_to_file(filename, words);
}


