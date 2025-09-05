#ifndef DATABASE_H
#define DATABASE_H

#include "config.h"
#include "auth.h"

// Database initialization
int initDatabase(void);
void closeDatabase(void);

// User management
int createUser(const Profile *profile);
int getUserByID(const char *userID, Profile *profile);
int updateUser(const Profile *profile);
int deleteUser(const char *userID);
int authenticateUser(const char *userID, const char *mobile, const char *passwordHash);

// Data management
int saveUserData(const char *userID, const char *dataType, const void *data, size_t dataSize);
int loadUserData(const char *userID, const char *dataType, void *data, size_t *dataSize);

// Security & Audit
int logActivity(const char *userID, const char *action, const char *details);
int getLoginAttempts(const char *userID);
int resetLoginAttempts(const char *userID);
int incrementLoginAttempts(const char *userID);

// Utility functions
int executeQuery(const char *query);
int backupDatabase(const char *backupPath);
int restoreDatabase(const char *backupPath);

#endif // DATABASE_H