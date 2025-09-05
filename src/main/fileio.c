#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include "../include/fileio.h"
#include "../include/utils.h"
#include "../include/config.h"
#include "../include/ui.h"
#include "../include/auth.h"
#include "../include/student.h"

int validateProfile(const Profile *p) {
    if (strlen(p->userID) == 0) return 0;
    if (p->dataCount < 0 || p->dataCount > MAX_SUBJECTS) return 0;
    if (!validateEmail(p->email)) return 0;
    if (!validateMobile(p->mobile)) return 0;
    return 1;
}

int readProfile(Profile *p, const char *userID) {
    char path[150] = {0};
    getProfilePath(path, userID);

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        logEvent(userID, "Failed to open profile for reading");
        return 0;
    }

    size_t result = fread(p, sizeof(Profile), 1, fp);
    fclose(fp);
    return (result == 1) ? 1 : 0;
}

int writeProfile(const Profile *p, const char *userID) {
    if (!validateProfile(p)) {
        logEvent(userID, "Invalid profile data - write aborted");
        return 0;
    }

    char path[150] = {0};
    getProfilePath(path, userID);

    // Ensure credentials directory exists
#ifdef _WIN32
    _mkdir("credentials");
#else
    mkdir("credentials", 0777);
#endif

    FILE *fp = fopen(path, "wb");
    if (!fp) {
        logEvent(userID, "Failed to open profile for writing");
        return 0;
    }

    size_t result = fwrite(p, sizeof(Profile), 1, fp);
    fclose(fp);
    return (result == 1) ? 1 : 0;
}