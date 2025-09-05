#ifndef UTILS_H
#define UTILS_H

// Extension & path generation
void buildProfileExt(char *ext, const char *studentID);
void getProfilePath(char *path, const char *studentID);

// Logging & audit
void logEvent(const char *studentID, const char *action);

// Utility
void sanitizeFilename(char *str);

#endif // UTILS_H
