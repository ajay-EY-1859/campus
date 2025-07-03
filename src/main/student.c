#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/student.h"
#include "../include/auth.h"
#include "../include/config.h"
#include "../include/fileio.h"
#include "../include/utils.h"
#include "../include/hpdf.h"
#include"../include/ui.h"
// 🏅 Grade classifier
const char* getGrade(float percent) {
    if (percent >= 90.0) return "A+ 🌟";
    else if (percent >= 80.0) return "A";
    else if (percent >= 70.0) return "B";
    else if (percent >= 60.0) return "C";
    else if (percent >= 50.0) return "D";
    else return "F ❌";
}

// 📊 Summary print
void printStudentSummary(int totalMarks, int totalFull) {
    float percentage = (totalFull == 0) ? 0 : (totalMarks * 100.0f / totalFull);
    printf("-------------------------------\n");
    printf("📊 Total: %d / %d\n", totalMarks, totalFull);
    printf("📈 Percentage: %.2f%%\n", percentage);
    printf("🏅 Grade: %s\n", getGrade(percentage));
}

// 📥 Save subject marks
void saveSubjectData(const char *studentID) {
    Profile p;
    if (!readProfile(&p, studentID)) {
        printf("❌ Cannot load profile for %s\n", studentID);
        return;
    }

    int marks[MAX_SUBJECTS], fullMarks[MAX_SUBJECTS];

    printf("\n📊 Enter marks for each subject:\n");
    for (int i = 0; i < p.subjectCount; i++) {
        printf("  %s → Marks obtained: ");
        scanf("%d", &marks[i]);
        printf("     → Full marks: ");
        scanf("%d", &fullMarks[i]);
    }

    char datafile[150];
    sprintf(datafile, DATA_DIR "%s.ajstu", studentID);
    FILE *df = fopen(datafile, "wb");
    if (!df) {
        logEvent(studentID, "❌ Failed to save subject data");
        return;
    }

    fwrite(&p.subjectCount, sizeof(int), 1, df);
    for (int i = 0; i < p.subjectCount; i++) {
        fwrite(&p.subjects[i], sizeof(p.subjects[i]), 1, df);
        fwrite(&marks[i], sizeof(int), 1, df);
        fwrite(&fullMarks[i], sizeof(int), 1, df);
    }
    fclose(df);

    logEvent(studentID, "✅ Subject marks saved");
    printf("✅ Subject marks saved to %s\n", datafile);
}

// 📤 Load and view subject marks
void loadSubjectData(const char *studentID) {
    int subjectCount;
    char subjects[MAX_SUBJECTS][MAX_LEN];
    int marks[MAX_SUBJECTS], fullMarks[MAX_SUBJECTS];
    int totalMarks = 0, totalFull = 0;

    char datafile[150];
    sprintf(datafile, DATA_DIR "%s.ajstu", studentID);
    FILE *fp = fopen(datafile, "rb");
    if (!fp) {
        printf("❌ No subject data found for %s\n", studentID);
        return;
    }

    fread(&subjectCount, sizeof(int), 1, fp);
    for (int i = 0; i < subjectCount; i++) {
        fread(&subjects[i], sizeof(subjects[i]), 1, fp);
        fread(&marks[i], sizeof(int), 1, fp);
        fread(&fullMarks[i], sizeof(int), 1, fp);
        printf("  %d. %s → %d / %d\n", i + 1, subjects[i], marks[i], fullMarks[i]);
        totalMarks += marks[i];
        totalFull += fullMarks[i];
    }
    fclose(fp);

    printStudentSummary(totalMarks, totalFull);
}

// 🧾 Export PDF using libharu
void exportResultPDF(const char *studentID) {
    int subjectCount;
    char subjects[MAX_SUBJECTS][MAX_LEN];
    int marks[MAX_SUBJECTS], fullMarks[MAX_SUBJECTS];
    int totalMarks = 0, totalFull = 0;

    char infile[150];
    sprintf(infile, DATA_DIR "%s.ajstu", studentID);
    FILE *fp = fopen(infile, "rb");
    if (!fp) {
        printf("❌ No subject data found.\n");
        return;
    }

    fread(&subjectCount, sizeof(int), 1, fp);
    for (int i = 0; i < subjectCount; i++) {
        fread(&subjects[i], sizeof(subjects[i]), 1, fp);
        fread(&marks[i], sizeof(int), 1, fp);
        fread(&fullMarks[i], sizeof(int), 1, fp);
        totalMarks += marks[i];
        totalFull += fullMarks[i];
    }
    fclose(fp);

    float percentage = (totalFull == 0) ? 0 : (totalMarks * 100.0f / totalFull);
    const char *grade = getGrade(percentage);

    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("❌ Failed to create PDF document.\n");
        return;
    }

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);

    float y = 800;
    char buffer[128];

    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 200, y, "AjCampus – Student Result");
    y -= 30;

    HPDF_Page_TextOut(page, 50, y, "Subject        Marks   Full Marks");
    y -= 20;

    for (int i = 0; i < subjectCount; i++) {
        snprintf(buffer, sizeof(buffer), "%-15s %5d   %5d", subjects[i], marks[i], fullMarks[i]);
        HPDF_Page_TextOut(page, 50, y, buffer);
        y -= 20;
    }

    y -= 10;
    snprintf(buffer, sizeof(buffer), "Total: %d / %d", totalMarks, totalFull);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;

    snprintf(buffer, sizeof(buffer), "Percentage: %.2f%%", percentage);
    HPDF_Page_TextOut(page, 50, y, buffer);
    y -= 20;

    snprintf(buffer, sizeof(buffer), "Grade: %s", grade);
    HPDF_Page_TextOut(page, 50, y, buffer);
    HPDF_Page_EndText(page);

    char outfile[150];
    sprintf(outfile, DATA_DIR "%s_result.pdf", studentID);
    HPDF_SaveToFile(pdf, outfile);
    HPDF_Free(pdf);

    logEvent(studentID, "📤 Exported result PDF");
    printf("📤 Result PDF exported to %s\n", outfile);
}
