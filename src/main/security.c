#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <ctype.h>
#include "../include/security.h"
#include "../include/database.h"

#define MAX_SESSIONS 100
#define SESSION_TIMEOUT 1800  // 30 minutes
#define MAX_LOGIN_ATTEMPTS 3
#define ACCOUNT_LOCK_DURATION 900  // 15 minutes

static Session activeSessions[MAX_SESSIONS];
static int sessionCount = 0;

// OTP Generation and Verification
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
    int otpNum = (int)(randomValue % 900000) + 100000; // 6-digit OTP
    snprintf(otp, 7, "%06d", otpNum);
    
    // Store OTP with timestamp
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_otp.dat", userID);
    
    // Ensure data directory exists
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
#endif

    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    
    time_t expiry = time(NULL) + 300; // 5 minutes expiry
    fwrite(otp, 6, 1, f);
    fwrite(&expiry, sizeof(time_t), 1, f);
    fclose(f);
    
    logSecurityEvent(userID, "OTP_GENERATED", "OTP generated for authentication");
    return 1;
}

int verifyOTP(const char *userID, const char *otp) {
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_otp.dat", userID);
    
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

int sendOTPSMS(const char *mobile, const char *otp) {
    // Do not print OTP to console; simulate delivery via outbox
#ifdef _WIN32
    _mkdir("data");
    _mkdir("data\\outbox");
#else
    mkdir("data", 0777);
    mkdir("data/outbox", 0777);
#endif

    FILE *f = fopen("data/outbox/sms.out", "a");
    if (f) {
        time_t now = time(NULL);
        char *timeStr = ctime(&now);
        if (timeStr) timeStr[strlen(timeStr)-1] = '\0';
        fprintf(f, "[%s] SMS -> %s | OTP=%s | TTL=5m\n", timeStr ? timeStr : "Unknown", mobile ? mobile : "UNKNOWN", otp ? otp : "");
        fclose(f);
    }
    logSecurityEvent(mobile, "OTP_SMS_DISPATCHED", "OTP sent via SMS channel");
    printf("OTP sent to your mobile.\n");
    return 1;
}

int sendOTPEmail(const char *email, const char *otp) {
    // Do not print OTP to console; simulate delivery via outbox
#ifdef _WIN32
    _mkdir("data");
    _mkdir("data\\outbox");
#else
    mkdir("data", 0777);
    mkdir("data/outbox", 0777);
#endif

    FILE *f = fopen("data/outbox/email.out", "a");
    if (f) {
        time_t now = time(NULL);
        char *timeStr = ctime(&now);
        if (timeStr) timeStr[strlen(timeStr)-1] = '\0';
        fprintf(f, "[%s] EMAIL -> %s | OTP=%s | TTL=5m\n", timeStr ? timeStr : "Unknown", email ? email : "UNKNOWN", otp ? otp : "");
        fclose(f);
    }
    logSecurityEvent(email, "OTP_EMAIL_DISPATCHED", "OTP sent via Email channel");
    printf("OTP sent to your email.\n");
    return 1;
}

// Session Management
int createSession(const char *userID, AuthLevel level, Session *session) {
    if (sessionCount >= MAX_SESSIONS) {
        cleanupExpiredSessions();
        if (sessionCount >= MAX_SESSIONS) return 0;
    }
    
    Session *newSession = &activeSessions[sessionCount++];
    strncpy(newSession->userID, userID, 19);
    newSession->userID[19] = '\0';
    
    // Generate session token
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

// Security Features
int checkPasswordStrength(const char *password) {
    size_t len = strlen(password);
    if (len < 8) return 0; // Too short
    
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (isupper(password[i])) hasUpper = 1;
        else if (islower(password[i])) hasLower = 1;
        else if (isdigit(password[i])) hasDigit = 1;
        else hasSpecial = 1;
    }
    
    int strength = hasUpper + hasLower + hasDigit + hasSpecial;
    return strength; // 1=Weak, 2=Fair, 3=Good, 4=Strong
}

int isAccountLocked(const char *userID) {
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_lock.dat", userID);
    
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    
    time_t lockTime, duration;
    fread(&lockTime, sizeof(time_t), 1, f);
    fread(&duration, sizeof(time_t), 1, f);
    fclose(f);
    
    if (time(NULL) - lockTime < duration) {
        return 1; // Still locked
    }
    
    // Lock expired, remove lock file
    remove(filename);
    return 0;
}

int lockAccount(const char *userID, int durationMinutes) {
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_lock.dat", userID);
    
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    
    time_t lockTime = time(NULL);
    time_t duration = durationMinutes * 60;
    
    fwrite(&lockTime, sizeof(time_t), 1, f);
    fwrite(&duration, sizeof(time_t), 1, f);
    fclose(f);
    
    logSecurityEvent(userID, "ACCOUNT_LOCKED", "Account locked due to security policy");
    return 1;
}

int unlockAccount(const char *userID) {
    char filename[200];
    snprintf(filename, sizeof(filename), "data/%s_lock.dat", userID);
    
    if (remove(filename) == 0) {
        logSecurityEvent(userID, "ACCOUNT_UNLOCKED", "Account manually unlocked");
        return 1;
    }
    return 0;
}

// Simple encryption (for demo - use proper encryption in production)
void encryptData(const char *data, char *encrypted, const char *key) {
    size_t keyLen = strlen(key);
    size_t dataLen = strlen(data);
    
    for (size_t i = 0; i < dataLen; i++) {
        encrypted[i] = data[i] ^ key[i % keyLen];
    }
    encrypted[dataLen] = '\0';
}

void decryptData(const char *encrypted, char *data, const char *key) {
    // XOR encryption is symmetric
    encryptData(encrypted, data, key);
}

void generateSecureHash(const char *input, char *hash) {
    // Simple hash function (use SHA-256 in production)
    unsigned long hashValue = 5381;
    int c;
    const char *str = input;
    
    while ((c = *str++)) {
        hashValue = ((hashValue << 5) + hashValue) + c;
    }
    
    snprintf(hash, 64, "%08lx", hashValue);
}

// Security Monitoring
int logSecurityEvent(const char *userID, const char *event, const char *details) {
    // Ensure data directory exists
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
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

int detectSuspiciousActivity(const char *userID) {
    int attempts = getLoginAttempts(userID);
    if (attempts >= MAX_LOGIN_ATTEMPTS) {
        logSecurityEvent(userID, "SUSPICIOUS_ACTIVITY", "Multiple failed login attempts detected");
        return 1;
    }
    return 0;
}

int generateSecurityReport(const char *reportPath) {
    FILE *report = fopen(reportPath, "w");
    if (!report) return 0;
    
    fprintf(report, "Campus Security Report\n");
    time_t now = time(NULL);
    char *timeStr = ctime(&now);
    fprintf(report, "Generated: %s\n", timeStr ? timeStr : "Unknown time");
    fprintf(report, "Active Sessions: %d\n", sessionCount);
    
    // Add more security metrics here
    
    fclose(report);
    return 1;
}