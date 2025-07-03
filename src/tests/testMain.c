#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/auth.h"

// Mock implementations for signup and signin
int signup_called = 0;
int signin_called = 0;

void signup() {
    signup_called = 1;
    printf("[MOCK] signup called\n");
}

void signin() {
    signin_called = 1;
    printf("[MOCK] signin called\n");
}

// Helper to simulate user input
void set_stdin(const char *input) {
    FILE *fp = freopen(NULL, "r", stdin);
    if (fp) fclose(fp);
    fp = fmemopen((void *)input, strlen(input), "r");
    stdin = fp;
}

// Test displayMainMenu output
void test_displayMainMenu() {
    printf("Running test_displayMainMenu...\n");
    fflush(stdout);
    // Redirect stdout to buffer
    char buffer[512] = {0};
    FILE *orig_stdout = stdout;
    FILE *mem = fmemopen(buffer, sizeof(buffer), "w");
    stdout = mem;

    extern void displayMainMenu();
    displayMainMenu();

    fflush(mem);
    stdout = orig_stdout;
    fclose(mem);

    if (strstr(buffer, "Ajay Campus Portal") && strstr(buffer, "1. Signup") && strstr(buffer, "2. Signin") && strstr(buffer, "3. Exit")) {
        printf("test_displayMainMenu passed.\n");
    } else {
        printf("test_displayMainMenu failed.\n");
    }
}

// Test main menu choices
void test_main_menu_choices() {
    printf("Running test_main_menu_choices...\n");
    // Test signup
    signup_called = 0;
    set_stdin("1\n3\n");
    main();
    if (signup_called) {
        printf("Signup option test passed.\n");
    } else {
        printf("Signup option test failed.\n");
    }

    // Test signin
    signin_called = 0;
    set_stdin("2\n3\n");
    main();
    if (signin_called) {
        printf("Signin option test passed.\n");
    } else {
        printf("Signin option test failed.\n");
    }
}

// Test invalid input
void test_invalid_input() {
    printf("Running test_invalid_input...\n");
    set_stdin("abc\n3\n");
    main();
    printf("Check output for invalid input message.\n");
}

int main() {
    test_displayMainMenu();
    test_main_menu_choices();
    test_invalid_input();
    return 0;
}