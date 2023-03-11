#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#include "../header/JSONParser.h"
#include "../header/StrUtils.h"
#include "../header/MemUtils.h"
#include "../header/Logger.h"

unsigned int idx;
char *content = NULL;

void get_value(JSON_Data *);
void get_str(JSON_Data *entry, u_char type);
void get_array(JSON_Data *);
void get_object(JSON_Data *);
void get_number(JSON_Data *);
void get_bool(JSON_Data *);
void print_recursive(JSON_Data *root, int indents);
void ff_to_start();
char skip_irrelevant_chars();
char *get_numeric_str();
char *read_until(char end_char, u_char include_end_char);
char curr_ch();
char next_ch();
JSON_Data *malloc_json_entry();
u_char is_num();

JSON_Data *jsonp_parse(char *file_content) {
    // Calc length of content
    JSON_Data *root, *curr;
    int content_length = 0;

    // Set idx to 0 in this function so that it resets in case we need to parse several json files
    idx = 0;

    SET_STR_LEN(content_length, file_content);

    content = file_content;
    root = malloc_json_entry();
    curr = root;
    ff_to_start();
    while (idx < content_length) {
        u_int next_idx;
        char next_char;
        skip_irrelevant_chars();

        get_str(curr, 'k');
        skip_irrelevant_chars();
        get_value(curr);
        skip_irrelevant_chars();

        // Go to next if we have not reached the end yet
        next_idx = idx + 1;
        next_char = content[next_idx];
        if (next_idx >= content_length || next_char == '\0' || (next_char == '}' && content[++next_idx] == '\0')) {
            break;
        }
        curr->next = malloc_json_entry();
        curr = curr->next;
        skip_irrelevant_chars();
        idx++;
    }

    return root;
}

void jsonp_print_data(JSON_Data *root) {
    printf("{\n");
    print_recursive(root, 2);
    printf("}\n");
}

void jsonp_free(JSON_Data *root) {
    JSON_Data *curr = root;
    u_char is_nested;
    while (curr != NULL) {
        JSON_Data *aux;
        is_nested = curr->type.arr | curr->type.obj;
        if (is_nested) {
            jsonp_free((JSON_Data *)curr->value);
        }
        aux = curr;
        curr = curr->next;
        if (is_nested) {
            aux->value = NULL;
        } else {
            MEM_FREE_3_AND_NULL(aux->value);
        }
        MEM_FREE_3_AND_NULL(aux->key);
        MEM_FREE_3_AND_NULL(aux);
    }
}

void print_recursive(JSON_Data *root, int indents) {
    int i;
    JSON_Data *curr;
    char padding[32];
    for (i = 0; i < indents; i++) {
        padding[i] = ' ';
    }
    padding[i] = '\0'; // Add null terminator

    for (curr = root; curr != NULL; curr = curr->next) {
        char *comma = curr->next == NULL ? "" : ",";
        printf("%s", padding);
        if (curr->key != NULL) {
            printf("\"%s\": ", curr->key);
        }
        // Check value
        if (curr->type.str) {
            printf("\"%s\"%s\n", (char *) curr->value, comma);
        } else if (curr->type.decimal) {
            printf("%.2f%s\n", *((float *) curr->value), comma);
        } else if (curr->type.integer) {
            printf("%i%s\n", *((int *) curr->value), comma);
        } else if (curr->type.boolean) {
            printf("%s%s\n", *((unsigned char *) curr->value) == 1 ? "true" : "false", comma);
        } else if (curr->type.arr) {
            printf("[\n");
            print_recursive((JSON_Data *) curr->value, indents + 2);
            printf("%s]%s\n", padding, comma);
        } else if (curr->type.obj) {
            printf("{\n");
            print_recursive((JSON_Data *) curr->value, indents + 2);
            printf("%s}%s\n", padding, comma);
        }
    }
}

void get_value(JSON_Data *entry) {
    char c = curr_ch();
    if (c == '\"') {
        get_str(entry, 'v');
    } else if (c == '[') {
        idx++;
        skip_irrelevant_chars();
        get_array(entry);
    } else if (c == '{') {
        idx++;
        skip_irrelevant_chars();
        get_object(entry);
    } else if (is_num()) {
        get_number(entry);
    } else if (c == 't' || c == 'f') {
        get_bool(entry);
    } else {
        idx++;
        skip_irrelevant_chars();
        get_value(entry);
    }
}

void get_str(JSON_Data *entry, u_char type) {
    char *str = NULL;
    if (curr_ch() == '\"') {
        next_ch();
    }
    str = read_until('\"', 0);
    if (type == 'k') {
        entry->key = str;
    } else if (type == 'v') {
        entry->value = str;
        entry->type.str = 1;
    } else {
        logr_log(ERROR, "JSONParser.c", "get_str", "value=%c not one of 'k', 'v'\n", type);
        exit(1);
    }
}

void get_number(JSON_Data *entry) {
    char *num_str = get_numeric_str();
    int *i_ptr;
    if (strchr(num_str, '.')) {
        logr_log(TRACE, "JSONParser.c", "get_number", "Floating point not implemented for ps1 and will not be parsed, key=%s", entry->key);
    } else {
        i_ptr = MEM_MALLOC_3(int);
        //TODO: FIx
//        *i_ptr = (int) strtol(num_str, NULL, 10);
        entry->value = i_ptr;
        entry->type.integer = 1;
    }
    MEM_FREE_3_AND_NULL(num_str);
}

void get_bool(JSON_Data *entry) {
    char *str_bool = read_until('e', 1);    // true and false both end at 'e'
    u_char *bool_ptr = MEM_MALLOC_3(u_char);
    *bool_ptr = STR_EQ(str_bool, "true");
    MEM_FREE_3_AND_NULL(str_bool);
    str_bool = NULL;
    entry->value = bool_ptr;
    entry->type.boolean = 1;
}

void get_array(JSON_Data *entry) {
    char c = curr_ch();
    JSON_Data *root = malloc_json_entry();
    JSON_Data *curr = root;
    entry->type.arr = 1;
    while (c != ']') {
        JSON_Data *next = NULL;
        skip_irrelevant_chars();
        get_value(curr);
        c = skip_irrelevant_chars();

        if (c == ']') {
            break;
        }

        next = malloc_json_entry();
        curr->next = next;
        curr = curr->next;
    }
    entry->value = root;
    next_ch(); // So that we iterate past the last ] char
}

void get_object(JSON_Data *entry) {
    char c = curr_ch();
    JSON_Data *root = malloc_json_entry();
    JSON_Data *curr = root;
    entry->type.obj = 1;
    while (c != '}') {
        JSON_Data *next = NULL;
        skip_irrelevant_chars();
        get_str(curr, 'k');
        skip_irrelevant_chars();
        get_value(curr);
        c = skip_irrelevant_chars();

        if (c == '}') {
            break;
        }

        next = malloc_json_entry();
        curr->next = next;
        curr = curr->next;
    }
    entry->value = root;
    next_ch(); // So that we iterate past the last } char
}

JSON_Data *malloc_json_entry() {
    Type zeroType = {0, 0, 0, 0, 0, 0};
    JSON_Data *data = MEM_MALLOC_3(JSON_Data);
    if (data == NULL) {
        logr_log(ERROR, "JSONParser.c", "malloc_json_entry", "Unable to allocate memory for JSON_Data entry, exiting...");
        exit(1);
    }
    data->key = NULL;
    data->value = NULL;
    data->next = NULL;
    data->type = zeroType;
    return data;
}

char *read_until(char end_char, u_char include_end_char) {
    const u_char size = 100;
    char *str = MEM_CALLOC_3(size, char);

    u_char i = 0;
    char c = curr_ch();
    while (c != end_char) {
        str[i] = c;
        c = next_ch();

        if (i >= size) {
            logr_log(ERROR, "JSONParser.c", "read_until", "String exceeded max length of %i, accumulated string=%s\n", size, str);
            exit(1);
        }

        i++;
    }
    if (include_end_char) {
        str[i] = end_char;
        i++;
    }
    str[i] = '\0';
    next_ch(); // So that we iterate past the last " char
    return str;
}

char *get_numeric_str() {
    const u_char size = 15;
    u_char i = 0;
    char *str = MEM_CALLOC_3(size, char);
    while (is_num()) {
        str[i] = curr_ch();
        next_ch();
        if (i >= size) {
            logr_log(ERROR, "JSONParser.c", "get_numeric_str", "Numeric string exceeded max length of %i, accumulated string=%s\n", size, str);
            exit(1);
        }
        i++;
    }
    str[i] = '\0';
    return str;
}

u_char is_num() {
    char c = curr_ch();
    u_char i;
    char numbers[] = "0123456789.\0";
    for (i = 0; numbers[i] != '\0'; i++) {
        if (numbers[i] == c) {
            return 1;
        }
    }
    return 0;
}

void ff_to_start() {
    while (content[idx] != '\0') {
        if (content[idx++] == '{') {
            return;
        }
    }
}

char skip_irrelevant_chars() {
    char c = curr_ch();
    while (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == ',') {
        c = next_ch();
    }
    return c;
}

char curr_ch() {
    return content[idx];
}

char next_ch() {
    if (curr_ch() == '\0') {
        return curr_ch();
    }
    return content[++idx];
}