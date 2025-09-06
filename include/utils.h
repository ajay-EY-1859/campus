#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// Safe input functions
bool safeGetInt(int *value, int min, int max);
bool safeGetString(char *buffer, size_t size);
void clearInputBuffer(void);

// Logging functions
void logEvent(const char *userID, const char *event);

// Utility functions
void getCurrentTime(char *buffer, size_t size);
bool isValidEmail(const char *email);
bool isValidMobile(const char *mobile);
void getProfilePath(char *path, const char *userID);
void sanitizeFilename(char *str);

#endif // UTILS_H