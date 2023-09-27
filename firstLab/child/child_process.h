//
// Created by anton on 20.09.23.
//

#ifndef UNTITLED_CHILD_PROCESS_H
#define UNTITLED_CHILD_PROCESS_H
#define _R_            "\x1b[31m" // red color for child process output color
#define _G_            "\x1b[32m" // green color for parent process output color
#define _P_            "\x1b[35m" // green color for parent process output color
#define RESET_COLOR    "\x1b[0m"  // reset color
#define MAX_WORDS 100
#define MAX_LENGTH 20
#define FILE_NAME_SIZE 20
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sys/wait.h"

void remove_vowels(int*, char[6]);

#endif //UNTITLED_CHILD_PROCESS_H
