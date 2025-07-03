#include <stdio.h>
#include <string.h>
#include "../include/auth.h"

void test_valid_emails() {
    const char *validEmails[] = {
        "test@example.com",
        "hello.world@domain.co.in",
        "student_123@school.edu",
        "my-email@sub.domain.org",
        "ajay99@gmail.com"
    };

    int passed = 1;
    for (int i = 0; i < sizeof(validEmails)/sizeof(validEmails[0]); i++) {
        if (!validateEmail(validEmails[i])) {
            printf("❌ FAIL: Valid email rejected → %s\n", validEmails[i]);
            passed = 0;
        }
    }
    if (passed) printf("✅ validateEmail() accepted all valid emails\n");
}

void test_invalid_emails() {
    const char *invalidEmails[] = {
        "no-at-symbol.com",
        "missing@domain",
        "@nouser.com",
        "user@.com",
        "user@domain..com",
        "user@domain,com",
        "a@b",
        "user@domain#mail.com",
        "user domain.com"
    };

    int passed = 1;
    for (int i = 0; i < sizeof(invalidEmails)/sizeof(invalidEmails[0]); i++) {
        if (validateEmail(invalidEmails[i])) {
            printf("❌ FAIL: Invalid email accepted → %s\n", invalidEmails[i]);
            passed = 0;
        }
    }
    if (passed) printf("✅ validateEmail() rejected all invalid emails\n");
}

int main() {
    printf("==== 🧪 AjCampus Email Validation Tests ====\n");
    test_valid_emails();
    test_invalid_emails();
    return 0;
}
