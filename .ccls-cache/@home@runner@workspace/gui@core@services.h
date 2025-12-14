#ifndef SERVICES_H
#define SERVICES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 100
#define MAX_SUBJECTS 10
#define DATA_DIR "data/"
#define CRED_DIR "credentials/"
#define LOG_DIR "logs/"
#define APP_NAME "Campus Management System"
#define APP_VERSION "v2.0 GUI"

typedef enum {
    CAMPUS_NONE = 0,
    CAMPUS_SCHOOL = 1,
    CAMPUS_COLLEGE = 2,
    CAMPUS_HOSPITAL = 3,
    CAMPUS_HOSTEL = 4
} CampusType;

typedef enum {
    SUCCESS = 0,
    ERROR_GENERAL = 1,
    ERROR_AUTH_FAILED = 2,
    ERROR_DATABASE = 3,
    ERROR_FILE_IO = 4,
    ERROR_INVALID_INPUT = 5,
    ERROR_NOT_FOUND = 9,
    ERROR_ALREADY_EXISTS = 10
} ErrorCode;

typedef struct {
    char userID[MAX_LEN];
    char name[MAX_LEN];
    char email[MAX_LEN];
    char mobile[MAX_LEN];
    char passwordHash[MAX_LEN];
    CampusType campusType;
    char campusName[MAX_LEN];
    time_t createdAt;
    time_t lastLogin;
} Profile;

typedef struct {
    char subjectName[MAX_LEN];
    int marks;
    int maxMarks;
} SubjectData;

typedef struct {
    int numSubjects;
    SubjectData subjects[MAX_SUBJECTS];
    float percentage;
    char grade[10];
} SchoolData;

typedef struct {
    char courseName[MAX_LEN];
    int credits;
    float gpa;
    float cgpa;
} CollegeData;

typedef struct {
    char patientID[MAX_LEN];
    char diagnosis[MAX_LEN];
    char doctorName[MAX_LEN];
    char treatment[MAX_LEN];
    time_t admissionDate;
} HospitalData;

typedef struct {
    char roomNumber[MAX_LEN];
    char blockName[MAX_LEN];
    int floorNumber;
    char roomType[MAX_LEN];
    float monthlyRent;
} HostelData;

ErrorCode init_database(void);
void close_database(void);
ErrorCode create_user(const Profile *profile);
ErrorCode get_user_by_id(const char *userID, Profile *profile);
ErrorCode authenticate_user(const char *userID, const char *mobile, const char *password);
ErrorCode update_user(const Profile *profile);
ErrorCode save_campus_data(const char *userID, CampusType type, const void *data);
ErrorCode load_campus_data(const char *userID, CampusType type, void *data);
void generate_user_id(char *userID, CampusType type);
void hash_password(const char *password, char *hash);
int validate_email(const char *email);
int validate_mobile(const char *mobile);
int validate_password(const char *password);
void log_activity(const char *userID, const char *action, const char *details);
const char* get_campus_name(CampusType type);
void calculate_school_grade(SchoolData *data);
void calculate_college_cgpa(CollegeData *data);

#endif