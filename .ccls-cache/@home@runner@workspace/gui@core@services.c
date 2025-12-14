#include "services.h"
#include <sys/stat.h>
#include <dirent.h>

static const char *DB_PATH = "data/campus.db";

ErrorCode init_database(void) {
    mkdir("data", 0755);
    mkdir("credentials", 0755);
    mkdir("logs", 0755);
    
    FILE *f = fopen(DB_PATH, "a+");
    if (!f) return ERROR_DATABASE;
    fclose(f);
    return SUCCESS;
}

void close_database(void) {}

void generate_user_id(char *userID, CampusType type) {
    const char *prefix;
    switch(type) {
        case CAMPUS_SCHOOL: prefix = "SCH"; break;
        case CAMPUS_COLLEGE: prefix = "COL"; break;
        case CAMPUS_HOSPITAL: prefix = "HOS"; break;
        case CAMPUS_HOSTEL: prefix = "HST"; break;
        default: prefix = "USR"; break;
    }
    time_t now = time(NULL);
    snprintf(userID, MAX_LEN, "%s%ld", prefix, now % 1000000);
}

void hash_password(const char *password, char *hash) {
    unsigned long h = 5381;
    int c;
    while ((c = *password++))
        h = ((h << 5) + h) + c;
    snprintf(hash, MAX_LEN, "%lx", h);
}

int validate_email(const char *email) {
    if (!email || strlen(email) < 5) return 0;
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return at && dot && at < dot && dot - at > 1;
}

int validate_mobile(const char *mobile) {
    if (!mobile || strlen(mobile) != 10) return 0;
    for (int i = 0; i < 10; i++)
        if (mobile[i] < '0' || mobile[i] > '9') return 0;
    return 1;
}

int validate_password(const char *password) {
    if (!password || strlen(password) < 8) return 0;
    int hasUpper = 0, hasLower = 0, hasDigit = 0;
    for (const char *p = password; *p; p++) {
        if (*p >= 'A' && *p <= 'Z') hasUpper = 1;
        if (*p >= 'a' && *p <= 'z') hasLower = 1;
        if (*p >= '0' && *p <= '9') hasDigit = 1;
    }
    return hasUpper && hasLower && hasDigit;
}

ErrorCode create_user(const Profile *profile) {
    if (!profile) return ERROR_INVALID_INPUT;
    
    FILE *f = fopen(DB_PATH, "ab");
    if (!f) return ERROR_FILE_IO;
    
    char header[] = "USER_RECORD";
    fwrite(header, 12, 1, f);
    fwrite(profile, sizeof(Profile), 1, f);
    fclose(f);
    
    log_activity(profile->userID, "USER_CREATED", "New user registered via GUI");
    return SUCCESS;
}

ErrorCode get_user_by_id(const char *userID, Profile *profile) {
    FILE *f = fopen(DB_PATH, "rb");
    if (!f) return ERROR_FILE_IO;
    
    char header[12] = {0};
    Profile temp;
    
    while (fread(header, 12, 1, f) == 1) {
        if (strcmp(header, "USER_RECORD") == 0) {
            if (fread(&temp, sizeof(Profile), 1, f) == 1) {
                if (strcmp(temp.userID, userID) == 0) {
                    *profile = temp;
                    fclose(f);
                    return SUCCESS;
                }
            }
        } else {
            fseek(f, sizeof(Profile), SEEK_CUR);
        }
    }
    
    fclose(f);
    return ERROR_NOT_FOUND;
}

ErrorCode authenticate_user(const char *userID, const char *mobile, const char *password) {
    Profile profile;
    if (get_user_by_id(userID, &profile) != SUCCESS) {
        return ERROR_NOT_FOUND;
    }
    
    char hash[MAX_LEN];
    hash_password(password, hash);
    
    if (strcmp(profile.mobile, mobile) == 0 && strcmp(profile.passwordHash, hash) == 0) {
        log_activity(userID, "LOGIN_SUCCESS", "User logged in via GUI");
        return SUCCESS;
    }
    
    log_activity(userID, "LOGIN_FAILED", "Authentication failed");
    return ERROR_AUTH_FAILED;
}

ErrorCode update_user(const Profile *profile) {
    if (!profile) return ERROR_INVALID_INPUT;
    
    FILE *f = fopen(DB_PATH, "rb");
    FILE *temp = fopen("data/temp.db", "wb");
    if (!f || !temp) {
        if (f) fclose(f);
        if (temp) fclose(temp);
        return ERROR_FILE_IO;
    }
    
    char header[12];
    Profile tempProfile;
    int updated = 0;
    
    while (fread(header, 12, 1, f) == 1) {
        if (strcmp(header, "USER_RECORD") == 0) {
            if (fread(&tempProfile, sizeof(Profile), 1, f) == 1) {
                fwrite(header, 12, 1, temp);
                if (strcmp(tempProfile.userID, profile->userID) == 0) {
                    fwrite(profile, sizeof(Profile), 1, temp);
                    updated = 1;
                } else {
                    fwrite(&tempProfile, sizeof(Profile), 1, temp);
                }
            }
        }
    }
    
    fclose(f);
    fclose(temp);
    
    if (updated) {
        remove(DB_PATH);
        rename("data/temp.db", DB_PATH);
        log_activity(profile->userID, "USER_UPDATED", "Profile updated");
        return SUCCESS;
    }
    
    remove("data/temp.db");
    return ERROR_NOT_FOUND;
}

ErrorCode save_campus_data(const char *userID, CampusType type, const void *data) {
    char filename[256];
    size_t dataSize;
    
    switch(type) {
        case CAMPUS_SCHOOL: dataSize = sizeof(SchoolData); break;
        case CAMPUS_COLLEGE: dataSize = sizeof(CollegeData); break;
        case CAMPUS_HOSPITAL: dataSize = sizeof(HospitalData); break;
        case CAMPUS_HOSTEL: dataSize = sizeof(HostelData); break;
        default: return ERROR_INVALID_INPUT;
    }
    
    snprintf(filename, sizeof(filename), "data/%s_campus.dat", userID);
    FILE *f = fopen(filename, "wb");
    if (!f) return ERROR_FILE_IO;
    
    fwrite(&type, sizeof(CampusType), 1, f);
    fwrite(data, dataSize, 1, f);
    fclose(f);
    
    log_activity(userID, "DATA_SAVED", "Campus data saved");
    return SUCCESS;
}

ErrorCode load_campus_data(const char *userID, CampusType type, void *data) {
    char filename[256];
    size_t dataSize;
    
    switch(type) {
        case CAMPUS_SCHOOL: dataSize = sizeof(SchoolData); break;
        case CAMPUS_COLLEGE: dataSize = sizeof(CollegeData); break;
        case CAMPUS_HOSPITAL: dataSize = sizeof(HospitalData); break;
        case CAMPUS_HOSTEL: dataSize = sizeof(HostelData); break;
        default: return ERROR_INVALID_INPUT;
    }
    
    snprintf(filename, sizeof(filename), "data/%s_campus.dat", userID);
    FILE *f = fopen(filename, "rb");
    if (!f) return ERROR_NOT_FOUND;
    
    CampusType storedType;
    fread(&storedType, sizeof(CampusType), 1, f);
    
    if (storedType != type) {
        fclose(f);
        return ERROR_INVALID_INPUT;
    }
    
    fread(data, dataSize, 1, f);
    fclose(f);
    return SUCCESS;
}

void log_activity(const char *userID, const char *action, const char *details) {
    FILE *f = fopen("logs/activity.log", "a");
    if (!f) return;
    
    time_t now = time(NULL);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(f, "[%s] User: %s | Action: %s | Details: %s\n", 
            timeStr, userID ? userID : "SYSTEM", action, details);
    fclose(f);
}

const char* get_campus_name(CampusType type) {
    switch(type) {
        case CAMPUS_SCHOOL: return "School";
        case CAMPUS_COLLEGE: return "College";
        case CAMPUS_HOSPITAL: return "Hospital";
        case CAMPUS_HOSTEL: return "Hostel";
        default: return "Unknown";
    }
}

void calculate_school_grade(SchoolData *data) {
    if (!data || data->numSubjects == 0) return;
    
    int totalMarks = 0, totalMax = 0;
    for (int i = 0; i < data->numSubjects; i++) {
        totalMarks += data->subjects[i].marks;
        totalMax += data->subjects[i].maxMarks;
    }
    
    data->percentage = (float)totalMarks / totalMax * 100;
    
    if (data->percentage >= 90) strcpy(data->grade, "A+");
    else if (data->percentage >= 80) strcpy(data->grade, "A");
    else if (data->percentage >= 70) strcpy(data->grade, "B");
    else if (data->percentage >= 60) strcpy(data->grade, "C");
    else if (data->percentage >= 50) strcpy(data->grade, "D");
    else strcpy(data->grade, "F");
}

void calculate_college_cgpa(CollegeData *data) {
    if (!data) return;
    data->cgpa = data->gpa;
}