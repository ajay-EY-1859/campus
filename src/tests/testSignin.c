#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/auth.h"
#include "../include/student.h"
#include "../include/ui.h"

// Mock functions and variables
int dashboard_called = 0;
char dashboard_studentID[MAX_LEN];

void dashboard(const char *studentID) {
    dashboard_called = 1;
    strncpy(dashboard_studentID, studentID, MAX_LEN);
}

void logEvent(const char *studentID, const char *event) {
    // Mock: do nothing
}

void getHiddenPassword(char *inputPassword) {
    // For testing, just copy a preset password
    strcpy(inputPassword, "testpass");
}

void hashPassword(const char *input, char *output) {
    // For testing, just copy input to output
    strcpy(output, input);
}

void getProfilePath(char *filename, const char *studentID) {
    sprintf(filename, "test_%s.profile", studentID);
}

// Helper to create a mock profile file
void create_mock_profile(const char *studentID, const char *mobile, const char *password) {
    Profile p;
    memset(&p, 0, sizeof(Profile));
    strncpy(p.studentID, studentID, MAX_LEN);
    strncpy(p.mobile, mobile, sizeof(p.mobile));
    strncpy(p.name, "Test User", sizeof(p.name));
    hashPassword(password, p.passwordHash);

    char filename[150];
    getProfilePath(filename, studentID);

    FILE *fp = fopen(filename, "wb");
    fwrite(&p, sizeof(Profile), 1, fp);
    fclose(fp);
}

// Helper to remove mock profile file
void remove_mock_profile(const char *studentID) {
    char filename[150];
    getProfilePath(filename, studentID);
    remove(filename);
}

// Mock scanf for test input
int scanf_call = 0;
char *mock_inputs[] = {
    "S12345",      // Student ID
    "9876543210",  // Mobile
    // Password is handled by getHiddenPassword
};

int mock_scanf(const char *format, char *buffer) {
    if (scanf_call < (sizeof(mock_inputs)/sizeof(mock_inputs[0]))) {
        strcpy(buffer, mock_inputs[scanf_call++]);
        return 1;
    }
    return 0;
}
#define scanf mock_scanf

// Include the code under test
#include "signin.c"
#undef scanf

void test_signin_success() {
    // Arrange
    create_mock_profile("S12345", "9876543210", "testpass");
    dashboard_called = 0;
    scanf_call = 0;

    // Act
    signin();

    // Assert
    assert(dashboard_called == 1);
    assert(strcmp(dashboard_studentID, "S12345") == 0);

    remove_mock_profile("S12345");
}

void test_signin_profile_not_found() {
    // Arrange
    dashboard_called = 0;
    scanf_call = 0;
    mock_inputs[0] = "NOUSER";

    // Act
    signin();

    // Assert
    assert(dashboard_called == 0);
}

void test_signin_invalid_credentials() {
    // Arrange
    create_mock_profile("S12345", "9876543210", "testpass");
    dashboard_called = 0;
    scanf_call = 0;
    mock_inputs[0] = "S12345";
    mock_inputs[1] = "wrongmobile";

    // Act
    signin();

    // Assert
    assert(dashboard_called == 0);

    remove_mock_profile("S12345");
}

int main() {
    test_signin_success();
    test_signin_profile_not_found();
    test_signin_invalid_credentials();
    printf("All signin tests passed!\n");
    return 0;
}