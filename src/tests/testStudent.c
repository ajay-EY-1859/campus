#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/student.h"

// Mock Profile struct and constants for testing
#define TEST_MAX_SUBJECTS 5
#define TEST_MAX_LEN 32

typedef struct {
    char subjects[TEST_MAX_SUBJECTS][TEST_MAX_LEN];
    int subjectCount;
} Profile;

void test_getGrade() {
    assert(strcmp(getGrade(95.0f), "A+ 🌟") == 0);
    assert(strcmp(getGrade(85.0f), "A") == 0);
    assert(strcmp(getGrade(75.0f), "B") == 0);
    assert(strcmp(getGrade(65.0f), "C") == 0);
    assert(strcmp(getGrade(55.0f), "D") == 0);
    assert(strcmp(getGrade(45.0f), "F ❌") == 0);
    printf("test_getGrade passed.\n");
}

void test_printStudentSummary() {
    // Just check that it runs without crashing
    printStudentSummary(80, 100);
    printStudentSummary(0, 0);
    printf("test_printStudentSummary passed.\n");
}

void test_save_and_loadSubjectData() {
    // Prepare a mock profile file
    Profile p;
    strcpy(p.subjects[0], "Math");
    strcpy(p.subjects[1], "Physics");
    p.subjectCount = 2;

    char studentID[] = "ST01";
    char ext[6];
    ext[0] = tolower(studentID[0]);
    ext[1] = tolower(studentID[1]);
    strcpy(&ext[2], "pfx");
    char profFile[150];
    sprintf(profFile, "credentials/%s.%s", studentID, ext);

    FILE *fp = fopen(profFile, "wb");
    assert(fp);
    fwrite(&p, sizeof(Profile), 1, fp);
    fclose(fp);

    // Simulate user input for marks
    FILE *input = fopen("test_input.txt", "w");
    fprintf(input, "90\n100\n80\n100\n");
    fclose(input);
    freopen("test_input.txt", "r", stdin);

    saveSubjectData(studentID);

    // Now test loading
    loadSubjectData(studentID);

    // Clean up
    remove(profFile);
    char datafile[150];
    sprintf(datafile, "data/%s.ajstu", studentID);
    remove(datafile);
    remove("test_input.txt");
    printf("test_save_and_loadSubjectData passed.\n");
}

int main() {
    test_getGrade();
    test_printStudentSummary();
    test_save_and_loadSubjectData();
    printf("All tests passed.\n");
    return 0;
}