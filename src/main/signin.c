#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../include/auth.h"
#include "../include/student.h"
#include "../include/ui.h"
#include"../include/config.h"
#include"../include/fileio.h"
#include"../include/utils.h"
#include"../include/hpdf.h"

void signin() {
    Profile p;
    char studentID[MAX_LEN], mobileInput[15], inputPassword[MAX_LEN], hashedInput[MAX_LEN];
    int attempts = 0;

    printf("🆔 Student ID: ");
    scanf(" %s", studentID);

    char filename[150];
    getProfilePath(filename, studentID);

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("❌ Profile not found for ID: %s\n", studentID);
        return;
    }

    fread(&p, sizeof(Profile), 1, fp);
    fclose(fp);

    while (attempts < 3) {
        printf("📱 Mobile Number: ");
        scanf(" %s", mobileInput);

        getHiddenPassword(inputPassword);
        hashPassword(inputPassword, hashedInput);

        if (strcmp(p.studentID, studentID) == 0 &&
            strcmp(p.mobile, mobileInput) == 0 &&
            strcmp(p.passwordHash, hashedInput) == 0) {

            printf("✅ Login successful! Welcome, %s [%s]\n", p.name, p.studentID);
            logEvent(p.studentID, "Signin");

            FILE *userFile = fopen("logs/user.txt", "w");
            if (userFile) {
                fprintf(userFile, "%s\n", p.studentID);
                fclose(userFile);
            }

            dashboard(p.studentID);
            return;

        } else {
            printf("❌ Invalid credentials. Try again (%d/3)\n", ++attempts);
        }
    }

    printf("🔒 Too many failed attempts. Please try again later.\n");
}
