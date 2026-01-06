#include <stdio.h>
#include "auth.h"
#include "config.h"
#include "student.h"
#include "utils.h"
#include "ui.h"
#include "fileio.h"
#include "database.h"
#include "campus_security.h"
#include "hpdf/hpdf.h"

// Function declarations
ErrorCode recoverUserID(void);

void displayMainMenu() {
    printf("\n%s (%s)\n", APP_NAME, APP_VERSION);
    printf("1. Register New User\n");
    printf("2. Login\n");
    printf("3. Forgot User ID\n");
    printf("4. Exit\n");
    printf("Select option: ");
}

int main() {
    // Initialize database
    if (initDatabase() != SUCCESS) {
        printf("Failed to initialize database. Exiting.\n");
        return ERROR_DATABASE;
    }
    
    int choice = 0;

    while (1) {
        displayMainMenu();
        if (safeGetInt(&choice, 1, 4) != SUCCESS) {
            printf("Invalid input. Please enter 1, 2, 3, or 4.\n");
            continue;
        }

        switch (choice) {
            case 1: 
                printf("Starting registration...\n");
                signup(); 
                break;
            case 2: 
                printf("Starting login...\n");
                signin(); 
                break;
            case 3:
                printf("Starting User ID recovery...\n");
                recoverUserID();
                break;
            case 4:
                printf("Goodbye! Thanks for using %s.\n", APP_NAME);
                closeDatabase();
                return SUCCESS;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return SUCCESS;
}
