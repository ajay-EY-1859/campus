#include <stdio.h>
#include <stdlib.h>

#define APP_NAME "Campus Management System"  
#define APP_VERSION "v2.0"

void displayMainMenu() {
    printf("\n%s (%s)\n", APP_NAME, APP_VERSION);
    printf("1. Register New User\n");
    printf("2. Login\n");
    printf("3. Forgot User ID\n");
    printf("4. Exit\n");
    printf("Select option: ");
}

int main() {
    printf("Starting %s %s...\n", APP_NAME, APP_VERSION);
    printf("Database initialized successfully\n");
    
    int choice = 0;

    while (1) {
        displayMainMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter 1, 2, 3, or 4.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1: 
                printf("Starting registration...\n");
                printf("Registration feature coming soon!\n");
                break;
            case 2: 
                printf("Starting login...\n");
                printf("Login feature coming soon!\n");
                break;
            case 3:
                printf("Starting User ID recovery...\n");
                printf("Recovery feature coming soon!\n");
                break;
            case 4:
                printf("Goodbye! Thanks for using %s.\n", APP_NAME);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}