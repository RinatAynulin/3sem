#include <stdio.h>
#include <string.h>
#include <malloc.h>

void Split(char* string, char* delimiters, char*** tokens_ptr, int* tokensCount) {
  /*
   * FIXIT:
   * Вынесите выделение памяти вне ф-и split. Почему? Допустим вы хотите оформить ф-ю в отдельную библиотеку, тогда пользователь увидит только
   * сигнатуру ф-и. Ему надо будет догадаться каким-то образом, что ему память выделть не надо, но освободить он её должен. Это не круто, поэтому если вы освободаете память, то вы и должны были её выделить.
   * По крайней мере так делать крайне желательно.
   */
  
    /*
     * лучше явно подчёркивать, то данная переменная является константой с помощью слова const
     */
  
    const int str_num = 30;
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

/*
 * Очень хорошо, что вы сделали дополнительную ф-ю для вывода результата с понятным названием. 
 * Это делает код самодокументируемым.
 */

void PrintTokens (char** tokens, int tokensCount) {
    int i = 0;

    for (; i < tokensCount; i++)
        printf("%s\n", tokens[i]);

}
int main() {
    /*
     * FIXIT:
     * Надо объявить две константы MaxStringSize и MaxDelimetersCount вместо магических чисел 256
     */
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
    /*
     * FIXIT:
     * В каком месте вы выделяли память под tokens[i]?
     * Вероятно не выделяли явно. Тогда где хранятся слова? Слова хранятся непосредственно в string (метод strtok её модифицирует, добавляя в нужные места символы конца строки).
     * Из-за того, что вы пытаетесь освободить память, которую не выделяли, у меня, например, программа падает с segmentation fault`ом.
     */
    
    for (; i < tokensCount; i++)
        free (tokens [i]);
    free (tokens);
    /*
     * FIXIT:
     * Освободите уж всю память, которую выделили
     */
    return 0;
}
