#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../include/utils.h"
#include "../include/config.h"
#include "../include/student.h"
#include "../include/ui.h"
#include "../include/fileio.h"
#include "../include/auth.h"
#include "../include/hpdf/hpdf.h"

// Helper: build extension from studentID initials
void buildProfileExt(char *ext, const char *userID) {
    if (strlen(userID) >= 2) {
        ext[0] = (char)tolower(userID[0]);
        ext[1] = (char)tolower(userID[1]);
    } else {
        ext[0] = 'x';
        ext[1] = 'x';
    }
    strncpy(&ext[2], "pfx", 3);
    ext[5] = '\0';
}

// Helper: build full file path from ID
ErrorCode getProfilePath(char *path, const char *userID) {
    if (!path || !userID) return ERROR_INVALID_INPUT;
    
    char ext[6] = {0};
    buildProfileExt(ext, userID);
    int written = snprintf(path, 150, CRED_DIR "%s.%s", userID, ext);
    if (written < 0 || written >= 150) return ERROR_INVALID_INPUT;
    return SUCCESS;
}

// Logging function
ErrorCode logEvent(const char *userID, const char *action) {
    if (!userID || !action) return ERROR_INVALID_INPUT;
#ifdef _WIN32
    _mkdir("logs");
#else
    mkdir("logs", 0700);
#endif
    FILE *log = fopen(LOG_DIR "login_audit.log", "a");
    if (!log) {
        fprintf(stderr, "Logging failed for %s [%s]\n", action ? action : "UNKNOWN", userID ? userID : "UNKNOWN");
        return ERROR_FILE_IO;
    }
    time_t now = time(NULL);
    char *timeStr = ctime(&now);
    if (timeStr) {
        fprintf(log, "%s: %s at %s", action ? action : "UNKNOWN_ACTION", 
                userID ? userID : "UNKNOWN_USER", timeStr);
    } else {
        fprintf(log, "%s: %s at [time unavailable]\n", 
                action ? action : "UNKNOWN_ACTION", userID ? userID : "UNKNOWN_USER");
    }
    fclose(log);
    return SUCCESS;
}

// Optional utility: sanitize filename (for backups etc.)
ErrorCode sanitizeFilename(char *str) {
    if (!str) return ERROR_INVALID_INPUT;
    
    for (int i = 0; str[i]; i++) {
        if (!isalnum(str[i]) && str[i] != '_' && str[i] != '-') {
            str[i] = '_';  // Replace unsupported chars
        }
    }
    return SUCCESS;
}

// Email validation function
ErrorCode isValidEmail(const char *email) {
    if (!email || strlen(email) < 5) return ERROR_INVALID_INPUT;
    
    const char *at = strchr(email, '@');
    if (!at || at == email) return ERROR_INVALID_INPUT;
    
    const char *dot = strchr(at, '.');
    if (!dot || dot == at + 1 || dot[1] == '\0') return ERROR_INVALID_INPUT;
    
    return SUCCESS;
}

// Mobile validation function
ErrorCode isValidMobile(const char *mobile) {
    if (!mobile) return ERROR_INVALID_INPUT;
    
    size_t len = strlen(mobile);
    if (len < 10 || len > 15) return ERROR_INVALID_INPUT;
    
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(mobile[i]) && mobile[i] != '+' && mobile[i] != '-' && mobile[i] != ' ') {
            return ERROR_INVALID_INPUT;
        }
    }
    
    return SUCCESS;
}



// Get current time as string
ErrorCode getCurrentTime(char *buffer, size_t size) {
    if (!buffer || size == 0) return ERROR_INVALID_INPUT;
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    if (tm_info) {
        strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
    } else {
        snprintf(buffer, size, "Unknown time");
        return ERROR_GENERAL;
    }
    return SUCCESS;
}



