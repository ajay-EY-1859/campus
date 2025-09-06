#include <stdio.h>
#include "auth.h"
#include "config.h"
#include "student.h"
#include "utils.h"
#include "ui.h"
#include "fileio.h"
#include "database.h"
#include "security.h"
#include "hpdf/hpdf.h"

void displayMainMenu() {
    printf("\n%s (%s)\n", APP_NAME, APP_VERSION);
    printf("1. Register New User\n");
    printf("2. Login\n");
    printf("3. Exit\n");
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
        if (safeGetInt(&choice, 1, 3) != SUCCESS) {
            printf("Invalid input. Please enter 1, 2, or 3.\n");
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
                printf("Goodbye! Thanks for using %s.\n", APP_NAME);
                closeDatabase();
                return SUCCESS;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return SUCCESS;
}
