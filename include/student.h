#ifndef STUDENT_H
#define STUDENT_H

#include "config.h"   // for MAX_SUBJECTS, MAX_LEN

// Academic module actions
void saveSubjectData(const char *studentID);
void loadSubjectData(const char *studentID);
void exportResultPDF(const char *studentID);

// Utilities
const char* getGrade(float percent);
void printStudentSummary(int total, int full);

#endif
