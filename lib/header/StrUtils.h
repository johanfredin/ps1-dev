#ifndef PSX_STR_UTILS_H
#define PSX_STR_UTILS_H

#include <ctype.h>
#include <string.h>

#define STR_EQ(str1, str2) strcmp(str1, str2) == 0

#define STR_NEQ(str1, str2) strcmp(str1, str2) != 0

#define SET_STR_LEN(i, str)      \
    while ((char)str[i] != '\0') \
    i++

#define STR_TO_LOWERCASE(str) \
    char *p;                  \
    for (p = str; *p; p++)    \
    *p = tolower(*p)

#define STR_READ_UNTIL(str, substr, stopchr, count)                         \
    for (count = 0; str[count] != stopchr && str[count] != '\0'; count++) { \
        substr[count] = str[count];                                         \
    }                                                                       \
    substr[count] = '\0'

#define STR_CONTAINS(str_to_check, str_val_to_find) STR_NEQ(str_to_check, "") && strstr(str_to_check, str_val_to_find) != NULL

#endif //PSX_STR_UTILS_H
