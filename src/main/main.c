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
    if (!initDatabase()) {
        printf("Failed to initialize database. Exiting.\n");
        return 1;
    }
    
    int choice = 0;

    while (1) {
        displayMainMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        switch (choice) {
            case 1: 
                signup(); 
                break;
            case 2: 
                signin(); 
                break;
            case 3:
                printf("Goodbye! Thanks for using %s.\n", APP_NAME);
                closeDatabase();
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
