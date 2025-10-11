#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include "../include/campus_unified.h"
#include "../include/utils.h"
#include "../include/student.h"

// Campus Configuration Mapping
CampusConfig getCampusConfig(CampusType type) {
    static CampusConfig configs[] = {
        {CAMPUS_NONE, false, false, false, false, "None", "Invalid"},
        {CAMPUS_SCHOOL, true, false, false, false, "school", "School"},
        {CAMPUS_COLLEGE, true, true, false, false, "college", "College"},
        {CAMPUS_HOSPITAL, false, false, true, false, "medical", "Hospital"},
        {CAMPUS_HOSTEL, false, false, false, true, "hostel", "Hostel"}
    };
    
    if (type >= CAMPUS_NONE && type < CAMPUS_AMOUNT) {
        return configs[type];
    }
    return configs[CAMPUS_NONE]; // Default to NONE for invalid types
}

// Unified File Loading Helper
FILE* loadFileForStudent(const char* studentID, const char* mode, const char* campusName) {
    if (!studentID || !mode) return NULL;
    
    char datafile[150];
    int written = snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", studentID);
    if (written < 0 || written >= (int)sizeof(datafile)) return NULL;
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    
    FILE *f = fopen(datafile, mode);
    if (!f && campusName) {
        printf("No %s data found for ID: %s\n", campusName, studentID);
    }
    
    return f;
}

// Unified Campus Data Save
ErrorCode saveUnifiedCampusData(const UnifiedCampusData* data) {
    if (!data) return ERROR_INVALID_INPUT;
    
    CampusConfig config = getCampusConfig(data->campusType);
    if (config.type == CAMPUS_NONE) return ERROR_INVALID_INPUT;
    
    FILE *f = loadFileForStudent(data->userID, "wb", NULL);
    if (!f) return ERROR_FILE_IO;
    
    // Save unified data structure
    size_t written = fwrite(data, sizeof(UnifiedCampusData), 1, f);
    fclose(f);
    
    if (written != 1) return ERROR_FILE_IO;
    
    printf("%s data saved successfully\n", config.errorPrefix);
    return SUCCESS;
}

// Unified Campus Data Load
ErrorCode loadUnifiedCampusData(const char* userID, UnifiedCampusData* data) {
    if (!userID || !data) return ERROR_INVALID_INPUT;
    
    FILE *f = loadFileForStudent(userID, "rb", "campus");
    if (!f) return ERROR_NOT_FOUND;
    
    size_t read = fread(data, sizeof(UnifiedCampusData), 1, f);
    fclose(f);
    
    if (read != 1) return ERROR_FILE_IO;
    
    return SUCCESS;
}

// Unified Data Validation
ErrorCode validateCampusData(const UnifiedCampusData* data) {
    if (!data) return ERROR_INVALID_INPUT;
    
    CampusConfig config = getCampusConfig(data->campusType);
    if (config.type == CAMPUS_NONE) return ERROR_INVALID_INPUT;
    
    // Validate based on campus type
    if (config.usesGrades) {
        if (data->subjectCount <= 0 || data->subjectCount > MAX_SUBJECTS) {
            return ERROR_INVALID_INPUT;
        }
        
        for (int i = 0; i < data->subjectCount; i++) {
            if (data->marks[i] < 0 || data->marks[i] > 100) {
                return ERROR_INVALID_INPUT;
            }
            
            if (config.type == CAMPUS_SCHOOL) {
                if (data->fullMarks[i] <= 0 || data->fullMarks[i] > 100) {
                    return ERROR_INVALID_INPUT;
                }
            }
            
            if (config.usesCredits) {
                if (data->credits[i] <= 0 || data->credits[i] > 10) {
                    return ERROR_INVALID_INPUT;
                }
            }
        }
    }
    
    if (config.usesMedicalData) {
        if (strlen(data->bloodPressure) == 0 || strlen(data->temperature) == 0) {
            return ERROR_INVALID_INPUT;
        }
    }
    
    if (config.usesAccommodation) {
        if (strlen(data->roomNumber) == 0 || strlen(data->floor) == 0) {
            return ERROR_INVALID_INPUT;
        }
    }
    
    return SUCCESS;
}

// Unified Save Function (replaces individual save functions)
ErrorCode saveCampusData(const char* userID, CampusType campusType) {
    if (!userID) return ERROR_INVALID_INPUT;
    
    CampusConfig config = getCampusConfig(campusType);
    if (config.type == CAMPUS_NONE) return ERROR_INVALID_INPUT;
    
    UnifiedCampusData data = {0};
    strncpy(data.userID, userID, sizeof(data.userID) - 1);
    data.campusType = campusType;
    data.lastUpdated = time(NULL);
    data.dataVersion = 1;
    
    printf("\nEnter %s data:\n", config.errorPrefix);
    
    if (config.usesGrades) {
        printf("Number of %s: ", config.type == CAMPUS_SCHOOL ? "subjects" : "courses");
        if (safeGetInt(&data.subjectCount, 1, MAX_SUBJECTS) != SUCCESS) {
            return ERROR_INVALID_INPUT;
        }
        
        for (int i = 0; i < data.subjectCount; i++) {
            printf("%s %d: ", config.type == CAMPUS_SCHOOL ? "Subject" : "Course", i + 1);
            if (safeGetString(data.subjects[i], sizeof(data.subjects[i])) != SUCCESS) {
                return ERROR_INVALID_INPUT;
            }
            
            printf("Marks: ");
            if (safeGetInt(&data.marks[i], 0, 100) != SUCCESS) {
                return ERROR_INVALID_INPUT;
            }
            
            if (config.type == CAMPUS_SCHOOL) {
                printf("Full marks: ");
                if (safeGetInt(&data.fullMarks[i], 1, 100) != SUCCESS) {
                    return ERROR_INVALID_INPUT;
                }
            }
            
            if (config.usesCredits) {
                printf("Credits: ");
                if (safeGetInt(&data.credits[i], 1, 10) != SUCCESS) {
                    return ERROR_INVALID_INPUT;
                }
            }
        }
    }
    
    if (config.usesMedicalData) {
        printf("Blood Pressure: ");
        if (safeGetString(data.bloodPressure, sizeof(data.bloodPressure)) != SUCCESS) {
            return ERROR_INVALID_INPUT;
        }
        
        printf("Temperature: ");
        if (safeGetString(data.temperature, sizeof(data.temperature)) != SUCCESS) {
            return ERROR_INVALID_INPUT;
        }
        
        printf("Weight: ");
        if (safeGetString(data.weight, sizeof(data.weight)) != SUCCESS) {
            return ERROR_INVALID_INPUT;
        }
        
        printf("Diagnosis: ");
        if (safeGetString(data.diagnosis, sizeof(data.diagnosis)) != SUCCESS) {
            return ERROR_INVALID_INPUT;
        }
    }
    
    if (config.usesAccommodation) {
        printf("Room Number: ");
        if (safeGetString(data.roomNumber, sizeof(data.roomNumber)) != SUCCESS) {
            return ERROR_INVALID_INPUT;
        }
        
        printf("Floor: ");
        if (safeGetString(data.floor, sizeof(data.floor)) != SUCCESS) {
            return ERROR_INVALID_INPUT;
        }
        
        printf("Mess Plan: ");
        if (safeGetString(data.messPlan, sizeof(data.messPlan)) != SUCCESS) {
            return ERROR_INVALID_INPUT;
        }
    }
    
    // Validate and save
    ErrorCode validation = validateCampusData(&data);
    if (validation != SUCCESS) return validation;
    
    return saveUnifiedCampusData(&data);
}

// Unified Load Function (replaces individual load functions)
ErrorCode loadCampusData(const char* userID) {
    if (!userID) return ERROR_INVALID_INPUT;
    
    UnifiedCampusData data = {0};
    ErrorCode result = loadUnifiedCampusData(userID, &data);
    if (result != SUCCESS) return result;
    
    CampusConfig config = getCampusConfig(data.campusType);
    
    printf("\n%s Data for %s:\n", config.errorPrefix, userID);
    
    if (config.usesGrades) {
        int total = 0, totalFull = 0, totalCredits = 0;
        
        for (int i = 0; i < data.subjectCount; i++) {
            if (config.type == CAMPUS_SCHOOL) {
                printf("%d. %s - %d/%d\n", i+1, data.subjects[i], data.marks[i], data.fullMarks[i]);
                total += data.marks[i];
                totalFull += data.fullMarks[i];
            } else if (config.usesCredits) {
                printf("%d. %s - %d marks (%d credits)\n", i+1, data.subjects[i], data.marks[i], data.credits[i]);
                total += data.marks[i] * data.credits[i];
                totalCredits += data.credits[i];
            }
        }
        
        if (config.type == CAMPUS_SCHOOL) {
            printSummary(total, totalFull, data.campusType);
        } else if (config.usesCredits) {
            float cgpa = totalCredits > 0 ? (float)total / (totalCredits * 10.0f) : 0.0f;
            printf("CGPA: %.2f\n", cgpa);
        }
    }
    
    if (config.usesMedicalData) {
        printf("Blood Pressure: %s\n", data.bloodPressure);
        printf("Temperature: %s\n", data.temperature);
        printf("Weight: %s\n", data.weight);
        printf("Diagnosis: %s\n", data.diagnosis);
    }
    
    if (config.usesAccommodation) {
        printf("Room Number: %s\n", data.roomNumber);
        printf("Floor: %s\n", data.floor);
        printf("Mess Plan: %s\n", data.messPlan);
    }
    
    return SUCCESS;
}