#ifndef AUTH_H
#define AUTH_H

#include "config.h"   // for MAX_LEN, MAX_SUBJECTS
#include "student.h"  // for export functions (if needed)
#include "ui.h"
#include "utils.h"

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
void signup(void);
void signin(void);
int editProfile(const char *userID);
int changePassword(const char *userID);
void viewProfile(const char *userID);
CampusType selectCampusType(void);

// Security utilities
void hashPassword(const char *password, char *hashOut);
void getHiddenPassword(char *password);
int validateEmail(const char *email);
int validateMobile(const char *mobile);

#endif // AUTH_H
