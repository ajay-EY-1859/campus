#ifndef UTIL_H
#define UTIL_H

// Extension & path generation
void buildProfileExt(char *ext, const char *studentID);
void getProfilePath(char *path, const char *studentID);

// Logging & audit
void logEvent(const char *studentID, const char *action);

// Extra utility (optional expansion)
void sanitizeFilename(char *str);

#endif
