#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/auth.h"

// Mock implementations for dependencies
int validateEmail(const char *email) {
    return strchr(email, '@') != NULL;
}
int validateMobile(const char *mobile) {
    return strlen(mobile) == 10;
}
void getHiddenPassword(char *buf) {
    strcpy(buf, "testpass");
}
void hashPassword(const char *input, char *output) {
    // Simple mock hash: reverse string
    int len = strlen(input);
    for (int i = 0; i < len; i++)
        output[i] = input[len - i - 1];
    output[len] = '\0';
}

// Helper to create a dummy profile file
void createDummyProfile(const char *studentID, const char *password, const char *email, const char *mobile) {
    Profile p = {0};
    strcpy(p.studentID, studentID);
    strcpy(p.name, "Test User");
    strcpy(p.schoolName, "Test School");
    strcpy(p.stream, "Science");
    strcpy(p.email, email);
    strcpy(p.mobile, mobile);
    p.subjectCount = 2;
    strcpy(p.subjects[0], "Math");
    strcpy(p.subjects[1], "Physics");
    hashPassword(password, p.passwordHash);

    char path[150];
    getProfilePath(path, studentID);
    FILE *fp = fopen(path, "wb");
    assert(fp);
    fwrite(&p, sizeof(Profile), 1, fp);
    fclose(fp);
}

void test_buildProfileExt() {
    char ext[6];
    buildProfileExt(ext, "AB1234");
    assert(strcmp(ext, "abpfx") == 0);
}

void test_getProfilePath() {
    char path[150];
    getProfilePath(path, "STU001");
    assert(strcmp(path, "credentials/STU001.stpfx") == 0);
}

void test_changePassword() {
    createDummyProfile("TST001", "testpass", "a@b.com", "1234567890");
    // getHiddenPassword always returns "testpass" (old), then "testpass" (new), so password will be changed to same
    int res = changePassword("TST001");
    assert(res == 0);
}

void test_editProfile() {
    createDummyProfile("TST002", "testpass", "old@b.com", "1234567890");
    // Simulate user input for scanf
    freopen("test_input.txt", "w", stdout);
    printf("new@b.com\n1234567891\n");
    freopen("test_input.txt", "r", stdin);

    int res = editProfile("TST002");
    assert(res == 0);

    // Check if profile updated
    char path[150];
    getProfilePath(path, "TST002");
    FILE *fp = fopen(path, "rb");
    assert(fp);
    Profile p;
    fread(&p, sizeof(Profile), 1, fp);
    fclose(fp);
    assert(strcmp(p.email, "new@b.com") == 0);
    assert(strcmp(p.mobile, "1234567891") == 0);
}

void test_viewProfile() {
    createDummyProfile("TST003", "testpass", "v@b.com", "1234567890");
    // Just call to check for crash
    viewProfile("TST003");
}

int main() {
    test_buildProfileExt();
    test_getProfilePath();
    test_changePassword();
    test_editProfile();
    test_viewProfile();
    printf("All tests passed!\n");
    return 0;
}