#ifndef AUTH_H
#define AUTH_H
#include"student.h"
#include"ui.h"
#include"utils.h"
#include"fileio.h"

#include "config.h" // for MAX_LEN, MAX_SUBJECTS

typedef struct {
    char name[MAX_LEN];
    char schoolName[MAX_LEN];
    char stream[50];
    int subjectCount;
    char subjects[MAX_SUBJECTS][MAX_LEN];
    char email[MAX_LEN];
    char mobile[15];
    char passwordHash[64];
    char studentID[20];
} Profile;

// Auth and Profile actions
void signup();
void signin();
int editProfile(const char *studentID);
int changePassword(const char *studentID);
void viewProfile(const char *studentID);

// Security utilities
void hashPassword(const char *password, char *hashOut);
void getHiddenPassword(char *password);
int validateEmail(const char *email);
int validateMobile(const char *mobile);

#endif
