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
        fprintf(log, "%s: %s at %s", action, userID, timeStr);
    } else {
        fprintf(log, "%s: %s at [time unavailable]\n", action, userID);
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
