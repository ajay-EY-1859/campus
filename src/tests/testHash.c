#include <stdio.h>
#include <string.h>
#include "../include/auth.h"

void test_hashPassword() {
    char hash1[64], hash2[64];

    const char *input = "secret123";

    hashPassword(input, hash1);
    hashPassword(input, hash2);

    if (strcmp(hash1, hash2) == 0) {
        printf("✅ hashPassword(): PASS (consistent hash)\n");
    } else {
        printf("❌ hashPassword(): FAIL (hash mismatch)\n");
    }
}

int main() {
    printf("==== AjCampus Test Suite ====\n");
    test_hashPassword();
    return 0;
}
