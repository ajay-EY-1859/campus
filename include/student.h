#ifndef STUDENT_H
#define STUDENT_H

#include "config.h"

// Campus-specific data management
void saveSchoolData(const char *studentID);
void saveCollegeData(const char *studentID);
void saveHospitalData(const char *patientID);
void saveHostelData(const char *residentID);

void loadSchoolData(const char *studentID);
void loadCollegeData(const char *studentID);
void loadHospitalData(const char *patientID);
void loadHostelData(const char *residentID);

void exportSchoolPDF(const char *studentID);
void exportCollegePDF(const char *studentID);
void exportHospitalPDF(const char *patientID);
void exportHostelPDF(const char *residentID);

// Utilities
const char* getGrade(float percent);
void printSummary(int total, int full, CampusType type);
const char* getCampusName(CampusType type);

#endif // STUDENT_H
