#include "child_process.h"

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

int create_process() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("child was not created");
        exit(-1);
    }
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

    for (int i = 0; i < counter_of_words; i++) {
        if (length_of_word(words[i]) < 10) {
            copy_word(words[i], short_words[short_count]);
            short_count++;
        } else {
            copy_word(words[i], long_words[long_count]);
            long_count++;
        }
    }
    short_words[short_count][0] = EOF;
    long_words[long_count][0] = EOF;

    int pipe_fd_for_first_child[2];
    if (pipe(pipe_fd_for_first_child) == -1) {
        perror("first pipe was not created");
        return -1;
    }

    pid_t first_process_id = create_process();
    if (first_process_id == 0) {
        printf(_G_"Child process start\n");
        remove_vowels((int *) &pipe_fd_for_first_child, _G_);
        

    } else {
        printf(_R_"parent process ");

        close(pipe_fd_for_first_child[0]);

        write(pipe_fd_for_first_child[1], &first_file_name, sizeof(first_file_name));
        write(pipe_fd_for_first_child[1], &long_words, sizeof(long_words));
        close(pipe_fd_for_first_child[1]);

        printf("Sent data to first process\n");

        wait(NULL);

        printf(_R_"first process_finish\n");
    }

    int pipe_fd_for_second_child[2];
    if (pipe(pipe_fd_for_second_child) == -1) {
        perror("first pipe was not created");
        return -1;
    }

    pid_t second_process_id = create_process();
    if (second_process_id == 0) {
        printf(_P_"Child process start\n");
        remove_vowels((int *) &pipe_fd_for_second_child, _P_);

    } else {
        printf(_R_"parent process ");
        close(pipe_fd_for_first_child[0]);
        write(pipe_fd_for_first_child[1], &second_file_name, sizeof(second_file_name));
        write(pipe_fd_for_first_child[1], &short_words, sizeof(short_words));
        close(pipe_fd_for_first_child[1]);
        printf("Sent data to second process\n");
        wait(NULL);
        printf(_R_"second process_finish\n");
    }
}