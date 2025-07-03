
#ifndef CONFIG_H
#define CONFIG_H

// 🔢 General Constants
#define MAX_LEN         100
#define MAX_SUBJECTS    10

// 📂 Directory Paths
#define DATA_DIR        "data/"
#define CRED_DIR        "credentials/"
#define LOG_DIR         "logs/"

// 📦 File Extensions
#define PROFILE_EXT     ".pfx"
#define SUBJECT_EXT     ".ajstu"
#define PDF_EXT         "_result.pdf"

// 🧾 File Names
#define USER_STATE_FILE LOG_DIR "user.txt"
#define AUDIT_LOG_FILE  LOG_DIR "login_audit.log"
// 🏷️ App Info
#define APP_NAME    "Ajay Campus"
#define APP_VERSION "v1.0"

#endif
