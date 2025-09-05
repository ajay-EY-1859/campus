#ifndef CONFIG_H
#define CONFIG_H

#define MAX_LEN         100
#define MAX_SUBJECTS    10
#define MAX_CAMPUSES    4

// Campus Types
typedef enum {
    CAMPUS_SCHOOL = 1,
    CAMPUS_COLLEGE = 2,
    CAMPUS_HOSPITAL = 3,
    CAMPUS_HOSTEL = 4
} CampusType;

// Paths
#define DATA_DIR        "data/"
#define CRED_DIR        "credentials/"
#define LOG_DIR         "logs/"

// Extensions
#define PROFILE_EXT     ".pfx"
#define SUBJECT_EXT     ".data"
#define PDF_EXT         "_result.pdf"

// Files
#define USER_STATE_FILE LOG_DIR "user.txt"
#define AUDIT_LOG_FILE  LOG_DIR "login_audit.log"

// App Info
#define APP_NAME        "Campus Management System"
#define APP_VERSION     "v2.0"

#endif // CONFIG_H
