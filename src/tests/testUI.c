#include <stdio.h>
#include <string.h>
#include "../include/auth.h"
#include "../include/student.h"

// Mock implementations for testing
void viewProfile(const char *studentID) {
    printf("[MOCK] viewProfile called with %s\n", studentID);
}
void editProfile(const char *studentID) {
    printf("[MOCK] editProfile called with %s\n", studentID);
}
void saveSubjectData(const char *studentID) {
    printf("[MOCK] saveSubjectData called with %s\n", studentID);
}
void loadSubjectData(const char *studentID) {
    printf("[MOCK] loadSubjectData called with %s\n", studentID);
}
void changePassword(const char *studentID) {
    printf("[MOCK] changePassword called with %s\n", studentID);
}
void exportResultPDF(const char *studentID) {
    printf("[MOCK] exportResultPDF called with %s\n", studentID);
}

// Forward declaration of dashboard
void dashboard(const char *studentID);

// Helper to simulate user input
void set_stdin(const char *input) {
    static FILE *input_stream = NULL;
    if (input_stream) fclose(input_stream);
    input_stream = fmemopen((void *)input, strlen(input), "r");
    stdin = input_stream;
}

void test_dashboard_view_profile() {
    set_stdin("1\n7\ny\n");
    printf("Test: View Profile and Logout\n");
    dashboard("S123");
}

void test_dashboard_edit_profile() {
    set_stdin("2\n7\ny\n");
    printf("Test: Edit Profile and Logout\n");
    dashboard("S456");
}

void test_dashboard_add_subject_marks() {
    set_stdin("3\n7\ny\n");
    printf("Test: Add Subject Marks and Logout\n");
    dashboard("S789");
}

void test_dashboard_view_subject_performance() {
    set_stdin("4\n7\ny\n");
    printf("Test: View Subject Performance and Logout\n");
    dashboard("S000");
}

void test_dashboard_change_password() {
    set_stdin("5\n7\ny\n");
    printf("Test: Change Password and Logout\n");
    dashboard("S111");
}

void test_dashboard_export_result_pdf() {
    set_stdin("6\n7\ny\n");
    printf("Test: Export Result PDF and Logout\n");
    dashboard("S222");
}

void test_dashboard_logout_cancel() {
    set_stdin("7\nn\n7\ny\n");
    printf("Test: Logout Cancel, then Logout Confirm\n");
    dashboard("S333");
}

void test_dashboard_invalid_choice() {
    set_stdin("99\n7\ny\n");
    printf("Test: Invalid Choice, then Logout\n");
    dashboard("S444");
}

int main() {
    test_dashboard_view_profile();
    test_dashboard_edit_profile();
    test_dashboard_add_subject_marks();
    test_dashboard_view_subject_performance();
    test_dashboard_change_password();
    test_dashboard_export_result_pdf();
    test_dashboard_logout_cancel();
    test_dashboard_invalid_choice();
    printf("All dashboard tests completed.\n");
    return 0;
}