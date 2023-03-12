#include "../header/Logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#if ENABLED
char *get_level(unsigned char level) {
    switch (level) {
        case WARN:
            return "WARN";
        case ERROR:
            return "ERROR";
        case INFO:
            return "INFO";
        case DEBUG:
            return "DEBUG";
        case TRACE:
            return "TRACE";
        default:
            printf("ERROR - Logger.c - get_level - Unknown log level=%d, shutting down...", level);
            exit(1);
    }
}
#endif

void logr_log(unsigned char level, char *src_file, char *src_func, char *msg, ...) {
#if ENABLED
    va_list args;
    if (LOG_LEVEL >= level) {
        printf("%s - %s - %s - ", get_level(level), src_file, src_func);
        va_start(args, msg);
        while (*msg != '\0') {
            if (*msg != '%') {
                putchar(*msg);
                msg++;
                continue;
            }
            ++msg;
            switch (*msg) {
                case 's':
                    printf("%s", args);
                    break;
                case 'd':
                    printf("%d", args);
                    break;
                case 'X':
                case 'x':
                    printf("%X", args);
                    break;
                case 'c':
                    printf("%c", args);
                    break;
                case 'p':
                    printf("%d", args);
                    break;
            }
            msg++;
        }
        putchar('\n');
        va_end(args);
    }
#endif
}