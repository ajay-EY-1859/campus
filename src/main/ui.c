#include <stdio.h>
#include <stdlib.h>
#include "../include/auth.h"
#include "../include/student.h"
#include "../include/config.h"
#include "../include/ui.h"
#include "../include/fileio.h"
#include "../include/database.h"

void dashboard(const char *userID) {
    Profile p = {0};
    
    if (!getUserByID(userID, &p)) {
        printf("Error: Cannot load user profile for ID: %s\n", userID ? userID : "UNKNOWN");
        printf("Please contact support or try registering again.\n");
        return;
    }
    
    int choice = 0;
    while (1) {
        printf("\n%s Dashboard\n", getCampusName(p.campusType));
        printf("1. View Profile\n");
        printf("2. Edit Profile\n");
        printf("3. Add Data\n");
        printf("4. View Data\n");
        printf("5. Change Password\n");
        printf("6. Export Report\n");
        printf("7. Export Profile\n");
        printf("8. Logout\n");
        printf("9. Exit Application\n");
        while (1) {
            printf("Select option: ");
            if (safeGetInt(&choice, 1, 9) == SUCCESS) {
                break;
            } else {
                printf("Invalid input\n");
            }
        }

        switch (choice) {
            case 1: viewProfile(userID); break;
            case 2: editProfile(userID); break;
            case 3:
                switch(p.campusType) {
                    case CAMPUS_SCHOOL: saveSchoolData(userID); break;
                    case CAMPUS_COLLEGE: saveCollegeData(userID); break;
                    case CAMPUS_HOSPITAL: saveHospitalData(userID); break;
                    case CAMPUS_HOSTEL: saveHostelData(userID); break;
                }
                break;
            case 4:
                switch(p.campusType) {
                    case CAMPUS_SCHOOL: loadSchoolData(userID); break;
                    case CAMPUS_COLLEGE: loadCollegeData(userID); break;
                    case CAMPUS_HOSPITAL: loadHospitalData(userID); break;
                    case CAMPUS_HOSTEL: loadHostelData(userID); break;
                }
                break;
            case 5: changePassword(userID); break;
            case 6:
                switch(p.campusType) {
                    case CAMPUS_SCHOOL: exportSchoolPDF(userID); break;
                    case CAMPUS_COLLEGE: exportCollegePDF(userID); break;
                    case CAMPUS_HOSPITAL: exportHospitalPDF(userID); break;
                    case CAMPUS_HOSTEL: exportHostelPDF(userID); break;
                }
                break;
            case 7: exportProfile(userID); break;
            case 8:
                printf("Logged out\n");
                return;
            case 9:
                printf("Goodbye! Thanks for using %s.\n", APP_NAME);
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
}
