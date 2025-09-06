#ifndef CONFIG_H
#define CONFIG_H

#define MAX_LEN         100
#define MAX_SUBJECTS    10
#define MAX_CAMPUSES    4

// Campus Types
typedef enum {
    CAMPUS_NONE = 0,
    CAMPUS_SCHOOL = 1,
    CAMPUS_COLLEGE = 2,
    CAMPUS_HOSPITAL = 3,
    CAMPUS_HOSTEL = 4,
    CAMPUS_AMOUNT = 5
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

// Error Codes
typedef enum {
    SUCCESS = 0,
    ERROR_GENERAL = 1,
    ERROR_AUTH_FAILED = 2,
    ERROR_DATABASE = 3,
    ERROR_FILE_IO = 4,
    ERROR_INVALID_INPUT = 5,
    ERROR_MEMORY = 6,
    ERROR_NETWORK = 7,
    ERROR_PERMISSION = 8,
    ERROR_NOT_FOUND = 9,
    ERROR_ALREADY_EXISTS = 10
} ErrorCode;

// Return value constants
#define RETURN_SUCCESS    0
#define RETURN_FAILURE    1
#define RETURN_TRUE       1
#define RETURN_FALSE      0

#endif // CONFIG_H
