#include "calculator.h"

#include <stdio.h>
#include <stdlib.h>


int main(const int argc, char* argv[])
{
    char *args = nullptr;
    TokenList *token_list = nullptr;
    int error = 0, result = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: calculator [command]\n");
        error = 1;
        goto clean_up;
    }

    error = init_token_list(&token_list);
    if (error)
    {
        fprintf(stderr, "Memory allocation failed for new token list\n");
        goto clean_up;
    }

    error = concat_args(&args, argc, argv);
    if (error)
    {
        fprintf(stderr, "Memory allocation failed for new argument 'args'\n");
        goto clean_up;
    }

    error = parse_token(&token_list, args);
    if (error)
    {
        fprintf(stderr, "Parsing failed for new argument 'args'\n");
        goto clean_up;
    }

    error = calculate(&result, token_list);
    if (error)
    {
        fprintf(stderr, "Calculation failed for command '%s'\n", argv[0]);
        goto clean_up;
    }

    printf("%s\n",args);
    printf("Result: %d\n", result);

clean_up:
    free_token_list(&token_list);
    free(args);
    return error;
}