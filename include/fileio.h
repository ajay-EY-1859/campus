#ifndef FILEIO_H
#define FILEIO_H

#include "auth.h"

// Profile file handling
int readProfile(Profile *p, const char *studentID);
int writeProfile(const Profile *p, const char *studentID);

// Subject data handling (optional extension)
int validateProfile(const Profile *p); // 🔍 validation before write

#endif
