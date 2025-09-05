#ifndef FILEIO_H
#define FILEIO_H

#include "config.h"    // for MAX_LEN etc.

#include "auth.h" // for Profile struct


// Profile file handling
int readProfile(Profile *p, const char *studentID);
int writeProfile(const Profile *p, const char *studentID);

// Profile validator
int validateProfile(const Profile *p);

#endif // FILEIO_H
