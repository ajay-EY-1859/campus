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

ErrorCode signup() {
    Profile p = {0};
    char password[MAX_LEN] = {0};

    do {
        printf("Enter your name: ");
        if (safeGetString(p.name, sizeof(p.name)) == SUCCESS && strlen(p.name) > 0) {
            break;
        }
        printf("Invalid input. Please try again.\n");
    } while (1);

    p.campusType = selectCampusType();

    do {
        printf("Enter %s name: ", getCampusName(p.campusType));
        if (safeGetString(p.instituteName, sizeof(p.instituteName)) == SUCCESS && strlen(p.instituteName) > 0) {
            break;
        }
        printf("Invalid input. Please try again.\n");
    } while (1);

    // Campus-specific department/stream prompt
    switch(p.campusType) {
        case CAMPUS_SCHOOL:
            printf("Stream (Science/Commerce/Arts): ");
            break;
        case CAMPUS_COLLEGE:
            printf("Department (CSE/ECE/Mechanical/etc): ");
            break;
        case CAMPUS_HOSPITAL:
            printf("Department (Cardiology/Neurology/etc): ");
            break;
        case CAMPUS_HOSTEL:
            printf("Block/Wing: ");
            break;
    }
    
    do {
        if (safeGetString(p.department, sizeof(p.department)) == SUCCESS && strlen(p.department) > 0) {
            break;
        }
        printf("Invalid input. Please try again: ");
    } while (1);

    // Campus-specific data fields
    switch(p.campusType) {
        case CAMPUS_SCHOOL:
            do {
                printf("Number of subjects: ");
                if (safeGetInt(&p.dataCount, 1, MAX_SUBJECTS) == SUCCESS) {
                    break;
                }
                printf("Invalid number. Please enter 1-%d: ", MAX_SUBJECTS);
            } while (1);
            for (int i = 0; i < p.dataCount; i++) {
                do {
                    printf("Subject %d: ", i + 1);
                    if (safeGetString(p.dataFields[i], sizeof(p.dataFields[i])) == SUCCESS && strlen(p.dataFields[i]) > 0) {
                        break;
                    }
                    printf("Invalid subject name. Please try again: ");
                } while (1);
            }
            break;
        case CAMPUS_COLLEGE:
            do {
                printf("Number of courses: ");
                if (safeGetInt(&p.dataCount, 1, MAX_SUBJECTS) == SUCCESS) {
                    break;
                }
                printf("Invalid number. Please enter 1-%d: ", MAX_SUBJECTS);
            } while (1);
            for (int i = 0; i < p.dataCount; i++) {
                do {
                    printf("Course %d: ", i + 1);
                    if (safeGetString(p.dataFields[i], sizeof(p.dataFields[i])) == SUCCESS && strlen(p.dataFields[i]) > 0) {
                        break;
                    }
                    printf("Invalid course name. Please try again: ");
                } while (1);
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
        if (safeGetString(p.email, sizeof(p.email)) != SUCCESS) {
            printf("Invalid input\n");
            continue;
        }
        if (isValidEmail(p.email) != SUCCESS) {
            printf("Invalid email format. Please try again.\n");
            continue;
        }
        if (isEmailAlreadyRegistered(p.email)) {
            printf("This email is already registered. Please use a different email.\n");
            continue;
        }
        break;
    } while (1);

    do {
        printf("Mobile: ");
        if (safeGetString(p.mobile, sizeof(p.mobile)) != SUCCESS) {
            printf("Invalid input\n");
            continue;
        }
        if (isValidMobile(p.mobile) != SUCCESS) {
            printf("Invalid mobile format. Please try again.\n");
            continue;
        }
        if (isMobileAlreadyRegistered(p.mobile)) {
            printf("This mobile number is already registered. Please use a different number.\n");
            continue;
        }
        break;
    } while (1);

    do {
        printf("Password ");
        getHiddenPassword(password);
        
        // Check password strength
        int strength = checkPasswordStrength(password);
        if (strength >= 3) {
            break;
        }
        printf("Password is too weak. Please try again.\n");
        printf("Requirements: 8+ chars, uppercase, lowercase, digit, special char\n");
    } while (1);
    
    hashPassword(password, p.passwordHash);
    generateUserID(&p);

    if (createUser(&p) != SUCCESS) {
        printf("Registration failed\n");
        return ERROR_DATABASE;
    }

    // Also save profile for dashboard access
    if (!writeProfile(&p, p.userID)) {
        printf("Warning: Profile file creation failed\n");
    }

    logActivity(p.userID, "USER_REGISTERED", "New user registration completed");

    FILE *userFile = fopen(USER_STATE_FILE, "w");
    if (userFile) {
        fprintf(userFile, "%s\n", p.userID);
        fclose(userFile);
    }

    printf("Registration successful! Your ID: %s\n", p.userID);
    printf("Welcome to your dashboard!\n");
    dashboard(p.userID);
    return SUCCESS;
}