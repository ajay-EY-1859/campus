#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../include/utils.h"
#include "../include/config.h"
#include"../include/student.h"
#include"../include/ui.h"
#include"../include/fileio.h"
#include"../include/auth.h"
#include"../include/hpdf.h"

// Helper: build extension from studentID initials
void buildProfileExt(char *ext, const char *studentID) {
    ext[0] = tolower(studentID[0]);
    ext[1] = tolower(studentID[1]);
    strcpy(&ext[2], "pfx");
}

// Helper: build full file path from ID
void getProfilePath(char *path, const char *studentID) {
    char ext[6];
    buildProfileExt(ext, studentID);
    sprintf(path, CRED_DIR "%s.%s", studentID, ext);
}

// Logging function
void logEvent(const char *studentID, const char *action) {
    FILE *log = fopen(LOG_DIR "login_audit.log", "a");
    if (!log) {
        // Optional fallback: stderr output if logging fails
        fprintf(stderr, "⚠️ Logging failed for %s [%s]\n", action, studentID);
        return;
    }
    time_t now = time(NULL);
    fprintf(log, "%s: %s at %s", action, studentID, ctime(&now));
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
