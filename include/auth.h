#ifndef AUTH_H
#define AUTH_H

#include "config.h"   // for MAX_LEN, MAX_SUBJECTS

typedef struct {
    char name[MAX_LEN];
    char instituteName[MAX_LEN];
    char department[50];
    CampusType campusType;
    int dataCount;
    char dataFields[MAX_SUBJECTS][MAX_LEN];
    char email[MAX_LEN];
    char mobile[15];
    char passwordHash[64];
    char userID[20];
} Profile;

// Auth and Profile actions
ErrorCode signup(void);
ErrorCode signin(void);
ErrorCode editProfile(const char *userID);
ErrorCode changePassword(const char *userID);
ErrorCode viewProfile(const char *userID);
CampusType selectCampusType(void);

// Security utilities
ErrorCode hashPassword(const char *password, char *hashOut);
ErrorCode getHiddenPassword(char *password);
ErrorCode validateEmail(const char *email);
ErrorCode validateMobile(const char *mobile);

#endif // AUTH_H
