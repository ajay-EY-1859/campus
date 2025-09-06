#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../include/auth.h"
#include "../include/utils.h"
#include "../include/fileio.h"
#include "../include/config.h"
#include "../include/student.h"
#include "../include/ui.h"
#include "../include/database.h"
#include "../include/security.h"

void generateUserID(Profile *p) {
    char initials[3] = {0};
    if (strlen(p->instituteName) >= 2) {
        initials[0] = (char)tolower(p->instituteName[0]);
        initials[1] = (char)tolower(p->instituteName[1]);
    } else {
        initials[0] = 'x';
        initials[1] = 'x';
    }
    initials[2] = '\0';
    srand((unsigned int)time(NULL));
    int serial = rand() % 900 + 100;
    snprintf(p->userID, sizeof(p->userID), "%s25%d", initials, serial);
}

void signup() {
    Profile p = {0};
    char password[MAX_LEN] = {0};

    printf("Enter your name: ");
    if (!safeGetString(p.name, sizeof(p.name))) {
        printf("Invalid input\n");
        return;
    }

    p.campusType = selectCampusType();

    printf("Enter %s name: ", getCampusName(p.campusType));
    if (!safeGetString(p.instituteName, sizeof(p.instituteName))) {
        printf("Invalid input\n");
        return;
    }

    printf("Department/Stream: ");
    if (!safeGetString(p.department, sizeof(p.department))) {
        printf("Invalid input\n");
        return;
    }

    // Campus-specific data fields
    switch(p.campusType) {
        case CAMPUS_SCHOOL:
            printf("Number of subjects: ");
            if (!safeGetInt(&p.dataCount, 1, MAX_SUBJECTS)) {
                printf("Invalid number of subjects\n");
                return;
            }
            for (int i = 0; i < p.dataCount; i++) {
                printf("Subject %d: ", i + 1);
                if (!safeGetString(p.dataFields[i], sizeof(p.dataFields[i]))) {
                    printf("Invalid subject name\n");
                    return;
                }
            }
            break;
        case CAMPUS_COLLEGE:
            printf("Number of courses: ");
            if (!safeGetInt(&p.dataCount, 1, MAX_SUBJECTS)) {
                printf("Invalid number of courses\n");
                return;
            }
            for (int i = 0; i < p.dataCount; i++) {
                printf("Course %d: ", i + 1);
                if (!safeGetString(p.dataFields[i], sizeof(p.dataFields[i]))) {
                    printf("Invalid course name\n");
                    return;
                }
            }
            break;
        case CAMPUS_HOSPITAL:
            p.dataCount = 4;
            strcpy(p.dataFields[0], "Blood Pressure");
            strcpy(p.dataFields[1], "Temperature");
            strcpy(p.dataFields[2], "Weight");
            strcpy(p.dataFields[3], "Diagnosis");
            break;
        case CAMPUS_HOSTEL:
            p.dataCount = 3;
            strcpy(p.dataFields[0], "Room Number");
            strcpy(p.dataFields[1], "Floor");
            strcpy(p.dataFields[2], "Mess Plan");
            break;
    }

    do {
        printf("Email: ");
        if (!safeGetString(p.email, sizeof(p.email))) {
            printf("Invalid input\n");
            continue;
        }
    } while (!isValidEmail(p.email));

    do {
        printf("Mobile: ");
        if (!safeGetString(p.mobile, sizeof(p.mobile))) {
            printf("Invalid input\n");
            continue;
        }
    } while (!isValidMobile(p.mobile));

    printf("Password ");
    getHiddenPassword(password);
    
    // Check password strength
    int strength = checkPasswordStrength(password);
    if (strength < 3) {
        printf("Password is too weak. Please use a stronger password.\n");
        printf("Requirements: 8+ chars, uppercase, lowercase, digit, special char\n");
        return;
    }
    
    hashPassword(password, p.passwordHash);
    generateUserID(&p);

    if (!createUser(&p)) {
        printf("Registration failed\n");
        return;
    }

    logActivity(p.userID, "USER_REGISTERED", "New user registration completed");

    FILE *userFile = fopen(USER_STATE_FILE, "w");
    if (userFile) {
        fprintf(userFile, "%s\n", p.userID);
        fclose(userFile);
    }

    printf("Registration successful! Your ID: %s\n", p.userID);
    printf("View profile? (y/n): ");
    char ch;
    while ((ch = (char)getchar()) != '\n' && ch != EOF);
    ch = (char)getchar();
    if (ch == 'y' || ch == 'Y') {
        viewProfile(p.userID);
    }
}