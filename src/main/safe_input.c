#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/utils.h"

ErrorCode clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return SUCCESS;
}

ErrorCode safeGetInt(int *value, int min, int max) {
    char buffer[32] = {0};
    char *endptr = NULL;
    long temp = 0;
    
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Remove newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    temp = strtol(buffer, &endptr, 10);
    
    // Check for conversion errors
    if (endptr == buffer || *endptr != '\0') {
        return ERROR_INVALID_INPUT;
    }
    
    // Check range
    if (temp < min || temp > max) {
        return ERROR_INVALID_INPUT;
    }
    
    *value = (int)temp;
    return SUCCESS;
}

ErrorCode safeGetString(char *buffer, size_t size) {
    if (!buffer || size == 0) {
        return ERROR_INVALID_INPUT;
    }
    
    if (!fgets(buffer, (int)size, stdin)) {
        return ERROR_INVALID_INPUT;
    }
    
    // Remove newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    return SUCCESS;
}