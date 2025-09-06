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

const char* getCampusName(CampusType type) {
    switch(type) {
        case CAMPUS_SCHOOL: return "School";
        case CAMPUS_COLLEGE: return "College";
        case CAMPUS_HOSPITAL: return "Hospital";
        case CAMPUS_HOSTEL: return "Hostel";
        default: return "Unknown";
    }
}

const char* getGrade(double percent) {
    if (percent >= 90.0) return "A+";
    else if (percent >= 80.0) return "A";
    else if (percent >= 70.0) return "B";
    else if (percent >= 60.0) return "C";
    else if (percent >= 50.0) return "D";
    else return "F";
}

void printSummary(int total, int full, CampusType type) {
    double percentage = (full == 0) ? 0.0 : (total * 100.0 / full);
    printf("Total: %d / %d\n", total, full);
    printf("Percentage: %.2f%%\n", percentage);
    if (type == CAMPUS_SCHOOL || type == CAMPUS_COLLEGE) {
        printf("Grade: %s\n", getGrade(percentage));
    }
}



// School Data Management
void saveSchoolData(const char *studentID) {
    Profile p = {0};
    if (!readProfile(&p, studentID)) {
        printf("Cannot load profile\n");
        return;
    }
    
    int marks[MAX_SUBJECTS] = {0}, fullMarks[MAX_SUBJECTS] = {0};
    printf("\nEnter marks for subjects:\n");
    for (int i = 0; i < p.dataCount; i++) {
        printf("%s - Marks: ", p.dataFields[i]);
        scanf("%d", &marks[i]);
        printf("Full marks: ");
        scanf("%d", &fullMarks[i]);
    }
    
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", studentID);
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
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
    
    int count, marks[MAX_SUBJECTS], fullMarks[MAX_SUBJECTS];
    char subjects[MAX_SUBJECTS][MAX_LEN];
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
    if (!readProfile(&p, studentID)) {
        printf("Cannot load profile\n");
        return;
    }
    
    int marks[MAX_SUBJECTS] = {0}, credits[MAX_SUBJECTS] = {0};
    printf("\nEnter semester marks:\n");
    for (int i = 0; i < p.dataCount; i++) {
        printf("%s - Marks: ", p.dataFields[i]);
        scanf("%d", &marks[i]);
        printf("Credits: ");
        scanf("%d", &credits[i]);
    }
    
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", studentID);
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
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
    
    int count, marks[MAX_SUBJECTS], credits[MAX_SUBJECTS];
    char subjects[MAX_SUBJECTS][MAX_LEN];
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
    
    double cgpa = totalCredits > 0 ? (double)totalMarks / (totalCredits * 10.0) : 0.0;
    printf("CGPA: %.2f\n", cgpa);
}

// Hospital Data Management
void saveHospitalData(const char *patientID) {
    Profile p = {0};
    if (!readProfile(&p, patientID)) {
        printf("Cannot load profile\n");
        return;
    }
    
    char values[MAX_SUBJECTS][MAX_LEN];
    printf("\nEnter medical data:\n");
    for (int i = 0; i < p.dataCount; i++) {
        printf("%s: ", p.dataFields[i]);
        scanf(" %99[^\n]", values[i]);
    }
    
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", patientID);
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
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
    
    int count;
    char fields[MAX_SUBJECTS][MAX_LEN], values[MAX_SUBJECTS][MAX_LEN];
    
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
    if (!readProfile(&p, residentID)) {
        printf("Cannot load profile\n");
        return;
    }
    
    char values[MAX_SUBJECTS][MAX_LEN];
    printf("\nEnter hostel data:\n");
    for (int i = 0; i < p.dataCount; i++) {
        printf("%s: ", p.dataFields[i]);
        scanf(" %99[^\n]", values[i]);
    }
    
    char datafile[150];
    snprintf(datafile, sizeof(datafile), DATA_DIR "%s.data", residentID);
    
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0777);
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
    
    int count;
    char fields[MAX_SUBJECTS][MAX_LEN], values[MAX_SUBJECTS][MAX_LEN];
    
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
    if (!readProfile(&p, studentID)) {
        printf("Cannot load profile for PDF\n");
        return;
    }
    
    double percentage = (totalFull == 0) ? 0.0 : (total * 100.0 / totalFull);
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
    if (!readProfile(&p, studentID)) {
        printf("Cannot load profile for PDF\n");
        return;
    }
    
    double cgpa = totalCredits > 0 ? (double)totalMarks / (totalCredits * 10.0) : 0.0;
    
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
    if (!readProfile(&p, patientID)) {
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
    if (!readProfile(&p, residentID)) {
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
    HPDF_SaveToFile(pdf, outfile);
    HPDF_Free(pdf);
    
    logEvent(residentID, "Hostel PDF exported");
    printf("Hostel report PDF exported to %s\n", outfile);
}