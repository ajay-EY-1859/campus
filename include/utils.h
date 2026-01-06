#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "config.h"
#include "auth.h"

// Safe input functions
ErrorCode safeGetInt(int *value, int min, int max);
ErrorCode safeGetString(char *buffer, size_t size);
ErrorCode clearInputBuffer(void);

// Logging functions
ErrorCode logEvent(const char *userID, const char *event);

// Utility functions
ErrorCode getCurrentTime(char *buffer, size_t size);
ErrorCode isValidEmail(const char *email);
ErrorCode isValidMobile(const char *mobile);
ErrorCode getProfilePath(char *path, const char *userID);
ErrorCode sanitizeFilename(char *str);
void generateUserID(Profile *p);

#endif // UTILS_H