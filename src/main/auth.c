#include <stdio.h>
#include <string.h>
#include "../include/auth.h"
#include "../include/fileio.h"
#include "../include/utils.h"
#include "../include/config.h"
#include"../include/UI.h"
int changePassword(const char *studentID) {
    Profile p;

    if (!readProfile(&p, studentID)) {
        printf("❌ Cannot open profile for password change.\n");
        logEvent(studentID, "🚫 Failed to open profile for password change");
        return 1;
    }

    char oldPass[MAX_LEN], oldHash[MAX_LEN];
    printf("🔐 Enter current password: ");
    getHiddenPassword(oldPass);
    hashPassword(oldPass, oldHash);

    if (strcmp(p.passwordHash, oldHash) != 0) {
        printf("🚫 Incorrect current password. Try again.\n");
        logEvent(studentID, "🚫 Incorrect current password attempt");
        return 1;
    }

    char newPass[MAX_LEN], confirmPass[MAX_LEN], newHash[MAX_LEN];
    printf("🔐 Enter new password: ");
    getHiddenPassword(newPass);

    if (strlen(newPass) < 6) {
        printf("❌ Password too short. Minimum 6 characters.\n");
        return 1;
    }

    printf("🔐 Confirm new password: ");
    getHiddenPassword(confirmPass);

    if (strcmp(newPass, confirmPass) != 0) {
        printf("❌ Passwords do not match.\n");
        return 1;
    }

    hashPassword(newPass, newHash);
    strcpy(p.passwordHash, newHash);

    if (!writeProfile(&p, studentID)) {
        printf("❌ Failed to write updated profile.\n");
        logEvent(studentID, "🚫 Password change failed during write");
        return 1;
    }

    logEvent(studentID, "✅ Password changed successfully");
    printf("✅ Password changed successfully!\n");
    return 0;
}
