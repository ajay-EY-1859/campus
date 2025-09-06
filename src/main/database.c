#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/database.h"
#include "../include/config.h"

// Simple file-based database simulation (SQLite alternative)
static FILE *dbFile = NULL;
static const char *DB_PATH = "data/campus.db";
#define HEADER_SIZE 12
#define USER_RECORD_HEADER "USER_RECORD"

int initDatabase(void) {
    // Create data directory if not exists
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif

    // Initialize database file
    dbFile = fopen(DB_PATH, "a+");
    if (!dbFile) {
        printf("Failed to initialize database\n");
        return 0;
    }
    fclose(dbFile);
    
    printf("Database initialized successfully\n");
    return 1;
}

void closeDatabase(void) {
    if (dbFile) {
        fclose(dbFile);
        dbFile = NULL;
    }
}

int createUser(const Profile *profile) {
    if (!profile) return 0;
    
    FILE *f = fopen(DB_PATH, "ab");
    if (!f) {
        printf("Error: Cannot open database for writing\n");
        return 0;
    }
    
    // Write user record with header
    char header[] = USER_RECORD_HEADER;
    if (fwrite(header, sizeof(header), 1, f) != 1 ||
        fwrite(profile, sizeof(Profile), 1, f) != 1) {
        printf("Error: Failed to write user data\n");
        fclose(f);
        return 0;
    }
    
    fclose(f);
    logActivity(profile->userID, "USER_CREATED", "New user registered");
    return 1;
}

int getUserByID(const char *userID, Profile *profile) {
    FILE *f = fopen(DB_PATH, "rb");
    if (!f) return 0;
    
    char header[HEADER_SIZE] = {0};
    Profile temp = {0};
    
    while (fread(header, sizeof(header), 1, f) == 1) {
        if (strcmp(header, USER_RECORD_HEADER) == 0) {
            if (fread(&temp, sizeof(Profile), 1, f) == 1) {
                if (strcmp(temp.userID, userID) == 0) {
                    *profile = temp;
                    fclose(f);
                    return 1;
                }
            }
        } else {
            // Skip non-user records
            fseek(f, sizeof(Profile), SEEK_CUR);
        }
    }
    
    fclose(f);
    return 0;
}

int updateUser(const Profile *profile) {
    if (!profile) return 0;
    
    // Simple implementation: delete old, add new
    FILE *f = fopen(DB_PATH, "rb");
    FILE *temp = fopen("data/temp.db", "wb");
    if (!f || !temp) {
        if (f) fclose(f);
        if (temp) fclose(temp);
        printf("Error: Cannot open database files for update\n");
        return 0;
    }
    
    char header[12];
    Profile tempProfile;
    int updated = 0;
    
    while (fread(header, sizeof(header), 1, f) == 1) {
        if (strcmp(header, "USER_RECORD") == 0) {
            if (fread(&tempProfile, sizeof(Profile), 1, f) == 1) {
                if (strcmp(tempProfile.userID, profile->userID) == 0) {
                    // Write updated record
                    fwrite(header, sizeof(header), 1, temp);
                    fwrite(profile, sizeof(Profile), 1, temp);
                    updated = 1;
                } else {
                    // Write original record
                    fwrite(header, sizeof(header), 1, temp);
                    fwrite(&tempProfile, sizeof(Profile), 1, temp);
                }
            }
        } else {
            // Copy other records as-is
            fwrite(header, sizeof(header), 1, temp);
            fread(&tempProfile, sizeof(Profile), 1, f);
            fwrite(&tempProfile, sizeof(Profile), 1, temp);
        }
    }
    
    fclose(f);
    fclose(temp);
    
    if (updated) {
        remove(DB_PATH);
        rename("data/temp.db", DB_PATH);
        logActivity(profile->userID, "USER_UPDATED", "Profile updated");
        return 1;
    }
    
    remove("data/temp.db");
    return 0;
}

int authenticateUser(const char *userID, const char *mobile, const char *passwordHash) {
    Profile profile;
    if (!getUserByID(userID, &profile)) {
        return 0;
    }
    
    if (strcmp(profile.mobile, mobile) == 0 && 
        strcmp(profile.passwordHash, passwordHash) == 0) {
        logActivity(userID, "LOGIN_SUCCESS", "User authenticated");
        resetLoginAttempts(userID);
        return 1;
    }
    
    incrementLoginAttempts(userID);
    logActivity(userID, "LOGIN_FAILED", "Authentication failed");
    return 0;
}

int saveUserData(const char *userID, const char *dataType, const void *data, size_t dataSize) {
    if (!userID || !dataType || !data || dataSize == 0) return 0;
    
    char filename[200] = {0};
    snprintf(filename, sizeof(filename), "data/%s_%s.dat", userID, dataType);
    
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Error: Cannot save data to %s\n", filename);
        return 0;
    }
    
    if (fwrite(data, dataSize, 1, f) != 1) {
        printf("Error: Failed to write data\n");
        fclose(f);
        return 0;
    }
    
    fclose(f);
    logActivity(userID, "DATA_SAVED", dataType);
    return 1;
}

int loadUserData(const char *userID, const char *dataType, void *data, size_t *dataSize) {
    if (!userID || !dataType || !data || !dataSize) return 0;
    
    char filename[200] = {0};
    snprintf(filename, sizeof(filename), "data/%s_%s.dat", userID, dataType);
    
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return 0;
    }
    
    long fileSize = ftell(f);
    if (fileSize < 0) {
        fclose(f);
        return 0;
    }
    
    *dataSize = (size_t)fileSize;
    
    if (fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return 0;
    }
    
    if (fread(data, *dataSize, 1, f) != 1) {
        fclose(f);
        return 0;
    }
    
    fclose(f);
    return 1;
}

int logActivity(const char *userID, const char *action, const char *details) {
    FILE *f = fopen("data/audit.log", "a");
    if (!f) return 0;
    
    time_t now = time(NULL);
    char *timeStr = ctime(&now);
    if (timeStr) {
        timeStr[strlen(timeStr) - 1] = '\0'; // Remove newline
    }
    
    fprintf(f, "[%s] User: %s | Action: %s | Details: %s\n", 
            timeStr ? timeStr : "Unknown", userID, action, details);
    fclose(f);
    return 1;
}

int getLoginAttempts(const char *userID) {
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_attempts.dat", userID);
    
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    
    int attempts = 0;
    fread(&attempts, sizeof(int), 1, f);
    fclose(f);
    return attempts;
}

int resetLoginAttempts(const char *userID) {
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_attempts.dat", userID);
    
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    
    int attempts = 0;
    fwrite(&attempts, sizeof(int), 1, f);
    fclose(f);
    return 1;
}

int incrementLoginAttempts(const char *userID) {
    int attempts = getLoginAttempts(userID);
    attempts++;
    
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_attempts.dat", userID);
    
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    
    fwrite(&attempts, sizeof(int), 1, f);
    fclose(f);
    return attempts;
}

int backupDatabase(const char *backupPath) {
    char command[300];
    snprintf(command, sizeof(command), "copy \"%s\" \"%s\"", DB_PATH, backupPath);
    return system(command) == 0;
}

int restoreDatabase(const char *backupPath) {
    char command[300];
    snprintf(command, sizeof(command), "copy \"%s\" \"%s\"", backupPath, DB_PATH);
    return system(command) == 0;
}