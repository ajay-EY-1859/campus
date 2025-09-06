#ifndef DATABASE_H
#define DATABASE_H

#include "config.h"
#include "auth.h"

// Database initialization
ErrorCode initDatabase(void);
ErrorCode closeDatabase(void);

// User management
ErrorCode createUser(const Profile *profile);
ErrorCode getUserByID(const char *userID, Profile *profile);
ErrorCode updateUser(const Profile *profile);
ErrorCode deleteUser(const char *userID);
ErrorCode authenticateUser(const char *userID, const char *mobile, const char *passwordHash);
ErrorCode recoverUserID(void);
int isEmailAlreadyRegistered(const char *email);
int isMobileAlreadyRegistered(const char *mobile);

// Data management
ErrorCode saveUserData(const char *userID, const char *dataType, const void *data, size_t dataSize);
ErrorCode loadUserData(const char *userID, const char *dataType, void *data, size_t *dataSize);

// Security & Audit
ErrorCode logActivity(const char *userID, const char *action, const char *details);
int getLoginAttempts(const char *userID);
ErrorCode resetLoginAttempts(const char *userID);
int incrementLoginAttempts(const char *userID);

// Utility functions
ErrorCode executeQuery(const char *query);
ErrorCode backupDatabase(const char *backupPath);
ErrorCode restoreDatabase(const char *backupPath);

#endif // DATABASE_H