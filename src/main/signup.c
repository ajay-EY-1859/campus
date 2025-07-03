#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../include/auth.h"
#include "../include/utils.h"
#include "../include/fileio.h"
#include "../include/config.h"
#include"../include/student.h"
#include"../include/ui.h"

void generateStudentID(Profile *p) {
    char initials[3] = { tolower(p->schoolName[0]), tolower(p->schoolName[1]), '\0' };
    srand(time(NULL));
    int serial = rand() % 900 + 100;
    snprintf(p->studentID, sizeof(p->studentID), "%s25%d", initials, serial);
}

void signup() {
    Profile p;
    char password[MAX_LEN];

    printf("👤 Enter your name: ");
    scanf(" %[^\n]", p.name);

    printf("🏫 Enter school name: ");
    scanf(" %[^\n]", p.schoolName);

    printf("🎓 Stream: ");
    scanf(" %[^\n]", p.stream);

    printf("📘 Number of subjects: ");
    scanf("%d", &p.subjectCount);
    if (p.subjectCount > MAX_SUBJECTS) p.subjectCount = MAX_SUBJECTS;

    for (int i = 0; i < p.subjectCount; i++) {
        printf("   Subject %d: ", i + 1);
        scanf(" %[^\n]", p.subjects[i]);
    }

    do {
        printf("📧 Email: ");
        scanf(" %s", p.email);
    } while (!validateEmail(p.email));

    do {
        printf("📱 Mobile (10-digit): ");
        scanf(" %s", p.mobile);
    } while (!validateMobile(p.mobile));

    getHiddenPassword(password);
    hashPassword(password, p.passwordHash);
    generateStudentID(&p);

    if (!writeProfile(&p, p.studentID)) {
        printf("❌ Failed to create profile file.\n");
        return;
    }

    logEvent(p.studentID, "Signup");

    FILE *userFile = fopen(USER_STATE_FILE, "w");
    if (userFile) {
        fprintf(userFile, "%s\n", p.studentID);
        fclose(userFile);
    }

    printf("✅ Signup successful! Your Student ID: %s\n", p.studentID);
}
