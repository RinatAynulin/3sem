#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <time.h>

#define MaxLineLength 256
#define MaxLinesCount 256
#define MaxArgsCount 16
#define FileName "input.in"
#define Delimiters " \n"

typedef struct {
    char **args;
    char *line;
    int argsCount, time;
} program;


void split(char* string, char* delimiters, char*** tokens_ptr, int* tokensCount) {
    char** tokens = *tokens_ptr;
    char* currentToken = NULL;
    currentToken = strtok(string, delimiters);
    while (currentToken) {
        tokens[*tokensCount] = currentToken;
        (*tokensCount)++;
        currentToken = strtok(NULL, delimiters);
    }
    tokens[*tokensCount] = currentToken;
}

void getPrograms(FILE *input, program** programs_ptr, int programsCount) {
    int i, n;
    program* currentProgram = NULL;
    *programs_ptr = (program*) malloc(MaxLinesCount * sizeof(program));
    currentProgram = *programs_ptr;

    for (i = 0; i < programsCount; i++) {
        (currentProgram[i]).args = (char**) malloc(MaxArgsCount * sizeof(char*));
        (currentProgram[i]).line = (char*) malloc(MaxLineLength * sizeof(char*));

        fgets((currentProgram[i]).line, MaxLineLength, input);

        split((currentProgram[i]).line, Delimiters, &((currentProgram[i]).args), &((currentProgram[i]).argsCount));
        (currentProgram[i]).time = atoi(((currentProgram[i]).args)[0]);
        for (n = 0; n < (currentProgram[i]).argsCount - 1; n++) {
            (currentProgram[i]).args[n] = (currentProgram[i]).args[n+1];
        }
        (currentProgram[i].argsCount)--;
    }
}

void runPrograms(program* programs, int programsCount) {
    int i, status;
    pid_t pid = 0;
    for (i = 0; i < programsCount; i++) {
        pid = fork();
        if (pid == 0) {
            pid = getpid();
            printf("Process %d (%s)  > \n", pid, programs[i].args[0]);
            sleep(programs[i].time);
            execvp(programs[i].args[0], programs[i].args);
            printf("Error\n");
            exit(-1);
        }
    }
}

void getProgramsCount(FILE *fp, int* programsCount) {
    char* programsCountString = (char*)malloc(MaxLineLength * sizeof(char));
    fgets(programsCountString, MaxLineLength, fp);
    *programsCount = atoi(programsCountString);
    free(programsCountString);
}


int main() {
    int programsCount, i;
    FILE *fp = fopen(FileName, "r");

    getProgramsCount(fp, &programsCount);
    program *programs = (program*)malloc(programsCount * sizeof(program));
    getPrograms(fp, &programs, programsCount);
    runPrograms(programs, programsCount);

    for (i = 0; i < programsCount; i++) {
        free ((programs)[i].args);
        free ((programs)[i].line);
    }
    free(programs);
    fclose(fp);
    return 0;
}
