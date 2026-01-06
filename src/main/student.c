#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include "../include/student.h"
#include "../include/auth.h"
#include "../include/config.h"

#ifndef HPDF_DISABLED
#include "../include/hpdf/hpdf.h"
#endif
#include "../include/config.h"
#include "../include/fileio.h"
#include "../include/utils.h"
#include "../include/hpdf/hpdf.h"
#include "../include/ui.h"
#include"../include/database.h"

const char* getCampusName(CampusType type) {
    switch(type) {
        case CAMPUS_SCHOOL: return "School";
        case CAMPUS_COLLEGE: return "College";
        case CAMPUS_HOSPITAL: return "Hospital";
        case CAMPUS_HOSTEL: return "Hostel";
        default: return "Unknown";
    }
}

const char* getGrade(float percent) {
    if (percent >= 90.0f) return "A+";
    else if (percent >= 80.0f) return "A";
    else if (percent >= 70.0f) return "B";
    else if (percent >= 60.0f) return "C";
    else if (percent >= 50.0f) return "D";
    else return "F";
}

void printSummary(int total, int full, CampusType type) {
    float percentage = (full == 0) ? 0.0f : (total * 100.0f / full);
    printf("Total: %d / %d\n", total, full);
    printf("Percentage: %.2f%%\n", percentage);
    if (type == CAMPUS_SCHOOL || type == CAMPUS_COLLEGE) {
        printf("Grade: %s\n", getGrade(percentage));
    }
}



// School Data Management
void saveSchoolData(const char *studentID) {
    Profile p = {0};
    if (!getUserByID(studentID, &p)) {
        printf("Cannot load profile\n");
        return;
    }
    
    int marks[MAX_SUBJECTS] = {0}, fullMarks[MAX_SUBJECTS] = {0};
    printf("\nEnter marks for subjects:\n");
    for (int i = 0; i < p.dataCount; i++) {
        while (1) {
            printf("%s - Marks: ", p.dataFields[i]);
            if (safeGetInt(&marks[i], 0, 100) == SUCCESS) break;
            printf("Invalid marks. 0-100.\n");
        }
        while (1) {
            printf("Full marks: ");
            if (safeGetInt(&fullMarks[i], 1, 100) == SUCCESS) break;
            printf("Invalid full marks. 1-100.\n");
        }
    }
    
    // Pack data
    struct {
        int count;
        char subjects[MAX_SUBJECTS][MAX_LEN];
        int marks[MAX_SUBJECTS];
        int fullMarks[MAX_SUBJECTS];
    } data;
    
    data.count = p.dataCount;
    memcpy(data.subjects, p.dataFields, sizeof(data.subjects));
    memcpy(data.marks, marks, sizeof(marks));
    memcpy(data.fullMarks, fullMarks, sizeof(fullMarks));
    
    if (saveUserData(studentID, "SCHOOL_DATA", &data, sizeof(data))) {
        printf("School marks saved to database.\n");
    } else {
        printf("Failed to save data.\n");
    }
}

void loadSchoolData(const char *studentID) {
    struct {
        int count;
        char subjects[MAX_SUBJECTS][MAX_LEN];
        int marks[MAX_SUBJECTS];
        int fullMarks[MAX_SUBJECTS];
    } data;
    size_t size = sizeof(data);
    
    if (!loadUserData(studentID, "SCHOOL_DATA", &data, &size)) {
        printf("No school data found\n");
        return;
    }
    
    int total = 0, totalFull = 0;
    for (int i = 0; i < data.count; i++) {
        printf("%d. %s - %d/%d\n", i+1, data.subjects[i], data.marks[i], data.fullMarks[i]);
        total += data.marks[i];
        totalFull += data.fullMarks[i];
    }
    printSummary(total, totalFull, CAMPUS_SCHOOL);
}

// College Data Management
void saveCollegeData(const char *studentID) {
    Profile p = {0};
    if (!getUserByID(studentID, &p)) {
        printf("Cannot load profile\n");
        return;
    }
    
    int marks[MAX_SUBJECTS] = {0}, credits[MAX_SUBJECTS] = {0};
    printf("\nEnter semester marks:\n");
    for (int i = 0; i < p.dataCount; i++) {
        while (1) {
            printf("%s - Marks: ", p.dataFields[i]);
            if (safeGetInt(&marks[i], 0, 100) == SUCCESS) break;
            printf("Invalid marks. 0-100.\n");
        }
        while (1) {
            printf("Credits: ");
            if (safeGetInt(&credits[i], 1, 10) == SUCCESS) break;
            printf("Invalid credits. 1-10.\n");
        }
    }
    
    struct {
        int count;
        char subjects[MAX_SUBJECTS][MAX_LEN];
        int marks[MAX_SUBJECTS];
        int credits[MAX_SUBJECTS];
    } data;
    
    data.count = p.dataCount;
    memcpy(data.subjects, p.dataFields, sizeof(data.subjects));
    memcpy(data.marks, marks, sizeof(marks));
    memcpy(data.credits, credits, sizeof(credits));
    
    if (saveUserData(studentID, "COLLEGE_DATA", &data, sizeof(data))) {
        printf("College marks saved to database.\n");
    } else {
        printf("Failed to save data.\n");
    }
}

void loadCollegeData(const char *studentID) {
    struct {
        int count;
        char subjects[MAX_SUBJECTS][MAX_LEN];
        int marks[MAX_SUBJECTS];
        int credits[MAX_SUBJECTS];
    } data;
    size_t size = sizeof(data);
    
    if (!loadUserData(studentID, "COLLEGE_DATA", &data, &size)) {
        printf("No college data found\n");
        return;
    }
    
    int totalMarks = 0, totalCredits = 0;
    for (int i = 0; i < data.count; i++) {
        printf("%d. %s - %d marks (%d credits)\n", i+1, data.subjects[i], data.marks[i], data.credits[i]);
        totalMarks += data.marks[i] * data.credits[i];
        totalCredits += data.credits[i];
    }
    float cgpa = totalCredits > 0 ? (float)totalMarks / (totalCredits * 10.0f) : 0.0f;
    printf("CGPA: %.2f\n", cgpa);
}

// Hospital Data Management
void saveHospitalData(const char *patientID) {
    Profile p = {0};
    if (!getUserByID(patientID, &p)) { printf("Cannot load profile\n"); return; }
    
    char values[MAX_SUBJECTS][MAX_LEN] = {0};
    printf("\nEnter medical data:\n");
    for (int i = 0; i < p.dataCount; i++) {
        while (1) {
            printf("%s: ", p.dataFields[i]);
            if (safeGetString(values[i], sizeof(values[i])) == SUCCESS && strlen(values[i]) > 0) break;
            printf("Invalid input.\n");
        }
    }
    
    struct {
        int count;
        char fields[MAX_SUBJECTS][MAX_LEN];
        char values[MAX_SUBJECTS][MAX_LEN];
    } data;
    
    data.count = p.dataCount;
    memcpy(data.fields, p.dataFields, sizeof(data.fields));
    memcpy(data.values, values, sizeof(data.values));
    
    if (saveUserData(patientID, "HOSPITAL_DATA", &data, sizeof(data))) {
        printf("Medical data saved to database.\n");
    } else { printf("Failed to save data.\n"); }
}

void loadHospitalData(const char *patientID) {
    struct {
        int count;
        char fields[MAX_SUBJECTS][MAX_LEN];
        char values[MAX_SUBJECTS][MAX_LEN];
    } data;
    size_t size = sizeof(data);
    
    if (!loadUserData(patientID, "HOSPITAL_DATA", &data, &size)) {
        printf("No medical data found\n");
        return;
    }
    
    for (int i = 0; i < data.count; i++) {
        printf("%s: %s\n", data.fields[i], data.values[i]);
    }
}

void saveHostelData(const char *residentID) {
    Profile p = {0};
    if (!getUserByID(residentID, &p)) { printf("Cannot load profile\n"); return; }
    
    char values[MAX_SUBJECTS][MAX_LEN] = {0};
    printf("\nEnter hostel data:\n");
    for (int i = 0; i < p.dataCount; i++) {
        while (1) {
            printf("%s: ", p.dataFields[i]);
            if (safeGetString(values[i], sizeof(values[i])) == SUCCESS && strlen(values[i]) > 0) break;
            printf("Invalid input.\n");
        }
    }
    struct {
        int count;
        char fields[MAX_SUBJECTS][MAX_LEN];
        char values[MAX_SUBJECTS][MAX_LEN];
    } data;
    
    data.count = p.dataCount;
    memcpy(data.fields, p.dataFields, sizeof(data.fields));
    memcpy(data.values, values, sizeof(data.values));
    
    if (saveUserData(residentID, "HOSTEL_DATA", &data, sizeof(data))) {
        printf("Hostel data saved to database.\n");
    } else { printf("Failed to save data.\n"); }
}

void loadHostelData(const char *residentID) {
    struct {
        int count;
        char fields[MAX_SUBJECTS][MAX_LEN];
        char values[MAX_SUBJECTS][MAX_LEN];
    } data;
    size_t size = sizeof(data);
    
    if (!loadUserData(residentID, "HOSTEL_DATA", &data, &size)) {
        printf("No hostel data found\n");
        return;
    }
    for (int i = 0; i < data.count; i++) {
        printf("%s: %s\n", data.fields[i], data.values[i]);
    }
}

// PDF Export Functions
#ifndef HPDF_DISABLED
void exportSchoolPDF(const char *studentID) {
    struct {
        int count;
        char subjects[MAX_SUBJECTS][MAX_LEN];
        int marks[MAX_SUBJECTS];
        int fullMarks[MAX_SUBJECTS];
    } data;
    size_t size = sizeof(data);
    
    if (!loadUserData(studentID, "SCHOOL_DATA", &data, &size)) {
        printf("No school data found for PDF export\n");
        return;
    }
    
    int count = data.count;
    // Unpack data for local usage if needed, or use directly
    // Using direct access from data struct for cleaner code
    int total = 0, totalFull = 0;
    for(int i=0; i<count; i++) {
        total += data.marks[i];
        totalFull += data.fullMarks[i];
    }
    
    Profile p = {0};
    if (!getUserByID(studentID, &p)) {
        printf("Cannot load profile for PDF\n");
        return;
    }

    float percentage = (totalFull == 0) ? 0.0f : (total * 100.0f / totalFull);
    const char *grade = getGrade(percentage);
    
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Failed to create PDF\n");
        return;
    }
    
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);
    
    float y = 800;
    char buffer[256];
    
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 200, y, "School Report Card");
    y -= 30;
    
    snprintf(buffer, sizeof(buffer), "Name: %s", p.name);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "School: %s", p.instituteName);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Department: %s", p.department);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 30;
    
    HPDF_Page_TextOut(page, 50, y, "Subject                Marks   Full Marks");
    y -= 20;
    
    for (int i = 0; i < count; i++) {
        snprintf(buffer, sizeof(buffer), "%-20s %5d   %5d", data.subjects[i], data.marks[i], data.fullMarks[i]);
        HPDF_Page_TextOut(page, 50, y, buffer);
        y -= 20;
    }
    
    y -= 10;
    snprintf(buffer, sizeof(buffer), "Total: %d / %d", total, totalFull);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Percentage: %.2f%%", percentage);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Grade: %s", grade);
    HPDF_Page_TextOut(page, 50, y, buffer);
    HPDF_Page_EndText(page);
    
    char outfile[150];
    snprintf(outfile, sizeof(outfile), DATA_DIR "%s_school_report.pdf", studentID);
    // Ensure data directory exists
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    HPDF_SaveToFile(pdf, outfile);
    HPDF_Free(pdf);
    
    logEvent(studentID, "School PDF exported");
    printf("School report PDF exported to %s\n", outfile);
}

void exportCollegePDF(const char *studentID) {
    struct {
        int count;
        char subjects[MAX_SUBJECTS][MAX_LEN];
        int marks[MAX_SUBJECTS];
        int credits[MAX_SUBJECTS];
    } data;
    size_t size = sizeof(data);
    
    if (!loadUserData(studentID, "COLLEGE_DATA", &data, &size)) {
        printf("No college data found for PDF export\n");
        return;
    }
    
    int count = data.count;
    int totalMarks = 0, totalCredits = 0;
    for(int i=0; i<count; i++) {
        totalMarks += data.marks[i] * data.credits[i];
        totalCredits += data.credits[i];
    }
    
    Profile p = {0};
    if (!getUserByID(studentID, &p)) {
        printf("Cannot load profile for PDF\n");
        return;
    }
    
    float cgpa = totalCredits > 0 ? (float)totalMarks / (totalCredits * 10.0f) : 0.0f;
    
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Failed to create PDF\n");
        return;
    }
    
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);
    
    float y = 800;
    char buffer[256];
    
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 200, y, "College Transcript");
    y -= 30;
    
    snprintf(buffer, sizeof(buffer), "Name: %s", p.name);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "College: %s", p.instituteName);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Department: %s", p.department);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 30;
    
    HPDF_Page_TextOut(page, 50, y, "Course                 Marks   Credits");
    y -= 20;
    
    for (int i = 0; i < count; i++) {
        snprintf(buffer, sizeof(buffer), "%-20s %5d   %5d", data.subjects[i], data.marks[i], data.credits[i]);
        HPDF_Page_TextOut(page, 50, y, buffer);
        y -= 20;
    }
    
    y -= 10;
    snprintf(buffer, sizeof(buffer), "Total Credits: %d", totalCredits);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "CGPA: %.2f", cgpa);
    HPDF_Page_TextOut(page, 50, y, buffer);
    HPDF_Page_EndText(page);
    
    char outfile[150];
    snprintf(outfile, sizeof(outfile), DATA_DIR "%s_college_transcript.pdf", studentID);
    // Ensure data directory exists
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    HPDF_SaveToFile(pdf, outfile);
    HPDF_Free(pdf);
    
    logEvent(studentID, "College PDF exported");
    printf("College transcript PDF exported to %s\n", outfile);
}

void exportHospitalPDF(const char *patientID) {
    struct {
        int count;
        char fields[MAX_SUBJECTS][MAX_LEN];
        char values[MAX_SUBJECTS][MAX_LEN];
    } data;
    size_t size = sizeof(data);
    
    if (!loadUserData(patientID, "HOSPITAL_DATA", &data, &size)) {
        printf("No medical data found for PDF export\n");
        return;
    }
    int count = data.count;
    
    Profile p = {0};
    if (!getUserByID(patientID, &p)) {
        printf("Cannot load profile for PDF\n");
        return;
    }
    
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Failed to create PDF\n");
        return;
    }
    
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);
    
    float y = 800;
    char buffer[256];
    
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 200, y, "Medical Report");
    y -= 30;
    
    snprintf(buffer, sizeof(buffer), "Patient: %s", p.name);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Hospital: %s", p.instituteName);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Department: %s", p.department);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 30;
    
    HPDF_Page_TextOut(page, 50, y, "Medical Data:");
    y -= 20;
    
    for (int i = 0; i < count; i++) {
        snprintf(buffer, sizeof(buffer), "%s: %s", data.fields[i], data.values[i]);
        HPDF_Page_TextOut(page, 50, y, buffer);
        y -= 20;
    }
    HPDF_Page_EndText(page);
    
    char outfile[150];
    snprintf(outfile, sizeof(outfile), DATA_DIR "%s_medical_report.pdf", patientID);
    // Ensure data directory exists
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    HPDF_SaveToFile(pdf, outfile);
    HPDF_Free(pdf);
    
    logEvent(patientID, "Medical PDF exported");
    printf("Medical report PDF exported to %s\n", outfile);
}

void exportHostelPDF(const char *residentID) {
    struct {
        int count;
        char fields[MAX_SUBJECTS][MAX_LEN];
        char values[MAX_SUBJECTS][MAX_LEN];
    } data;
    size_t size = sizeof(data);
    
    if (!loadUserData(residentID, "HOSTEL_DATA", &data, &size)) {
        printf("No hostel data found for PDF export\n");
        return;
    }
    int count = data.count;
    
    Profile p = {0};
    if (!getUserByID(residentID, &p)) {
        printf("Cannot load profile for PDF\n");
        return;
    }
    
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Failed to create PDF\n");
        return;
    }
    
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);
    
    float y = 800;
    char buffer[256];
    
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 200, y, "Hostel Accommodation Report");
    y -= 30;
    
    snprintf(buffer, sizeof(buffer), "Resident: %s", p.name);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Hostel: %s", p.instituteName);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Department: %s", p.department);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 30;
    
    HPDF_Page_TextOut(page, 50, y, "Accommodation Details:");
    y -= 20;
    
    for (int i = 0; i < count; i++) {
        snprintf(buffer, sizeof(buffer), "%s: %s", data.fields[i], data.values[i]);
        HPDF_Page_TextOut(page, 50, y, buffer);
        y -= 20;
    }
    HPDF_Page_EndText(page);
    
    char outfile[150];
    snprintf(outfile, sizeof(outfile), DATA_DIR "%s_hostel_report.pdf", residentID);
    // Ensure data directory exists
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    HPDF_SaveToFile(pdf, outfile);
    HPDF_Free(pdf);
    
    logEvent(residentID, "Hostel PDF exported");
    printf("Hostel report PDF exported to %s\n", outfile);
}

int exportProfilePDF(const char *userID, const char *filename) {
    Profile p = {0};
    if (!getUserByID(userID, &p)) {
        printf("[ERROR] Cannot load profile for PDF export (userID: %s)\n", userID);
        return 1;
    }
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("[ERROR] Failed to create PDF\n");
        return 1;
    }
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);
    float y = 800;
    char buffer[256];
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 200, y, "Profile Export");
    y -= 30;
    snprintf(buffer, sizeof(buffer), "Name: %s", p.name);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    snprintf(buffer, sizeof(buffer), "%s: %s", getCampusName(p.campusType), p.instituteName);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    snprintf(buffer, sizeof(buffer), "Department: %s", p.department);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    snprintf(buffer, sizeof(buffer), "Email: %s", p.email);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    snprintf(buffer, sizeof(buffer), "Mobile: %s", p.mobile);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    snprintf(buffer, sizeof(buffer), "User ID: %s", p.userID);
    HPDF_Page_TextOut(page, 50, y, buffer);
    HPDF_Page_EndText(page);
    // Ensure data directory exists (for custom filename paths under data/)
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    HPDF_SaveToFile(pdf, filename);
    HPDF_Free(pdf);
    printf("Profile PDF exported to %s\n", filename);
    return 1;
}

int exportProfileTXT(const char *userID, const char *filename) {
    Profile p = {0};
    if (!getUserByID(userID, &p)) {
        printf("[ERROR] Cannot load profile for text export (userID: %s)\n", userID);
        return 1;
    }
    // Ensure data directory exists
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("[ERROR] Cannot create text file: %s\n", filename);
        return 1;
    }
    fprintf(f, "=== PROFILE EXPORT ===\n\n");
    fprintf(f, "Name: %s\n", p.name);
    fprintf(f, "%s: %s\n", getCampusName(p.campusType), p.instituteName);
    fprintf(f, "Department: %s\n", p.department);
    fprintf(f, "Email: %s\n", p.email);
    fprintf(f, "Mobile: %s\n", p.mobile);
    fprintf(f, "User ID: %s\n", p.userID);
    fprintf(f, "Campus Type: %s\n", getCampusName(p.campusType));
    fclose(f);
    printf("Profile text file exported to %s\n", filename);
    return 1;
}

int exportProfileCSV(const char *userID, const char *filename) {
    Profile p = {0};
    if (!getUserByID(userID, &p)) {
        printf("[ERROR] Cannot load profile for CSV export (userID: %s)\n", userID);
        return 1;
    }
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("[ERROR] Cannot create CSV file: %s\n", filename);
        return 1;
    }
    fprintf(f, "Field,Value\n");
    fprintf(f, "Name,%s\n", p.name);
    fprintf(f, "%s,%s\n", getCampusName(p.campusType), p.instituteName);
    fprintf(f, "Department,%s\n", p.department);
    fprintf(f, "Email,%s\n", p.email);
    fprintf(f, "Mobile,%s\n", p.mobile);
    fprintf(f, "User ID,%s\n", p.userID);
    fprintf(f, "Campus Type,%s\n", getCampusName(p.campusType));
    fclose(f);
    printf("Profile CSV file exported to %s\n", filename);
    return 1;
}

void exportRecoveredProfilePDF(const Profile *p, const char *filename) {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Failed to create PDF\n");
        return;
    }
    
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);
    
    float y = 800;
    char buffer[256];
    
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 180, y, "Recovered Profile Details");
    y -= 30;
    
    snprintf(buffer, sizeof(buffer), "Name: %s", p->name);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "User ID: %s", p->userID);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "%s: %s", getCampusName(p->campusType), p->instituteName);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Department: %s", p->department);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Email: %s", p->email);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;
    
    snprintf(buffer, sizeof(buffer), "Mobile: %s", p->mobile);
    HPDF_Page_TextOut(page, 50, y, buffer);
    HPDF_Page_EndText(page);
    
    HPDF_SaveToFile(pdf, filename);
    HPDF_Free(pdf);
    
    printf("Recovered profile PDF exported to %s\n", filename);
}

void exportRecoveredProfileTXT(const Profile *p, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Cannot create text file\n");
        return;
    }
    
    fprintf(f, "=== RECOVERED PROFILE DETAILS ===\n\n");
    fprintf(f, "Name: %s\n", p->name);
    fprintf(f, "User ID: %s\n", p->userID);
    fprintf(f, "%s: %s\n", getCampusName(p->campusType), p->instituteName);
    fprintf(f, "Department: %s\n", p->department);
    fprintf(f, "Email: %s\n", p->email);
    fprintf(f, "Mobile: %s\n", p->mobile);
    fprintf(f, "Campus Type: %s\n", getCampusName(p->campusType));
    
    fclose(f);
    printf("Recovered profile text file exported to %s\n", filename);
}

void exportRecoveredProfileCSV(const Profile *p, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Cannot create CSV file\n");
        return;
    }
    
    fprintf(f, "Field,Value\n");
    fprintf(f, "Name,%s\n", p->name);
    fprintf(f, "User ID,%s\n", p->userID);
    fprintf(f, "%s,%s\n", getCampusName(p->campusType), p->instituteName);
    fprintf(f, "Department,%s\n", p->department);
    fprintf(f, "Email,%s\n", p->email);
    fprintf(f, "Mobile,%s\n", p->mobile);
    fprintf(f, "Campus Type,%s\n", getCampusName(p->campusType));
    
    fclose(f);
    printf("Recovered profile CSV file exported to %s\n", filename);
}
#else
// Fallback implementations when HPDF is disabled — generate plain text files
void ensure_data_dir(void) {
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
}

void exportSchoolPDF(const char *studentID) {
    ensure_data_dir();
    char out[256];
    snprintf(out, sizeof(out), "data/%s_school_report.txt", studentID);
    FILE *f = fopen(out, "w");
    if (!f) { printf("Cannot create report file: %s\n", out); return; }
    fprintf(f, "=== School Report (text fallback) ===\n\n");
    fprintf(f, "Student ID: %s\n", studentID);
    fprintf(f, "(Detailed per-subject data not available in fallback export)\n");
    fclose(f);
    printf("School report exported to %s\n", out);
}

void exportCollegePDF(const char *studentID) {
    ensure_data_dir();
    char out[256];
    snprintf(out, sizeof(out), "data/%s_college_transcript.txt", studentID);
    FILE *f = fopen(out, "w");
    if (!f) { printf("Cannot create report file: %s\n", out); return; }
    fprintf(f, "=== College Transcript (text fallback) ===\n\n");
    fprintf(f, "Student ID: %s\n", studentID);
    fprintf(f, "(Detailed transcript data not available in fallback export)\n");
    fclose(f);
    printf("College transcript exported to %s\n", out);
}

void exportHospitalPDF(const char *patientID) {
    ensure_data_dir();
    char out[256];
    snprintf(out, sizeof(out), "data/%s_medical_report.txt", patientID);
    FILE *f = fopen(out, "w");
    if (!f) { printf("Cannot create report file: %s\n", out); return; }
    fprintf(f, "=== Medical Report (text fallback) ===\n\n");
    fprintf(f, "Patient ID: %s\n", patientID);
    fprintf(f, "(Detailed medical data not available in fallback export)\n");
    fclose(f);
    printf("Medical report exported to %s\n", out);
}

void exportHostelPDF(const char *residentID) {
    ensure_data_dir();
    char out[256];
    snprintf(out, sizeof(out), "data/%s_hostel_report.txt", residentID);
    FILE *f = fopen(out, "w");
    if (!f) { printf("Cannot create report file: %s\n", out); return; }
    fprintf(f, "=== Hostel Report (text fallback) ===\n\n");
    fprintf(f, "Resident ID: %s\n", residentID);
    fprintf(f, "(Detailed accommodation data not available in fallback export)\n");
    fclose(f);
    printf("Hostel report exported to %s\n", out);
}

int exportProfilePDF(const char *userID, const char *filename) {
    // Write profile information to the requested filename as plain text
    Profile p = {0};
    if (!getUserByID(userID, &p)) {
        printf("[ERROR] Cannot load profile for export (userID: %s)\n", userID);
        return 0;
    }
    ensure_data_dir();
    FILE *f = fopen(filename, "w");
    if (!f) { printf("[ERROR] Cannot create file: %s\n", filename); return 0; }
    fprintf(f, "=== PROFILE EXPORT ===\n\n");
    fprintf(f, "Name: %s\n", p.name);
    fprintf(f, "%s: %s\n", getCampusName(p.campusType), p.instituteName);
    fprintf(f, "Department: %s\n", p.department);
    fprintf(f, "Email: %s\n", p.email);
    fprintf(f, "Mobile: %s\n", p.mobile);
    fprintf(f, "User ID: %s\n", p.userID);
    fprintf(f, "Campus Type: %s\n", getCampusName(p.campusType));
    fclose(f);
    printf("Profile exported to %s\n", filename);
    return 1;
}

// When HPDF is disabled, keep TXT/CSV functions (they already exist), but
// ensure fallback returns success when called directly here for safety.
int exportProfileTXT(const char *userID, const char *filename) {
    Profile p = {0};
    if (!getUserByID(userID, &p)) { printf("[ERROR] Cannot load profile\n"); return 0; }
    ensure_data_dir();
    FILE *f = fopen(filename, "w");
    if (!f) { printf("[ERROR] Cannot create file: %s\n", filename); return 0; }
    fprintf(f, "=== PROFILE EXPORT ===\n\n");
    fprintf(f, "Name: %s\n", p.name);
    fprintf(f, "%s: %s\n", getCampusName(p.campusType), p.instituteName);
    fprintf(f, "Department: %s\n", p.department);
    fprintf(f, "Email: %s\n", p.email);
    fprintf(f, "Mobile: %s\n", p.mobile);
    fprintf(f, "User ID: %s\n", p.userID);
    fprintf(f, "Campus Type: %s\n", getCampusName(p.campusType));
    fclose(f);
    printf("Profile text exported to %s\n", filename);
    return 1;
}

int exportProfileCSV(const char *userID, const char *filename) {
    Profile p = {0};
    if (!getUserByID(userID, &p)) { printf("[ERROR] Cannot load profile\n"); return 0; }
    ensure_data_dir();
    FILE *f = fopen(filename, "w");
    if (!f) { printf("[ERROR] Cannot create file: %s\n", filename); return 0; }
    fprintf(f, "Field,Value\n");
    fprintf(f, "Name,%s\n", p.name);
    fprintf(f, "%s,%s\n", getCampusName(p.campusType), p.instituteName);
    fprintf(f, "Department,%s\n", p.department);
    fprintf(f, "Email,%s\n", p.email);
    fprintf(f, "Mobile,%s\n", p.mobile);
    fprintf(f, "User ID,%s\n", p.userID);
    fprintf(f, "Campus Type,%s\n", getCampusName(p.campusType));
    fclose(f);
    printf("Profile CSV exported to %s\n", filename);
    return 1;
}

#endif // HPDF_DISABLED
