#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/utils.h"

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool safeGetInt(int *value, int min, int max) {
    char buffer[32] = {0};
    char *endptr = NULL;
    long temp = 0;
    
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return false;
    }
    
    // Remove newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    temp = strtol(buffer, &endptr, 10);
    
    // Check for conversion errors
    if (endptr == buffer || *endptr != '\0') {
        return false;
    }
    
    // Check range
    if (temp < min || temp > max) {
        return false;
    }
    
    *value = (int)temp;
    return true;
}

bool safeGetString(char *buffer, size_t size) {
    if (!buffer || size == 0) {
        return false;
    }
    
    if (!fgets(buffer, size, stdin)) {
        return false;
    }
    
    // Remove newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    return true;
}