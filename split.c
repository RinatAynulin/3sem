#include <stdio.h>
#include <string.h>
#include <malloc.h>

void Split(char* string, char* delimiters, char*** tokens_ptr, int* tokensCount) {
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
    const int MaxStringSize = 256;
    const int MaxDelimetersCount = 256;
    const int str_num = 30;
    char* string = (char*)malloc(sizeof(char) * MaxStringSize);
    char* delimiters = (char*)malloc(sizeof(char) * MaxDelimetersCount);
    char **tokens = (char**) calloc(str_num, sizeof(char*));
    int tokensCount = 0;
    int i = 0;


    gets (string);
    gets (delimiters);
    Split(string, delimiters, &tokens, &tokensCount);
    PrintTokens(tokens, tokensCount);
    printf("%d", tokensCount);
 
    free (tokens);
    free (string);
    free (delimiters);
    return 0;
}
