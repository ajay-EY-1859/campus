#include <stdio.h>
#include "../include/auth.h"
#include "../include/config.h"
#include"../include/student.h"
#include"../include/utils.h"
#include"../include/ui.h"
#include"../include/fileio.h"
#include"../include/hpdf.h"

void displayMainMenu() {
    printf("\n===== 📚 %s Portal (%s) =====\n", APP_NAME, APP_VERSION);
    printf("1. Signup\n");
    printf("2. Signin\n");
    printf("3. Exit\n");
    printf("Select an option: ");
}

int main() {
    int choice;

    while (1) {
        displayMainMenu();
        if (scanf("%d", &choice) != 1) {
            printf("❌ Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        switch (choice) {
            case 1: signup(); break;
            case 2: signin(); break;
            case 3:
                printf("👋 Goodbye! Thanks for using %s.\n", APP_NAME);
                return 0;
            default:
                printf("❌ Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
