#pragma once
#include <inttypes.h>

typedef enum operator_type OperatorType;
typedef enum token_type TokenType;
typedef struct number Number;
typedef struct operator Operator;
typedef struct token_node TokenNode;
typedef struct token_list TokenList;
typedef struct token Token;

enum operator_type
{
    O_SUM,
    O_MINUS
};

enum token_type
{
    T_OPT,
    T_NUM
};

struct token
{
    TokenType type;
    union
    {
        uint16_t number;
        OperatorType operator;
    } data;
};

struct token_node
{
    Token *data;
    TokenNode *next;
};

struct token_list
{
    size_t size;
    TokenNode *head;
};

int init_token_list(TokenList **dest);
Token *get_token_from_list(const TokenList *src, size_t index);
int token_list_append(TokenList *list, Token **data);
void free_token_list(TokenList **token_list);

int concat_args(char **dest, int argc, char* argv[]);
int parse_token(TokenList **dest, const char *args);
int calculate(int *target, const TokenList *list);