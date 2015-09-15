#include <stdio.h>
#include <string.h>
#include <malloc.h>

void Split(char* string, char* delimiters, char*** tokens_ptr, int* tokensCount) {
    int str_num = 30;
    *tokens_ptr = (char**) calloc(str_num, sizeof(char*));
    char** tokens = *tokens_ptr;
    char* cur_token = NULL;

    cur_token = strtok(string, delimiters);
    while (cur_token) {
        tokens[*tokensCount] = cur_token;
        (*tokensCount)++;
        cur_token = strtok(NULL, delimiters);
    }
    tokens[*tokensCount] = cur_token;
}

void PrintTokens (char** tokens, int tokensCount) {
    int i = 0;

    for (; i < tokensCount; i++)
        printf("%s\n", tokens[i]);

}
int main() {
    char* string = (char*)malloc(sizeof(char) * 256);
    char* delimiters = (char*)malloc(sizeof(char) * 256);
    char **tokens = NULL;
    int tokensCount = 0;
    int i = 0;

    gets (string);
    gets (delimiters);
    Split(string, delimiters, &tokens, &tokensCount);
    PrintTokens(tokens, tokensCount);
    printf("%d", tokensCount);
    for (; i < tokensCount; i++)
        free (tokens [i]);
    free (tokens);
    return 0;

}
