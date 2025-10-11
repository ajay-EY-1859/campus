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
        // Loop until valid marks are entered
        while (1) {
            printf("%s - Marks: ", p.dataFields[i]);
            if (safeGetInt(&marks[i], 0, 100) == SUCCESS) {
                break;
            } else {
                printf("Invalid marks entered. Please enter a value between 0 and 100.\n");
            }
        }
        // Loop until valid full marks are entered
        while (1) {
            printf("Full marks: ");
            if (safeGetInt(&fullMarks[i], 1, 100) == SUCCESS) {
                break;
            } else {
                printf("Invalid full marks entered. Please enter a value between 1 and 100.\n");
            }
        }
    }
    
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", studentID);
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    
    FILE *f = fopen(datafile, "wb");
    if (f) {
        fwrite(&p.dataCount, sizeof(int), 1, f);
        for (int i = 0; i < p.dataCount; i++) {
            fwrite(&p.dataFields[i], sizeof(p.dataFields[i]), 1, f);
            fwrite(&marks[i], sizeof(int), 1, f);
            fwrite(&fullMarks[i], sizeof(int), 1, f);
        }
        fclose(f);
        printf("School marks saved\n");
    }
}

void loadSchoolData(const char *studentID) {
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", studentID);
    FILE *f = fopen(datafile, "rb");
    if (!f) {
        printf("No school data found\n");
        return;
    }
    
    int count = 0, marks[MAX_SUBJECTS] = {0}, fullMarks[MAX_SUBJECTS] = {0};
    char subjects[MAX_SUBJECTS][MAX_LEN] = {0};
    int total = 0, totalFull = 0;
    
    fread(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) {
        fread(&subjects[i], sizeof(subjects[i]), 1, f);
        fread(&marks[i], sizeof(int), 1, f);
        fread(&fullMarks[i], sizeof(int), 1, f);
        printf("%d. %s - %d/%d\n", i+1, subjects[i], marks[i], fullMarks[i]);
        total += marks[i];
        totalFull += fullMarks[i];
    }
    fclose(f);
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
        // Loop until valid marks are entered
        while (1) {
            printf("%s - Marks: ", p.dataFields[i]);
            if (safeGetInt(&marks[i], 0, 100) == SUCCESS) {
                break;
            } else {
                printf("Invalid marks entered. Please enter a value between 0 and 100.\n");
            }
        }
        // Loop until valid credits are entered
        while (1) {
            printf("Credits: ");
            if (safeGetInt(&credits[i], 1, 10) == SUCCESS) {
                break;
            } else {
                printf("Invalid credits entered. Please enter a value between 1 and 10.\n");
            }
        }
    }
    
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", studentID);
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    
    FILE *f = fopen(datafile, "wb");
    if (f) {
        fwrite(&p.dataCount, sizeof(int), 1, f);
        for (int i = 0; i < p.dataCount; i++) {
            fwrite(&p.dataFields[i], sizeof(p.dataFields[i]), 1, f);
            fwrite(&marks[i], sizeof(int), 1, f);
            fwrite(&credits[i], sizeof(int), 1, f);
        }
        fclose(f);
        printf("College marks saved\n");
    }
}

void loadCollegeData(const char *studentID) {
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", studentID);
    FILE *f = fopen(datafile, "rb");
    if (!f) {
        printf("No college data found\n");
        return;
    }
    
    int count = 0, marks[MAX_SUBJECTS] = {0}, credits[MAX_SUBJECTS] = {0};
    char subjects[MAX_SUBJECTS][MAX_LEN] = {0};
    int totalMarks = 0, totalCredits = 0;
    
    fread(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) {
        fread(&subjects[i], sizeof(subjects[i]), 1, f);
        fread(&marks[i], sizeof(int), 1, f);
        fread(&credits[i], sizeof(int), 1, f);
        printf("%d. %s - %d marks (%d credits)\n", i+1, subjects[i], marks[i], credits[i]);
        totalMarks += marks[i] * credits[i];
        totalCredits += credits[i];
    }
    fclose(f);
    
    float cgpa = totalCredits > 0 ? (float)totalMarks / (totalCredits * 10.0f) : 0.0f;
    printf("CGPA: %.2f\n", cgpa);
}

// Hospital Data Management
void saveHospitalData(const char *patientID) {
    Profile p = {0};
    if (!getUserByID(patientID, &p)) {
        printf("Cannot load profile\n");
        return;
    }
    
    char values[MAX_SUBJECTS][MAX_LEN] = {0};
    printf("\nEnter medical data:\n");
    for (int i = 0; i < p.dataCount; i++) {
        while (1) {
            printf("%s: ", p.dataFields[i]);
            if (safeGetString(values[i], sizeof(values[i])) == SUCCESS && strlen(values[i]) > 0) {
                break;
            } else {
                printf("Invalid input entered. Please try again.\n");
            }
        }
    }
    
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", patientID);
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    
    FILE *f = fopen(datafile, "wb");
    if (f) {
        fwrite(&p.dataCount, sizeof(int), 1, f);
        for (int i = 0; i < p.dataCount; i++) {
            fwrite(&p.dataFields[i], sizeof(p.dataFields[i]), 1, f);
            fwrite(&values[i], sizeof(values[i]), 1, f);
        }
        fclose(f);
        printf("Medical data saved\n");
    }
}

void loadHospitalData(const char *patientID) {
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", patientID);
    FILE *f = fopen(datafile, "rb");
    if (!f) {
        printf("No medical data found\n");
        return;
    }
    
    int count = 0;
    char fields[MAX_SUBJECTS][MAX_LEN] = {0}, values[MAX_SUBJECTS][MAX_LEN] = {0};
    
    fread(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) {
        fread(&fields[i], sizeof(fields[i]), 1, f);
        fread(&values[i], sizeof(values[i]), 1, f);
        printf("%s: %s\n", fields[i], values[i]);
    }
    fclose(f);
}

// Hostel Data Management
void saveHostelData(const char *residentID) {
    Profile p = {0};
    if (!getUserByID(residentID, &p)) {
        printf("Cannot load profile\n");
        return;
    }
    
    char values[MAX_SUBJECTS][MAX_LEN] = {0};
    printf("\nEnter hostel data:\n");
    for (int i = 0; i < p.dataCount; i++) {
        while (1) {
            printf("%s: ", p.dataFields[i]);
            if (safeGetString(values[i], sizeof(values[i])) == SUCCESS && strlen(values[i]) > 0) {
                break;
            } else {
                printf("Invalid input entered. Please try again.\n");
            }
        }
    }
    
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", residentID);
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0700);
#endif
    
    FILE *f = fopen(datafile, "wb");
    if (f) {
        fwrite(&p.dataCount, sizeof(int), 1, f);
        for (int i = 0; i < p.dataCount; i++) {
            fwrite(&p.dataFields[i], sizeof(p.dataFields[i]), 1, f);
            fwrite(&values[i], sizeof(values[i]), 1, f);
        }
        fclose(f);
        printf("Hostel data saved\n");
    }
}

void loadHostelData(const char *residentID) {
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", residentID);
    FILE *f = fopen(datafile, "rb");
    if (!f) {
        printf("No hostel data found\n");
        return;
    }
    
    int count = 0;
    char fields[MAX_SUBJECTS][MAX_LEN] = {0}, values[MAX_SUBJECTS][MAX_LEN] = {0};
    
    fread(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) {
        fread(&fields[i], sizeof(fields[i]), 1, f);
        fread(&values[i], sizeof(values[i]), 1, f);
        printf("%s: %s\n", fields[i], values[i]);
    }
    fclose(f);
}

// PDF Export Functions
void exportSchoolPDF(const char *studentID) {
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", studentID);
    FILE *f = fopen(datafile, "rb");
    if (!f) {
        printf("No school data found for PDF export\n");
        return;
    }
    
    int count, marks[MAX_SUBJECTS], fullMarks[MAX_SUBJECTS];
    char subjects[MAX_SUBJECTS][MAX_LEN];
    int total = 0, totalFull = 0;
    
    fread(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) {
        fread(&subjects[i], sizeof(subjects[i]), 1, f);
        fread(&marks[i], sizeof(int), 1, f);
        fread(&fullMarks[i], sizeof(int), 1, f);
        total += marks[i];
        totalFull += fullMarks[i];
    }
    fclose(f);
    
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
        snprintf(buffer, sizeof(buffer), "%-20s %5d   %5d", subjects[i], marks[i], fullMarks[i]);
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
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", studentID);
    FILE *f = fopen(datafile, "rb");
    if (!f) {
        printf("No college data found for PDF export\n");
        return;
    }
    
    int count, marks[MAX_SUBJECTS], credits[MAX_SUBJECTS];
    char subjects[MAX_SUBJECTS][MAX_LEN];
    int totalMarks = 0, totalCredits = 0;
    
    fread(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) {
        fread(&subjects[i], sizeof(subjects[i]), 1, f);
        fread(&marks[i], sizeof(int), 1, f);
        fread(&credits[i], sizeof(int), 1, f);
        totalMarks += marks[i] * credits[i];
        totalCredits += credits[i];
    }
    fclose(f);
    
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
        snprintf(buffer, sizeof(buffer), "%-20s %5d   %5d", subjects[i], marks[i], credits[i]);
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
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", patientID);
    FILE *f = fopen(datafile, "rb");
    if (!f) {
        printf("No medical data found for PDF export\n");
        return;
    }
    
    int count;
    char fields[MAX_SUBJECTS][MAX_LEN], values[MAX_SUBJECTS][MAX_LEN];
    
    fread(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) {
        fread(&fields[i], sizeof(fields[i]), 1, f);
        fread(&values[i], sizeof(values[i]), 1, f);
    }
    fclose(f);
    
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
        snprintf(buffer, sizeof(buffer), "%s: %s", fields[i], values[i]);
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
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", residentID);
    FILE *f = fopen(datafile, "rb");
    if (!f) {
        printf("No hostel data found for PDF export\n");
        return;
    }
    
    int count;
    char fields[MAX_SUBJECTS][MAX_LEN], values[MAX_SUBJECTS][MAX_LEN];
    
    fread(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) {
        fread(&fields[i], sizeof(fields[i]), 1, f);
        fread(&values[i], sizeof(values[i]), 1, f);
    }
    fclose(f);
    
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
        snprintf(buffer, sizeof(buffer), "%s: %s", fields[i], values[i]);
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
