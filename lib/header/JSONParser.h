#ifndef BADJSON_PARSER_H
#define BADJSON_PARSER_H

typedef struct Type {
    u_char integer: 1;
    u_char decimal: 1;
    u_char str: 1;
    u_char boolean: 1;
    u_char arr: 1;
    u_char obj: 1;
} Type;

typedef struct JSON_Data {
    char *key;
    void *value;
    Type type;
    struct JSON_Data *next;
} JSON_Data;

JSON_Data *jsonp_parse(char *file_content);
void jsonp_print_data(JSON_Data *root);
void jsonp_free(JSON_Data *root);

#endif // BADJSON_PARSER_H

