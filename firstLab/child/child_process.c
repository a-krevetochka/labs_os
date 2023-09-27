//
// Created by anton on 20.09.23.
//

#include "child_process.h"

int isVowel(char letter){
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

        default: return 0;
    }
}

int contain(char letter, char word[MAX_WORDS]){
    for (int i = 0; i < MAX_LENGTH; ++i) {
        if(word[i] == letter){
            return 1;
        }
    }
    return 0;
}

void write_to_file(char filename[FILE_NAME_SIZE], char words[MAX_WORDS][MAX_LENGTH]){
    FILE* file = fopen(filename, "w");
    if (file == NULL){
        printf("file wasn't open");
        exit(-1);
    }
    for (int index_of_word = 0; index_of_word < MAX_WORDS && words[index_of_word][0] != EOF; ++index_of_word){
        if(contain('_', words[index_of_word])){
            for (int index_of_letters = 0; index_of_letters < MAX_LENGTH && words[index_of_word][index_of_letters] != '\0'; ++index_of_letters) {
                if (words[index_of_word][index_of_letters] != '_') {
                    fprintf(file, "%c", words[index_of_word][index_of_letters]);
                    printf("%c", words[index_of_word][index_of_letters]);
                }
            }
            fprintf(file, "%c", '\n');
            printf("\n");
            continue;
        }
        fprintf(file, "%s\n", words[index_of_word]);
        printf("%s\n", words[index_of_word]);
    }
    fclose(file);
}

void remove_vowels(int* pipe, char color[6]){
    printf("%s", color);
    char filename[FILE_NAME_SIZE];
    char words[MAX_WORDS][MAX_LENGTH];


    close(pipe[1]);
    read(pipe[0], &filename, sizeof(filename));
    read(pipe[0], &words, sizeof(words));
    close(pipe[0]);
    for (int index_of_word = 0; index_of_word < MAX_WORDS && words[index_of_word][0] != '\0'; ++index_of_word){
        for (int index_of_letter = 0; index_of_letter < MAX_LENGTH && words[index_of_word][index_of_letter] != '\0'; ++index_of_letter){
            if (isVowel(words[index_of_word][index_of_letter])){
                words[index_of_word][index_of_letter] = '_';
            }
        }
    }

    write_to_file(filename, words);
    exit(0);
}


