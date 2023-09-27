//#include "child_process.h"
#define _R_            "\x1b[31m" // red color for child process output color
#define _G_            "\x1b[32m" // green color for parent process output color
#define MAX_WORDS 100
#define MAX_LENGTH 20
#define FILE_NAME_SIZE 20
#define CHILD "./child"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sys/wait.h"

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

void error_handler(int result_of_creating){
    if(result_of_creating == -1){
        perror("creating error: ");
        exit(-1);
    }
}

int create_process() {
    pid_t pid = fork();
    error_handler(pid);
    return pid;
}

int main() {
    char first_file_name[FILE_NAME_SIZE], second_file_name[FILE_NAME_SIZE];
    char words[MAX_WORDS][MAX_LENGTH];

    printf(_R_"Enter the first filename: ");
    scanf("%s", &first_file_name);

    printf("Enter the second filename: ");
    scanf("%s", &second_file_name);

    int counter_of_words;
    printf("Enter the number of words: ");
    scanf("%d", &counter_of_words);

    printf("Enter the words: ");
    for (int i = 0; i < counter_of_words; i++) {
        scanf("%s", words[i]);
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

    int pipe_fd_for_first_child[2];
    error_handler(pipe(pipe_fd_for_first_child));

    pid_t first_process_id = create_process();

    if (first_process_id == 0) {
        printf(_G_"First child process start\n");

        close(pipe_fd_for_first_child[1]);
        dup2(pipe_fd_for_first_child[0], STDIN_FILENO);

        execl(CHILD, CHILD, NULL);
    } else {
        printf(_R_"parent process ");

        close(pipe_fd_for_first_child[0]);
        write(pipe_fd_for_first_child[1], &first_file_name, sizeof(first_file_name));
        for (int i = 0; i < MAX_WORDS; ++i) {
            write(pipe_fd_for_first_child[1], &long_words[i], sizeof(long_words[i]));
        }
        close(pipe_fd_for_first_child[1]);
        printf("Sent data to first process\n");

        wait(NULL);

        printf(_R_"first process finish\n");
    }

    int pipe_fd_for_second_child[2];
    error_handler(pipe(pipe_fd_for_second_child));

    pid_t second_process_id = create_process();

    if (second_process_id == 0) {
        printf(_G_"Second child process start\n");
        close(pipe_fd_for_first_child[1]);
        dup2(pipe_fd_for_first_child[0], STDIN_FILENO);
        execl(CHILD, CHILD, NULL);
    } else {
        printf(_R_"parent process ");

        close(pipe_fd_for_first_child[0]);
        write(pipe_fd_for_first_child[1], &second_file_name, sizeof(second_file_name));
        for (int i = 0; i < MAX_WORDS; ++i) {
            write(pipe_fd_for_first_child[1], &short_words[i], sizeof(long_words[i]));
        }
        close(pipe_fd_for_first_child[1]);

        printf("Sent data to second process\n");
        wait(NULL);
        printf(_R_"second process_finish\n");
    }
}