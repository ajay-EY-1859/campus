#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/auth.h"

// Mock functions for dependencies
int validateEmail(const char *email) {
    return strstr(email, "@") && strstr(email, "."); // simple mock
}
int validateMobile(const char *mobile) {
    return strlen(mobile) == 10;
}
void getHiddenPassword(char *password) {
    strcpy(password, "TestPass123");
}
void hashPassword(const char *password, char *hash) {
    strcpy(hash, "hashedpassword"); // mock hash
}
void getProfilePath(char *filename, const char *studentID) {
    sprintf(filename, "test_profiles/%s.dat", studentID);
}
void logEvent(const char *studentID, const char *event) {
    // mock: do nothing
}

// Test generateStudentID
void test_generateStudentID() {
    Profile p;
    strcpy(p.schoolName, "ABSchool");
    generateStudentID(&p);
    assert(strlen(p.studentID) >= 7);
    assert(p.studentID[0] == 'a' && p.studentID[1] == 'b');
    printf("test_generateStudentID passed.\n");
}

// Test signup (integration, simulating user input)
void test_signup() {
    // Redirect stdin from a file with test inputs
    FILE *input = fopen("test_signup_input.txt", "w");
    fprintf(input, "Test User\nTestSchool\nScience\n2\nMath\nPhysics\ntest@example.com\n1234567890\n");
    fclose(input);
    input = fopen("test_signup_input.txt", "r");
    FILE *orig_stdin = stdin;
    stdin = input;

    signup();

    // Check if profile file was created
    char filename[150];
    Profile p;
    strcpy(p.schoolName, "TestSchool");
    generateStudentID(&p);
    getProfilePath(filename, p.studentID);

    FILE *fp = fopen(filename, "rb");
    assert(fp != NULL);
    fclose(fp);

    // Clean up
    remove(filename);
    remove("test_signup_input.txt");
    stdin = orig_stdin;
    printf("test_signup passed.\n");
}

int main() {
    test_generateStudentID();
    test_signup();
    printf("All tests passed.\n");
    return 0;
}