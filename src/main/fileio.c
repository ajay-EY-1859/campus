#include <stdio.h>
#include <string.h>
#include "../include/fileio.h"
#include "../include/utils.h"
#include "../include/config.h"
#include"../include/UI.h"
#include"../include/auth.h"
#include"../include/student.h"

int validateProfile(const Profile *p) {
    if (strlen(p->studentID) == 0) return 0;
    if (p->subjectCount < 0 || p->subjectCount > MAX_SUBJECTS) return 0;
    if (!validateEmail(p->email)) return 0;
    if (!validateMobile(p->mobile)) return 0;
    return 1;
}

int readProfile(Profile *p, const char *studentID) {
    char path[150];
    getProfilePath(path, studentID);

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        logEvent(studentID, "❌ Failed to open profile for reading");
        return 0;
    }

    fread(p, sizeof(Profile), 1, fp);
    fclose(fp);
    return 1;
}

int writeProfile(const Profile *p, const char *studentID) {
    if (!validateProfile(p)) {
        logEvent(studentID, "🚫 Invalid profile data — write aborted");
        return 0;
    }

    char path[150];
    getProfilePath(path, studentID);

    FILE *fp = fopen(path, "wb");
    if (!fp) {
        logEvent(studentID, "❌ Failed to open profile for writing");
        return 0;
    }

    fwrite(p, sizeof(Profile), 1, fp);
    fclose(fp);
    return 1;
}
