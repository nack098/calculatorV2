#include "calculator.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int concat_args(char** dest, const int argc, char* argv[])
{
    int error = 0;
    size_t arg_size = 0;
    [[maybe_unused]]char *args = nullptr;

    for (int i = 1; i < argc; ++i)
        arg_size += strlen(argv[i]) + 1;

    args = malloc(arg_size);

    if (args == nullptr)
    {
        fprintf(stderr, "Out of memory\n");
        error = 1;
        goto exit;
    }

    strcpy(args, argv[1]);
    for (int i = 2; i < argc; ++i)
    {
        strcat(args, " ");
        strcat(args, argv[i]);
    }
    (*dest) = args;

exit:
    return error;
}

int init_token_list(TokenList **dest)
{
    int error = 0;
    [[maybe_unused]]TokenList *list = nullptr;
    [[maybe_unused]]TokenNode *head = nullptr;

    list = malloc(sizeof(*list));
    if (list == nullptr)
    {
        error = 1;
        goto exit;
    }

    head = malloc(sizeof(*head));
    if (head == nullptr)
    {
        error = 1;
        goto clean_dest;
    }

    head->next = nullptr;
    head->data = nullptr;

    list->head = head;
    list->size = 0;

    (*dest) = list;

    goto exit;

clean_dest:
    free(list);
exit:
    return error;
}

Token *get_token_from_list(const TokenList *src, const size_t index)
{
    if (index >= src->size) {
        fprintf(stderr, "Index out of range\n");
        exit(EXIT_FAILURE);
    }

    const TokenNode *current = src->head->next;
    for (size_t i = 0; i < index; ++i)
        current = current->next;

    return current->data;
}

void free_token_list(TokenList **token_list)
{
    if (token_list == nullptr || *token_list == nullptr) return;

    TokenList *list = *token_list;
    TokenNode *current = list->head->next;
    while (current != nullptr) {
        TokenNode *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }

    free(list->head);
    free(list);

    *token_list = nullptr;
}

int token_list_append(TokenList *list, Token **data)
{
    int error = 0;
    [[maybe_unused]]TokenNode *new_node = nullptr, *current = nullptr;

    if (data == nullptr) {
        fprintf(stderr, "Attempted to append null token\n");
        error = 1;
        goto exit;
    }

    new_node = malloc(sizeof(*new_node));
    if (new_node == nullptr) {
        fprintf(stderr, "Out of memory\n");
        error = 1;
        goto exit;
    }

    new_node->data = *data;
    new_node->next = nullptr;

    current = list->head;
    while (current->next != nullptr)
        current = current->next;

    current->next = new_node;
    list->size++;

exit:
    return error;
}

typedef enum parser_state
{
    P_EMPTY,
    P_CHAR,
    P_NUM,
} ParserState;

int parse_token(TokenList **dest, const char *args)
{
    int error = 0, i = 0;
    ParserState state = P_EMPTY;
    [[maybe_unused]]char c = args[0];
    [[maybe_unused]]Token *token = nullptr;
    size_t arg_size = 0;

    arg_size = strlen(args);
    if (arg_size == 0)
    {
        error = 1;
        goto exit;
    }

    i = 0;
    while (i < arg_size)
    {
        c = args[i];
        switch (state)
        {
        case P_EMPTY:
            if (token != nullptr)
            {
                token_list_append(*dest, &token);
            }
            if (isspace(c))
            {
                ++i;
            }
            else
            {
                state = P_CHAR;
            }
            break;
        case P_CHAR:
            token = malloc(sizeof(*token));
            if (isdigit(c))
            {
                token->type = T_NUM;
                token->data.number = 0;

                state = P_NUM;
            }
            else if (c == '+')
            {
                token->type = T_OPT;
                token->data.operator = O_SUM;
                ++i;

                state = P_EMPTY;
            }
            else if (c == '-')
            {
                token->type = T_OPT;
                token->data.operator = O_MINUS;
                ++i;

                state = P_EMPTY;
            }
            else
            {
                free(token);
                error = 1;
                goto exit;
            }
            break;
        case P_NUM:
            if (isdigit(c))
            {
                token->data.number *= 10;
                token->data.number += c - '0';
                ++i;
            }
            else
            {
                state = P_EMPTY;
            }
        }
    }

    if (token != nullptr)
    {
        token_list_append(*dest, &token);
    }

exit:
    return error;
}

int calculate(int *target, const TokenList *list)
{
    int error = 0, result = 0;
    OperatorType current_opt = O_SUM;
    const TokenNode *current = list->head;


    while (true)
    {
        current = current->next;
        if (current == nullptr) break;

        switch (current->data->type)
        {
        case T_NUM:
            if (current_opt == O_SUM)
            {
                result += current->data->data.number;
            }
            else if (current_opt == O_MINUS)
            {
                result -= current->data->data.number;
            }
            else
            {
                result = 0;
                error = 1;
                goto exit;
            }
        case T_OPT:
            current_opt = current->data->data.operator;
        }
    }

    (*target) = result;

exit:
    return error;
}