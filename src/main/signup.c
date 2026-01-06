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
#include "../include/campus_security.h"



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
            // Validation: ensure all fields are non-empty and unique
            for (int i = 0; i < p.dataCount; i++) {
                if (strlen(p.dataFields[i]) == 0) {
                    printf("Error: Subject name cannot be empty.\n");
                    return ERROR_INVALID_INPUT;
                }
                for (int j = i + 1; j < p.dataCount; j++) {
                    if (strcmp(p.dataFields[i], p.dataFields[j]) == 0) {
                        printf("Error: Duplicate subject name '%s'.\n", p.dataFields[i]);
                        return ERROR_INVALID_INPUT;
                    }
                }
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
            // Validation: ensure all fields are non-empty and unique
            for (int i = 0; i < p.dataCount; i++) {
                if (strlen(p.dataFields[i]) == 0) {
                    printf("Error: Course name cannot be empty.\n");
                    return ERROR_INVALID_INPUT;
                }
                for (int j = i + 1; j < p.dataCount; j++) {
                    if (strcmp(p.dataFields[i], p.dataFields[j]) == 0) {
                        printf("Error: Duplicate course name '%s'.\n", p.dataFields[i]);
                        return ERROR_INVALID_INPUT;
                    }
                }
            }
            break;
        case CAMPUS_HOSPITAL:
            p.dataCount = 4;
            strncpy(p.dataFields[0], "Blood Pressure", sizeof(p.dataFields[0]) - 1);
            strncpy(p.dataFields[1], "Temperature", sizeof(p.dataFields[1]) - 1);
            strncpy(p.dataFields[2], "Weight", sizeof(p.dataFields[2]) - 1);
            strncpy(p.dataFields[3], "Diagnosis", sizeof(p.dataFields[3]) - 1);
            p.dataFields[0][sizeof(p.dataFields[0]) - 1] = '\0';
            p.dataFields[1][sizeof(p.dataFields[1]) - 1] = '\0';
            p.dataFields[2][sizeof(p.dataFields[2]) - 1] = '\0';
            p.dataFields[3][sizeof(p.dataFields[3]) - 1] = '\0';
            break;
        case CAMPUS_HOSTEL:
            p.dataCount = 3;
            strncpy(p.dataFields[0], "Room Number", sizeof(p.dataFields[0]) - 1);
            strncpy(p.dataFields[1], "Floor", sizeof(p.dataFields[1]) - 1);
            strncpy(p.dataFields[2], "Mess Plan", sizeof(p.dataFields[2]) - 1);
            p.dataFields[0][sizeof(p.dataFields[0]) - 1] = '\0';
            p.dataFields[1][sizeof(p.dataFields[1]) - 1] = '\0';
            p.dataFields[2][sizeof(p.dataFields[2]) - 1] = '\0';
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