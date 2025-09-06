#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/auth.h"
#include "../include/fileio.h"
#include "../include/utils.h"
#include "../include/config.h"
#include "../include/ui.h"
#include "../include/student.h"

CampusType selectCampusType(void) {
    int choice = 0;
    printf("\nSelect Campus Type:\n");
    printf("1. School\n");
    printf("2. College\n");
    printf("3. Hospital\n");
    printf("4. Hostel\n");
    printf("Enter choice: ");
    
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
        printf("Invalid choice. Enter 1-4: ");
        while (getchar() != '\n');
    }
    // Clear the input buffer after successful input
    while (getchar() != '\n');
    return (CampusType)choice;
}

// Simple password hash using XOR and hex encoding (for demo only)
ErrorCode hashPassword(const char *password, char *hashOut) {
    if (!password || !hashOut) return ERROR_INVALID_INPUT;
    unsigned char key = 0x5A;
    int i = 0;
    size_t len = strlen(password);
    if (len > 31) len = 31; // Prevent buffer overflow
    
    for (i = 0; i < (int)len; i++) {
        unsigned char c = password[i] ^ key;
        snprintf(hashOut + i * 2, 3, "%02X", c);
    }
    hashOut[i * 2] = '\0';
    return SUCCESS;
}

// Hide password input (Windows only, fallback to normal input)
#ifdef _WIN32
#include <conio.h>
#endif
ErrorCode getHiddenPassword(char *password) {
    if (!password) return ERROR_INVALID_INPUT;
    
    printf("(input hidden): ");
    int i = 0;
#ifdef _WIN32
    char ch = 0;
    while ((ch = (char)_getch()) != '\r' && i < MAX_LEN - 1) {
        if (ch == '\b' && i > 0) { i--; printf("\b \b"); continue; }
        if (ch == '\b') continue;
        password[i++] = (char)ch;
        printf("*");
    }
    password[i] = '\0';
    printf("\n");
#else
    if (fgets(password, MAX_LEN, stdin) != NULL) {
        password[strcspn(password, "\n")] = '\0';
    } else {
        password[0] = '\0';
        return ERROR_INVALID_INPUT;
    }
#endif
    return SUCCESS;
}

// Basic email validation
// Strong email validation
ErrorCode validateEmail(const char *email) {
    if (!email) return ERROR_INVALID_INPUT;
    
    // Must have one '@', at least one '.', and valid chars
    int atCount = 0, dotCount = 0;
    size_t len = strlen(email);
    if (len < 6 || len > 64) return ERROR_INVALID_INPUT;
    for (int i = 0; i < (int)len; i++) {
        if (email[i] == '@') atCount++;
        if (email[i] == '.') dotCount++;
        if (!(isalnum(email[i]) || email[i] == '@' || email[i] == '.' || email[i] == '_' || email[i] == '-')) return ERROR_INVALID_INPUT;
    }
    if (atCount != 1 || dotCount < 1) return ERROR_INVALID_INPUT;
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    if (!at || !dot || at > dot) return ERROR_INVALID_INPUT;
    if (at == email || dot == email + len - 1) return ERROR_INVALID_INPUT;
    return SUCCESS;
}

// Basic mobile validation (10 digits)
// Strong mobile validation (10 digits, starts with 6-9)
ErrorCode validateMobile(const char *mobile) {
    if (!mobile) return ERROR_INVALID_INPUT;
    if (strlen(mobile) != 10) return ERROR_INVALID_INPUT;
    if (mobile[0] < '6' || mobile[0] > '9') return ERROR_INVALID_INPUT;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(mobile[i])) return ERROR_INVALID_INPUT;
    }
    return SUCCESS;
}

// Strong password validation (min 8 chars, upper, lower, digit, special)
int validatePassword(const char *password) {
    size_t len = strlen(password);
    if (len < 8) return 0;
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    for (int i = 0; i < (int)len; i++) {
        if (isupper(password[i])) hasUpper = 1;
        else if (islower(password[i])) hasLower = 1;
        else if (isdigit(password[i])) hasDigit = 1;
        else if (strchr("!@#$%^&*()-_=+[]{}|;:'\",.<>?/", password[i])) hasSpecial = 1;
    }
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

// StudentID validation (2 letters + 25 + 3 digits)
int validateStudentID(const char *studentID) {
    if (strlen(studentID) != 7) return 0;
    if (!isalpha(studentID[0]) || !isalpha(studentID[1])) return 0;
    if (studentID[2] != '2' || studentID[3] != '5') return 0;
    for (int i = 4; i < 7; i++) {
        if (!isdigit(studentID[i])) return 0;
    }
    return 1;
}

// Edit profile (simple demo: just print message)
ErrorCode editProfile(const char *userID) {
    if (!userID) return ERROR_INVALID_INPUT;
    Profile p = {0};
    if (!readProfile(&p, userID)) {
        printf("Cannot open profile for editing.\n");
        logEvent(userID, "Failed to open profile for editing");
        return ERROR_FILE_IO;
    }

    printf("\nEdit Profile\n");
    printf("Current Name: %s\n", p.name);
    printf("Enter new name (or press Enter to keep): ");
    char buf[MAX_LEN] = {0};
    if (fgets(buf, MAX_LEN, stdin) != NULL && buf[0] != '\n') {
        buf[strcspn(buf, "\n")] = '\0';
        strncpy(p.name, buf, MAX_LEN - 1);
        p.name[MAX_LEN - 1] = '\0';
    }

    printf("Current %s: %s\n", getCampusName(p.campusType), p.instituteName);
    printf("Enter new %s (or press Enter to keep): ", getCampusName(p.campusType));
    if (fgets(buf, MAX_LEN, stdin) != NULL && buf[0] != '\n') {
        buf[strcspn(buf, "\n")] = '\0';
        strncpy(p.instituteName, buf, MAX_LEN - 1);
        p.instituteName[MAX_LEN - 1] = '\0';
    }

    printf("Current Department: %s\n", p.department);
    printf("Enter new department (or press Enter to keep): ");
    if (fgets(buf, 50, stdin) != NULL && buf[0] != '\n') {
        buf[strcspn(buf, "\n")] = '\0';
        strncpy(p.department, buf, 49);
        p.department[49] = '\0';
    }

    printf("Current Email: %s\n", p.email);
    printf("Enter new email (or press Enter to keep): ");
    if (fgets(buf, MAX_LEN, stdin) != NULL && buf[0] != '\n') {
        buf[strcspn(buf, "\n")] = '\0';
        if (validateEmail(buf)) {
            strncpy(p.email, buf, MAX_LEN - 1);
            p.email[MAX_LEN - 1] = '\0';
        } else {
            printf("Invalid email format. Keeping old email.\n");
        }
    }

    printf("Current Mobile: %s\n", p.mobile);
    printf("Enter new mobile (or press Enter to keep): ");
    if (fgets(buf, 15, stdin) != NULL && buf[0] != '\n') {
        buf[strcspn(buf, "\n")] = '\0';
        if (validateMobile(buf)) {
            strncpy(p.mobile, buf, 14);
            p.mobile[14] = '\0';
        } else {
            printf("Invalid mobile format. Keeping old mobile.\n");
        }
    }

    if (!writeProfile(&p, userID)) {
        printf("Failed to save updated profile.\n");
        logEvent(userID, "Profile edit failed during write");
        return ERROR_FILE_IO;
    }
    logEvent(userID, "Profile edited successfully");
    printf("Profile updated successfully!\n");
    return SUCCESS;
}

// View profile (simple demo: just print message)
ErrorCode viewProfile(const char *userID) {
    if (!userID) return ERROR_INVALID_INPUT;
    Profile p = {0};
    if (!readProfile(&p, userID)) {
        printf("Cannot open profile for viewing.\n");
        logEvent(userID, "Failed to open profile for viewing");
        return ERROR_FILE_IO;
    }
    printf("\n%s Profile\n", getCampusName(p.campusType));
    printf("Name: %s\n", p.name);
    printf("%s: %s\n", getCampusName(p.campusType), p.instituteName);
    printf("Department: %s\n", p.department);
    printf("Email: %s\n", p.email);
    printf("Mobile: %s\n", p.mobile);
    printf("User ID: %s\n", p.userID);
    printf("Data Fields (%d):\n", p.dataCount);
    for (int i = 0; i < p.dataCount; i++) {
        printf("  %d. %s\n", i + 1, p.dataFields[i]);
    }
    logEvent(userID, "Viewed profile");
    return SUCCESS;
}

ErrorCode changePassword(const char *userID) {
    if (!userID) return ERROR_INVALID_INPUT;
    Profile p = {0};
    if (!readProfile(&p, userID)) {
        printf("Cannot open profile for password change.\n");
        logEvent(userID, "Failed to open profile for password change");
        return ERROR_FILE_IO;
    }
    char oldPass[MAX_LEN] = {0}, oldHash[MAX_LEN] = {0};
    printf("Enter current password: ");
    getHiddenPassword(oldPass);
    hashPassword(oldPass, oldHash);
    if (strcmp(p.passwordHash, oldHash) != 0) {
        printf("Incorrect current password. Try again.\n");
        logEvent(userID, "Incorrect current password attempt");
        return ERROR_AUTH_FAILED;
    }
    char newPass[MAX_LEN] = {0}, confirmPass[MAX_LEN] = {0}, newHash[MAX_LEN] = {0};
    printf("Enter new password: ");
    getHiddenPassword(newPass);
    if (strlen(newPass) < 6) {
        printf("Password too short. Minimum 6 characters.\n");
        return ERROR_INVALID_INPUT;
    }
    printf("Confirm new password: ");
    getHiddenPassword(confirmPass);
    if (strcmp(newPass, confirmPass) != 0) {
        printf("Passwords do not match.\n");
        return ERROR_INVALID_INPUT;
    }
    hashPassword(newPass, newHash);
    strncpy(p.passwordHash, newHash, 63);
    p.passwordHash[63] = '\0';
    if (!writeProfile(&p, userID)) {
        printf("Failed to write updated profile.\n");
        logEvent(userID, "Password change failed during write");
        return ERROR_FILE_IO;
    }
    logEvent(userID, "Password changed successfully");
    printf("Password changed successfully!\n");
    return SUCCESS;
}

ErrorCode exportProfile(const char *userID) {
    if (!userID) return ERROR_INVALID_INPUT;
    
    Profile p = {0};
    if (!readProfile(&p, userID)) {
        printf("Cannot load profile for export.\n");
        return ERROR_FILE_IO;
    }
    
    printf("\nSelect Export Format:\n");
    printf("1. PDF\n");
    printf("2. Text File\n");
    printf("3. CSV\n");
    printf("Enter choice: ");
    
    int format = 0;
    if (scanf("%d", &format) != 1 || format < 1 || format > 3) {
        printf("Invalid format selection.\n");
        while (getchar() != '\n');
        return ERROR_INVALID_INPUT;
    }
    while (getchar() != '\n');
    
    char filename[200] = {0};
    
    switch(format) {
        case 1: // PDF
            snprintf(filename, sizeof(filename), "data/%s_profile.pdf", userID);
            exportProfilePDF(userID, filename);
            break;
        case 2: // Text
            snprintf(filename, sizeof(filename), "data/%s_profile.txt", userID);
            exportProfileTXT(userID, filename);
            break;
        case 3: // CSV
            snprintf(filename, sizeof(filename), "data/%s_profile.csv", userID);
            exportProfileCSV(userID, filename);
            break;
    }
    
    logEvent(userID, "Profile exported");
    return SUCCESS;
}
