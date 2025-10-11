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

ErrorCode signin() {
    Profile p = {0};
    char userID[MAX_LEN] = {0}, mobileInput[15] = {0}, inputPassword[MAX_LEN] = {0}, hashedInput[MAX_LEN] = {0};
    char otp[7] = {0}, inputOTP[7] = {0};
    int attempts = 0;
    Session session = {0};

    printf("User ID: ");
    if (safeGetString(userID, sizeof(userID)) != SUCCESS || strlen(userID) == 0) {
        printf("Invalid input\n");
        return ERROR_INVALID_INPUT;
    }
    
    // Check if account is locked
    if (isAccountLocked(userID)) {
        printf("Account is temporarily locked due to security reasons.\n");
        return ERROR_PERMISSION;
    }

    if (!getUserByID(userID, &p)) {
        printf("Login failed! Profile not found for ID: %s\n", userID);
        return ERROR_NOT_FOUND;
    }

    while (attempts < 3) {
        printf("Mobile Number: ");
        if (safeGetString(mobileInput, sizeof(mobileInput)) != SUCCESS || strlen(mobileInput) == 0) {
            printf("Invalid input\n");
            return ERROR_INVALID_INPUT;
        }

        printf("Password ");
        getHiddenPassword(inputPassword);
        hashPassword(inputPassword, hashedInput);

        if (authenticateUser(userID, mobileInput, hashedInput)) {
            // OTP resend loop
            int otpVerified = 0;
            int resendCount = 0;
            while (!otpVerified && resendCount < 3) {
                if (!generateOTP(userID, otp)) {
                    printf("Failed to generate OTP. Please try again.\n");
                    return ERROR_NETWORK;
                }
                sendOTPSMS(mobileInput, otp);
                sendOTPEmail(p.email, otp);
                printf("OTP sent to your mobile and email.\n");
                while (1) {
                    printf("Enter OTP (or type 'r' to resend): ");
                    char otpInput[16] = {0};
                    if (safeGetString(otpInput, sizeof(otpInput)) != SUCCESS || strlen(otpInput) == 0) {
                        printf("Invalid input.\n");
                        continue;
                    }
                    if (strcmp(otpInput, "r") == 0 || strcmp(otpInput, "R") == 0) {
                        resendCount++;
                        break; // break inner loop, resend OTP
                    }
                    if (verifyOTP(userID, otpInput)) {
                        otpVerified = 1;
                        break;
                    } else {
                        printf("Invalid OTP. Try again or type 'r' to resend.\n");
                    }
                }
            }
            if (otpVerified) {
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
                return SUCCESS;
            } else {
                printf("OTP verification failed.\n");
                attempts++;
            }
        } else {
            printf("Login failed! Invalid credentials. Try again (%d/3)\n", ++attempts);
        }
    }

    printf("Too many failed attempts. Account will be locked.\n");
    lockAccount(userID, 15); // Lock for 15 minutes
    return ERROR_AUTH_FAILED;
}