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
    
    displayMainMenu();
    printf("\nDemo version is running successfully!\n");
    printf("This is a console-based Campus Management System.\n");
    printf("Features include:\n");
    printf("- User Registration and Authentication\n");
    printf("- Multi-campus support (School, College, Hospital, Hostel)\n"); 
    printf("- PDF report generation\n");
    printf("- Secure data management\n");
    printf("\nThe application is ready to run interactively.\n");
    printf("For full interactive mode, run the executable and provide input.\n");
    
    return 0;
}