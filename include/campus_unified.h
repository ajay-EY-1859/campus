#ifndef CAMPUS_UNIFIED_H
#define CAMPUS_UNIFIED_H

#include <stdbool.h>
#include <time.h>
#include "config.h"
#include "auth.h"

// Unified Campus Data Structure (Lego Bricking Approach)
typedef struct {
    // Common fields for all campus types
    char userID[20];
    char name[MAX_LEN];
    char instituteName[MAX_LEN];
    char department[50];
    CampusType campusType;
    
    // School/College specific (grades/marks)
    int subjectCount;
    char subjects[MAX_SUBJECTS][MAX_LEN];
    int marks[MAX_SUBJECTS];
    int fullMarks[MAX_SUBJECTS];  // School only
    int credits[MAX_SUBJECTS];    // College only
    
    // Hospital specific (medical data)
    char bloodPressure[20];
    char temperature[10];
    char weight[10];
    char diagnosis[MAX_LEN];
    
    // Hostel specific (accommodation)
    char roomNumber[10];
    char floor[10];
    char messPlan[50];
    
    // Metadata
    time_t lastUpdated;
    int dataVersion;
} UnifiedCampusData;

// Campus Configuration Structure
typedef struct {
    CampusType type;
    bool usesGrades;
    bool usesCredits;
    bool usesMedicalData;
    bool usesAccommodation;
    const char* dataTypeName;
    const char* errorPrefix;
} CampusConfig;

// Helper Functions
FILE* loadFileForStudent(const char* studentID, const char* mode, const char* campusName);
ErrorCode saveUnifiedCampusData(const UnifiedCampusData* data);
ErrorCode loadUnifiedCampusData(const char* userID, UnifiedCampusData* data);
CampusConfig getCampusConfig(CampusType type);
ErrorCode validateCampusData(const UnifiedCampusData* data);

#endif // CAMPUS_UNIFIED_H