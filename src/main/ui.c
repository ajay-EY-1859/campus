#include <stdio.h>
#include "../include/auth.h"
#include "../include/student.h"
#include "../include/config.h"  // 🧩 Now added for centralized constants
#include"../include/ui.h"
#include"../include/fileio.h"
 dashboard(const char *studentID) {
    int choice;
    while (1) {
        printf("\n===== 🎓 AjCampus – Student Dashboard =====\n");
        printf("1. View Profile\n");
        printf("2. Edit Profile\n");
        printf("3. Add Subject Marks\n");
        printf("4. View Subject Performance\n");
        printf("5. Change Password\n");
        printf("6. Export Result (PDF)\n");
        printf("7. Logout\n");
        printf("Select an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewProfile(studentID); break;
            case 2: editProfile(studentID); break;
            case 3: saveSubjectData(studentID); break;
            case 4: loadSubjectData(studentID); break;        
            case 5: changePassword(studentID); break;
            case 6: exportResultPDF(studentID); break;
            case 7: {
                char confirm;
                printf("⚠️  Are you sure you want to logout? (y/n): ");
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y') {
                    printf("👋 Logged out successfully.\n");
                    return;
                } else {
                    printf("🔁 Staying logged in.\n");
                }
                break;
            }
            default:
                printf("❌ Invalid choice. Try again.\n");
        }
    }
}
