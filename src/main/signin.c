#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../include/auth.h"
#include "../include/student.h"
#include "../include/ui.h"
#include "../include/config.h"
#include "../include/fileio.h"
#include "../include/utils.h"
#include "../include/hpdf/hpdf.h"
#include "../include/database.h"
#include "../include/security.h"

void signin() {
    Profile p = {0};
    char userID[MAX_LEN] = {0}, mobileInput[15] = {0}, inputPassword[MAX_LEN] = {0}, hashedInput[MAX_LEN] = {0};
    char otp[7] = {0}, inputOTP[7] = {0};
    int attempts = 0;
    Session session = {0};

    printf("User ID: ");
    if (scanf(" %99s", userID) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    // Check if account is locked
    if (isAccountLocked(userID)) {
        printf("Account is temporarily locked due to security reasons.\n");
        return;
    }

    char filename[150] = {0};
    getProfilePath(filename, userID);

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Login failed! Profile not found for ID: %s\n", userID);
        return;
    }

    fread(&p, sizeof(Profile), 1, fp);
    fclose(fp);

    while (attempts < 3) {
        printf("Mobile Number: ");
        if (scanf(" %14s", mobileInput) != 1) {
            printf("Invalid input\n");
            return;
        }

        printf("Password ");
        getHiddenPassword(inputPassword);
        hashPassword(inputPassword, hashedInput);

        if (authenticateUser(userID, mobileInput, hashedInput)) {
            // Generate and send OTP for enhanced security
            if (generateOTP(userID, otp)) {
                sendOTPSMS(mobileInput, otp);
                sendOTPEmail(p.email, otp);
                
                printf("OTP sent to your mobile and email. Enter OTP: ");
                if (scanf(" %6s", inputOTP) == 1 && verifyOTP(userID, inputOTP)) {
                    printf("Login successful! Welcome, %s [%s]\n", p.name, p.userID);
                    
                    // Create secure session
                    createSession(userID, AUTH_LEVEL_BASIC, &session);
                    
                    FILE *userFile = fopen("logs/user.txt", "w");
                    if (userFile) {
                        fprintf(userFile, "%s\n", p.userID);
                        fclose(userFile);
                    }
                    
                    dashboard(p.userID);
                    destroySession(session.sessionToken);
                    return;
                } else {
                    printf("Invalid OTP. Login failed.\n");
                    attempts++;
                }
            } else {
                printf("Failed to generate OTP. Please try again.\n");
                return;
            }

        } else {
            printf("Login failed! Invalid credentials. Try again (%d/3)\n", ++attempts);
        }
    }

    printf("Too many failed attempts. Account will be locked.\n");
    lockAccount(userID, 15); // Lock for 15 minutes
}