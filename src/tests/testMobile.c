#include <stdio.h>
#include "../include/auth.h"

void test_valid_mobile_numbers() {
    const char *valid[] = {
        "9876543210",
        "8123456789",
        "7000000000",
        "9999999999"
    };

    int passed = 1;
    for (int i = 0; i < sizeof(valid)/sizeof(valid[0]); i++) {
        if (!validateMobile(valid[i])) {
            printf("❌ FAIL: Valid mobile rejected → %s\n", valid[i]);
            passed = 0;
        }
    }
    if (passed) printf("✅ validateMobile() accepted all valid numbers\n");
}

void test_invalid_mobile_numbers() {
    const char *invalid[] = {
        "123456789",     // too short
        "abcdefghij",    // letters
        "99999",         // too short
        "70000000000",   // too long
        "98765 43210",   // space inside
        "+919876543210", // has +91
        "0000000000",    // invalid pattern
        " 9876543210",   // leading space
        "98765@3210"     // special character
    };

    int passed = 1;
    for (int i = 0; i < sizeof(invalid)/sizeof(invalid[0]); i++) {
        if (validateMobile(invalid[i])) {
            printf("❌ FAIL: Invalid mobile accepted → %s\n", invalid[i]);
            passed = 0;
        }
    }
    if (passed) printf("✅ validateMobile() rejected all invalid numbers\n");
}

int main() {
    printf("==== 🧪 AjCampus Mobile Validation Tests ====\n");
    test_valid_mobile_numbers();
    test_invalid_mobile_numbers();
    return 0;
}
