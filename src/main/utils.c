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
void getProfilePath(char *path, const char *userID) {
    char ext[6] = {0};
    buildProfileExt(ext, userID);
    snprintf(path, 150, CRED_DIR "%s.%s", userID, ext);
}

// Logging function
void logEvent(const char *userID, const char *action) {
    FILE *log = fopen(LOG_DIR "login_audit.log", "a");
    if (!log) {
        // Optional fallback: stderr output if logging fails
        fprintf(stderr, "Logging failed for %s [%s]\n", action, userID);
        return;
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
}

// Optional utility: sanitize filename (for backups etc.)
void sanitizeFilename(char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isalnum(str[i]) && str[i] != '_' && str[i] != '-') {
            str[i] = '_';  // Replace unsupported chars
        }
    }
}

// Email validation function
bool isValidEmail(const char *email) {
    if (!email || strlen(email) < 5) return false;
    
    const char *at = strchr(email, '@');
    if (!at || at == email) return false;
    
    const char *dot = strchr(at, '.');
    if (!dot || dot == at + 1 || dot[1] == '\0') return false;
    
    return true;
}

// Mobile validation function
bool isValidMobile(const char *mobile) {
    if (!mobile) return false;
    
    size_t len = strlen(mobile);
    if (len < 10 || len > 15) return false;
    
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(mobile[i]) && mobile[i] != '+' && mobile[i] != '-' && mobile[i] != ' ') {
            return false;
        }
    }
    
    return true;
}



// Get current time as string
void getCurrentTime(char *buffer, size_t size) {
    if (!buffer || size == 0) return;
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    if (tm_info) {
        strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
    } else {
        snprintf(buffer, size, "Unknown time");
    }
}
