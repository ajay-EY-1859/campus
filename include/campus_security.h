#ifndef SECURITY_H
#define SECURITY_H

#include "config.h"

// Authentication levels
typedef enum {
    AUTH_LEVEL_BASIC = 1,
    AUTH_LEVEL_ENHANCED = 2,
    AUTH_LEVEL_ADMIN = 3
} AuthLevel;

// Session management
typedef struct {
    char userID[20];
    char sessionToken[64];
    time_t loginTime;
    time_t lastActivity;
    AuthLevel authLevel;
    int isActive;
} Session;

// Advanced authentication functions
int sendOTPSMS(const char *mobile, const char *otp);
int sendOTPEmail(const char *email, const char *otp);
int generateOTP(const char *userID, char *otp);
int verifyOTP(const char *userID, const char *otp);

// Session management
int createSession(const char *userID, AuthLevel level, Session *session);
int validateSession(const char *sessionToken, Session *session);
int updateSessionActivity(const char *sessionToken);
int destroySession(const char *sessionToken);
int cleanupExpiredSessions(void);

// Security features
int checkPasswordStrength(const char *password);
int isAccountLocked(const char *userID);
int lockAccount(const char *userID, int durationMinutes);
int unlockAccount(const char *userID);

// Encryption utilities
void encryptData(const char *data, char *encrypted, const char *key);
void decryptData(const char *encrypted, char *data, const char *key);
void generateSecureHash(const char *input, char *hash);

// Audit and monitoring
int logSecurityEvent(const char *userID, const char *event, const char *details);
int detectSuspiciousActivity(const char *userID);
int generateSecurityReport(const char *reportPath);

#endif // SECURITY_H