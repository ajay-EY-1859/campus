#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <ctype.h>
#include "../include/security.h"
#include "../include/database.h"

#define ACCOUNT_LOCK_FILE_PREFIX "data/lock_"
#define MAX_SESSIONS 100
#define SESSION_TIMEOUT 1800
#define MAX_LOGIN_ATTEMPTS 3
#define ACCOUNT_LOCK_DURATION 900

static Session activeSessions[MAX_SESSIONS];
static int sessionCount = 0;

// Sanitize userID to prevent path traversal
static int sanitizeUserID(const char *userID, char *sanitized, size_t size) {
    if (!userID || !sanitized || size == 0) return 0;
    size_t len = strlen(userID);
    if (len == 0 || len >= size) return 0;
    for (size_t i = 0; i < len; i++) {
        char c = userID[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
              (c >= '0' && c <= '9') || c == '_' || c == '-')) {
            return 0;
        }
    }
    strncpy(sanitized, userID, size - 1);
    sanitized[size - 1] = '\0';
    return 1;
}

int isAccountLocked(const char *userID) {
    char sanitized[64];
    if (!sanitizeUserID(userID, sanitized, sizeof(sanitized))) return 0;
    char filename[256];
    snprintf(filename, sizeof(filename), ACCOUNT_LOCK_FILE_PREFIX "%s.dat", sanitized);
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    time_t expiry = 0;
    fread(&expiry, sizeof(time_t), 1, f);
    fclose(f);
    if (time(0) < expiry) {
        return 1;
    } else {
        remove(filename);
        return 0;
    }
}

int lockAccount(const char *userID, int durationMinutes) {
    char sanitized[64];
    if (!sanitizeUserID(userID, sanitized, sizeof(sanitized))) return 0;
    char filename[256];
    snprintf(filename, sizeof(filename), ACCOUNT_LOCK_FILE_PREFIX "%s.dat", sanitized);
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    time_t expiry = time(0) + durationMinutes * 60;
    fwrite(&expiry, sizeof(time_t), 1, f);
    fclose(f);
    logSecurityEvent(userID, "ACCOUNT_LOCKED", "Account locked due to failed attempts");
    return 1;
}

int unlockAccount(const char *userID) {
    char sanitized[64];
    if (!sanitizeUserID(userID, sanitized, sizeof(sanitized))) return 0;
    char filename[256];
    snprintf(filename, sizeof(filename), ACCOUNT_LOCK_FILE_PREFIX "%s.dat", sanitized);
    int res = remove(filename);
    if (res == 0) {
        logSecurityEvent(userID, "ACCOUNT_UNLOCKED", "Account unlocked manually");
        return 1;
    }
    return 0;
}

int checkPasswordStrength(const char *password) {
    int len = (int)strlen(password);
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    if (len < 8) return 0;
    for (int i = 0; i < len && i < 128; i++) {
        if ('A' <= password[i] && password[i] <= 'Z') hasUpper = 1;
        else if ('a' <= password[i] && password[i] <= 'z') hasLower = 1;
        else if ('0' <= password[i] && password[i] <= '9') hasDigit = 1;
        else if (ispunct((unsigned char)password[i])) hasSpecial = 1;
    }
    return hasUpper + hasLower + hasDigit + hasSpecial;
}

int generateOTP(const char *userID, char *otp) {
    unsigned int randomValue = 0;
#ifdef _WIN32
    if (rand_s(&randomValue) != 0) {
        return 0;
    }
#else
    FILE *urnd = fopen("/dev/urandom", "rb");
    if (urnd) {
        if (fread(&randomValue, sizeof(randomValue), 1, urnd) != 1) {
            fclose(urnd);
            return 0;
        }
        fclose(urnd);
    } else {
        srand((unsigned int)time(NULL));
        randomValue = (unsigned int)rand();
    }
#endif
    int otpNum = (int)(randomValue % 900000) + 100000;
    snprintf(otp, 7, "%06d", otpNum);
    
    char sanitized[64];
    if (!sanitizeUserID(userID, sanitized, sizeof(sanitized))) return 0;
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_otp.dat", sanitized);
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif

    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    
    time_t expiry = time(NULL) + 300;
    fwrite(otp, 6, 1, f);
    fwrite(&expiry, sizeof(time_t), 1, f);
    fclose(f);
    
    logSecurityEvent(userID, "OTP_GENERATED", "OTP generated for authentication");
    return 1;
}

int verifyOTP(const char *userID, const char *otp) {
    char sanitized[64];
    if (!sanitizeUserID(userID, sanitized, sizeof(sanitized))) return 0;
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_otp.dat", sanitized);
    
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    
    char storedOTP[7] = {0};
    time_t expiry;
    
    fread(storedOTP, 6, 1, f);
    fread(&expiry, sizeof(time_t), 1, f);
    fclose(f);
    
    if (time(NULL) > expiry) {
        remove(filename);
        logSecurityEvent(userID, "OTP_EXPIRED", "OTP verification failed - expired");
        return 0;
    }
    
    if (strcmp(storedOTP, otp) == 0) {
        remove(filename);
        logSecurityEvent(userID, "OTP_VERIFIED", "OTP verification successful");
        return 1;
    }
    
    logSecurityEvent(userID, "OTP_INVALID", "OTP verification failed - invalid code");
    return 0;
}

#include "../include/send_otp_sms.h"

int sendOTPEmail(const char *email, const char *otp) {
#ifdef _WIN32
    _mkdir("data");
    _mkdir("data\\outbox");
#else
    mkdir("data", 0700);
    mkdir("data/outbox", 0700);
#endif

    FILE *f = fopen("data/outbox/email.out", "a");
    if (f) {
        time_t now = time(NULL);
        char *timeStr = ctime(&now);
        if (timeStr) timeStr[strlen(timeStr)-1] = '\0';
        fprintf(f, "[%s] EMAIL -> %s | OTP=****** | TTL=5m\n", timeStr ? timeStr : "Unknown", email ? email : "UNKNOWN");
        fclose(f);
    }
    logSecurityEvent(email, "OTP_EMAIL_DISPATCHED", "OTP sent via Email channel");
    printf("OTP sent to your email.\n");
    return 1;
}

int createSession(const char *userID, AuthLevel level, Session *session) {
    if (sessionCount >= MAX_SESSIONS) {
        cleanupExpiredSessions();
        if (sessionCount >= MAX_SESSIONS) return 0;
    }
    
    Session *newSession = &activeSessions[sessionCount++];
    strncpy(newSession->userID, userID, 19);
    newSession->userID[19] = '\0';
    
    srand((unsigned int)time(NULL));
    snprintf(newSession->sessionToken, 64, "%s_%lld_%d", userID, (long long)time(NULL), rand());
    
    newSession->loginTime = time(NULL);
    newSession->lastActivity = time(NULL);
    newSession->authLevel = level;
    newSession->isActive = 1;
    
    *session = *newSession;
    logSecurityEvent(userID, "SESSION_CREATED", "New session created");
    return 1;
}

int validateSession(const char *sessionToken, Session *session) {
    for (int i = 0; i < sessionCount; i++) {
        if (activeSessions[i].isActive && 
            strcmp(activeSessions[i].sessionToken, sessionToken) == 0) {
            
            if (time(NULL) - activeSessions[i].lastActivity > SESSION_TIMEOUT) {
                activeSessions[i].isActive = 0;
                logSecurityEvent(activeSessions[i].userID, "SESSION_EXPIRED", "Session expired");
                return 0;
            }
            
            *session = activeSessions[i];
            return 1;
        }
    }
    return 0;
}

int updateSessionActivity(const char *sessionToken) {
    for (int i = 0; i < sessionCount; i++) {
        if (activeSessions[i].isActive && 
            strcmp(activeSessions[i].sessionToken, sessionToken) == 0) {
            activeSessions[i].lastActivity = time(NULL);
            return 1;
        }
    }
    return 0;
}

int destroySession(const char *sessionToken) {
    for (int i = 0; i < sessionCount; i++) {
        if (strcmp(activeSessions[i].sessionToken, sessionToken) == 0) {
            activeSessions[i].isActive = 0;
            logSecurityEvent(activeSessions[i].userID, "SESSION_DESTROYED", "Session terminated");
            return 1;
        }
    }
    return 0;
}

int cleanupExpiredSessions(void) {
    int cleaned = 0;
    time_t now = time(NULL);
    
    for (int i = 0; i < sessionCount; i++) {
        if (activeSessions[i].isActive && 
            (now - activeSessions[i].lastActivity > SESSION_TIMEOUT)) {
            activeSessions[i].isActive = 0;
            cleaned++;
        }
    }
    return cleaned;
}

void encryptData(const char *data, char *encrypted, const char *key) {
    if (!data || !encrypted || !key) return;
    size_t dataLen = strlen(data);
    size_t keyLen = strlen(key);
    if (keyLen == 0) return;
    for (size_t i = 0; i < dataLen && i < 1024; i++) {
        encrypted[i] = data[i] ^ key[i % keyLen];
    }
    encrypted[dataLen < 1024 ? dataLen : 1024] = '\0';
}

void decryptData(const char *encrypted, char *data, const char *key) {
    encryptData(encrypted, data, key);
}

void generateSecureHash(const char *input, char *hash) {
    unsigned long hashValue = 5381;
    int c;
    const char *str = input;
    
    while ((c = *str++)) {
        hashValue = ((hashValue << 5) + hashValue) + c;
    }
    
    snprintf(hash, 64, "%08lx", hashValue);
}

int logSecurityEvent(const char *userID, const char *event, const char *details) {
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif

    FILE *f = fopen("data/security.log", "a");
    if (!f) return 0;
    
    time_t now = time(NULL);
    char *timeStr = ctime(&now);
    if (timeStr) {
        timeStr[strlen(timeStr) - 1] = '\0';
    }
    
    fprintf(f, "[%s] SECURITY | User: %s | Event: %s | Details: %s\n", 
            timeStr ? timeStr : "Unknown", userID ? userID : "UNKNOWN", 
            event ? event : "NO_EVENT", details ? details : "NO_DETAILS");
    fclose(f);
    return 1;
}

int generateSecurityReport(const char *reportPath) {
    FILE *report = fopen(reportPath, "w");
    if (!report) return 0;
    
    fprintf(report, "Campus Security Report\n");
    time_t now = time(NULL);
    char *timeStr = ctime(&now);
    fprintf(report, "Generated: %s\n", timeStr ? timeStr : "Unknown time");
    fprintf(report, "Active Sessions: %d\n", sessionCount);
    
    fclose(report);
    return 1;
}
