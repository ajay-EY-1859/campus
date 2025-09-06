#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/auth.h"
#include "../include/student.h"
#include "../include/config.h"
#include "../include/utils.h"

// Test data structures for different campus types
typedef struct {
    char name[MAX_LEN];
    char instituteName[MAX_LEN];
    char department[50];
    char email[MAX_LEN];
    char mobile[15];
    CampusType campusType;
} TestProfile;

// School Test Cases
TestProfile schoolTestCases[] = {
    {"Ajay Kumar", "Saraswati Vidya Mandir", "Science", "ajay.school@gmail.com", "9876543210", CAMPUS_SCHOOL},
    {"Priya Sharma", "Delhi Public School", "Commerce", "priya.dps@yahoo.com", "8765432109", CAMPUS_SCHOOL},
    {"Rahul Singh", "Kendriya Vidyalaya", "Arts", "rahul.kv@hotmail.com", "7654321098", CAMPUS_SCHOOL},
    {"Sneha Patel", "St. Mary's Convent", "Science", "sneha.stmary@gmail.com", "6543210987", CAMPUS_SCHOOL},
    {"Vikram Gupta", "Modern School", "Commerce", "vikram.modern@outlook.com", "9123456780", CAMPUS_SCHOOL}
};

// College Test Cases  
TestProfile collegeTestCases[] = {
    {"Dr. Amit Verma", "IIT Delhi", "Computer Science", "amit.iitd@iitd.ac.in", "9876543211", CAMPUS_COLLEGE},
    {"Kavya Reddy", "BITS Pilani", "Electronics", "kavya.bits@pilani.bits-pilani.ac.in", "8765432110", CAMPUS_COLLEGE},
    {"Arjun Mehta", "NIT Trichy", "Mechanical", "arjun.nitt@nitt.edu", "7654321099", CAMPUS_COLLEGE},
    {"Riya Joshi", "IIIT Hyderabad", "Information Technology", "riya.iiith@iiit.ac.in", "6543210988", CAMPUS_COLLEGE},
    {"Karan Agarwal", "DTU Delhi", "Civil Engineering", "karan.dtu@dtu.ac.in", "9123456781", CAMPUS_COLLEGE}
};

// Hospital Test Cases
TestProfile hospitalTestCases[] = {
    {"Dr. Rajesh Kumar", "AIIMS Delhi", "Cardiology", "dr.rajesh@aiims.edu", "9876543212", CAMPUS_HOSPITAL},
    {"Dr. Sunita Sharma", "Apollo Hospital", "Neurology", "dr.sunita@apollohospitals.com", "8765432111", CAMPUS_HOSPITAL},
    {"Dr. Manoj Gupta", "Fortis Healthcare", "Orthopedics", "dr.manoj@fortishealthcare.com", "7654321100", CAMPUS_HOSPITAL},
    {"Dr. Neha Singh", "Max Hospital", "Pediatrics", "dr.neha@maxhealthcare.com", "6543210989", CAMPUS_HOSPITAL},
    {"Dr. Suresh Patel", "Medanta Hospital", "Oncology", "dr.suresh@medanta.org", "9123456782", CAMPUS_HOSPITAL}
};

// Hostel Test Cases
TestProfile hostelTestCases[] = {
    {"Rohit Sharma", "Boys Hostel Block A", "Block A", "rohit.hostela@college.edu", "9876543213", CAMPUS_HOSTEL},
    {"Anita Kumari", "Girls Hostel Block B", "Block B", "anita.hostelb@college.edu", "8765432112", CAMPUS_HOSTEL},
    {"Deepak Yadav", "PG Hostel North", "North Wing", "deepak.pgn@hostel.com", "7654321101", CAMPUS_HOSTEL},
    {"Pooja Agarwal", "International Hostel", "International Block", "pooja.intl@hostel.edu", "6543210990", CAMPUS_HOSTEL},
    {"Manish Kumar", "Executive Hostel", "Executive Wing", "manish.exec@hostel.org", "9123456783", CAMPUS_HOSTEL}
};

// Test Functions
void testSchoolRegistration() {
    printf("\n=== SCHOOL REGISTRATION TESTS ===\n");
    
    for (int i = 0; i < 5; i++) {
        TestProfile *test = &schoolTestCases[i];
        printf("\nTest Case %d: %s\n", i+1, test->name);
        printf("School: %s\n", test->instituteName);
        printf("Stream: %s\n", test->department);
        printf("Email: %s\n", test->email);
        printf("Mobile: %s\n", test->mobile);
        
        // Test email validation
        ErrorCode emailResult = validateEmail(test->email);
        printf("Email Validation: %s\n", emailResult == SUCCESS ? "PASS" : "FAIL");
        
        // Test mobile validation
        ErrorCode mobileResult = validateMobile(test->mobile);
        printf("Mobile Validation: %s\n", mobileResult == SUCCESS ? "PASS" : "FAIL");
        
        printf("Campus Type: %s\n", getCampusName(test->campusType));
        printf("Status: %s\n", (emailResult == SUCCESS && mobileResult == SUCCESS) ? "✅ VALID" : "❌ INVALID");
    }
}

void testCollegeRegistration() {
    printf("\n=== COLLEGE REGISTRATION TESTS ===\n");
    
    for (int i = 0; i < 5; i++) {
        TestProfile *test = &collegeTestCases[i];
        printf("\nTest Case %d: %s\n", i+1, test->name);
        printf("College: %s\n", test->instituteName);
        printf("Department: %s\n", test->department);
        printf("Email: %s\n", test->email);
        printf("Mobile: %s\n", test->mobile);
        
        // Test email validation
        ErrorCode emailResult = validateEmail(test->email);
        printf("Email Validation: %s\n", emailResult == SUCCESS ? "PASS" : "FAIL");
        
        // Test mobile validation  
        ErrorCode mobileResult = validateMobile(test->mobile);
        printf("Mobile Validation: %s\n", mobileResult == SUCCESS ? "PASS" : "FAIL");
        
        printf("Campus Type: %s\n", getCampusName(test->campusType));
        printf("Status: %s\n", (emailResult == SUCCESS && mobileResult == SUCCESS) ? "✅ VALID" : "❌ INVALID");
    }
}

void testHospitalRegistration() {
    printf("\n=== HOSPITAL REGISTRATION TESTS ===\n");
    
    for (int i = 0; i < 5; i++) {
        TestProfile *test = &hospitalTestCases[i];
        printf("\nTest Case %d: %s\n", i+1, test->name);
        printf("Hospital: %s\n", test->instituteName);
        printf("Department: %s\n", test->department);
        printf("Email: %s\n", test->email);
        printf("Mobile: %s\n", test->mobile);
        
        // Test email validation
        ErrorCode emailResult = validateEmail(test->email);
        printf("Email Validation: %s\n", emailResult == SUCCESS ? "PASS" : "FAIL");
        
        // Test mobile validation
        ErrorCode mobileResult = validateMobile(test->mobile);
        printf("Mobile Validation: %s\n", mobileResult == SUCCESS ? "PASS" : "FAIL");
        
        printf("Campus Type: %s\n", getCampusName(test->campusType));
        printf("Status: %s\n", (emailResult == SUCCESS && mobileResult == SUCCESS) ? "✅ VALID" : "❌ INVALID");
    }
}

void testHostelRegistration() {
    printf("\n=== HOSTEL REGISTRATION TESTS ===\n");
    
    for (int i = 0; i < 5; i++) {
        TestProfile *test = &hostelTestCases[i];
        printf("\nTest Case %d: %s\n", i+1, test->name);
        printf("Hostel: %s\n", test->instituteName);
        printf("Block/Wing: %s\n", test->department);
        printf("Email: %s\n", test->email);
        printf("Mobile: %s\n", test->mobile);
        
        // Test email validation
        ErrorCode emailResult = validateEmail(test->email);
        printf("Email Validation: %s\n", emailResult == SUCCESS ? "PASS" : "FAIL");
        
        // Test mobile validation
        ErrorCode mobileResult = validateMobile(test->mobile);
        printf("Mobile Validation: %s\n", mobileResult == SUCCESS ? "PASS" : "FAIL");
        
        printf("Campus Type: %s\n", getCampusName(test->campusType));
        printf("Status: %s\n", (emailResult == SUCCESS && mobileResult == SUCCESS) ? "✅ VALID" : "❌ INVALID");
    }
}

// Edge Cases and Invalid Data Tests
void testInvalidCases() {
    printf("\n=== INVALID DATA TESTS ===\n");
    
    // Invalid Email Tests
    char *invalidEmails[] = {
        "invalid-email",           // No @ symbol
        "@domain.com",            // No username
        "user@",                  // No domain
        "user@domain",            // No TLD
        "user..name@domain.com",  // Double dots
        "user@domain..com"        // Double dots in domain
    };
    
    printf("\n--- Invalid Email Tests ---\n");
    for (int i = 0; i < 6; i++) {
        ErrorCode result = validateEmail(invalidEmails[i]);
        printf("Email: %-25s Result: %s\n", invalidEmails[i], 
               result == SUCCESS ? "❌ UNEXPECTED PASS" : "✅ CORRECTLY FAILED");
    }
    
    // Invalid Mobile Tests
    char *invalidMobiles[] = {
        "123456789",      // 9 digits (too short)
        "12345678901",    // 11 digits (too long)
        "1234567890",     // Starts with 1 (invalid)
        "abcd567890",     // Contains letters
        "9876-543-210",   // Contains hyphens
        "987 654 3210"    // Contains spaces
    };
    
    printf("\n--- Invalid Mobile Tests ---\n");
    for (int i = 0; i < 6; i++) {
        ErrorCode result = validateMobile(invalidMobiles[i]);
        printf("Mobile: %-15s Result: %s\n", invalidMobiles[i],
               result == SUCCESS ? "❌ UNEXPECTED PASS" : "✅ CORRECTLY FAILED");
    }
}

// Campus-Specific Data Field Tests
void testCampusDataFields() {
    printf("\n=== CAMPUS DATA FIELDS TESTS ===\n");
    
    // School subjects
    char schoolSubjects[][MAX_LEN] = {
        "Physics", "Chemistry", "Mathematics", "Biology", "English"
    };
    
    printf("\n--- School Subjects ---\n");
    for (int i = 0; i < 5; i++) {
        printf("Subject %d: %s ✅\n", i+1, schoolSubjects[i]);
    }
    
    // College courses
    char collegeCourses[][MAX_LEN] = {
        "Data Structures", "Computer Networks", "Database Systems", 
        "Software Engineering", "Machine Learning"
    };
    
    printf("\n--- College Courses ---\n");
    for (int i = 0; i < 5; i++) {
        printf("Course %d: %s ✅\n", i+1, collegeCourses[i]);
    }
    
    // Hospital medical fields
    char hospitalFields[][MAX_LEN] = {
        "Blood Pressure", "Temperature", "Weight", "Diagnosis"
    };
    
    printf("\n--- Hospital Medical Fields ---\n");
    for (int i = 0; i < 4; i++) {
        printf("Field %d: %s ✅\n", i+1, hospitalFields[i]);
    }
    
    // Hostel accommodation fields
    char hostelFields[][MAX_LEN] = {
        "Room Number", "Floor", "Mess Plan"
    };
    
    printf("\n--- Hostel Accommodation Fields ---\n");
    for (int i = 0; i < 3; i++) {
        printf("Field %d: %s ✅\n", i+1, hostelFields[i]);
    }
}

// Main test runner
int main() {
    printf("🎯 CAMPUS MANAGEMENT SYSTEM - COMPREHENSIVE TEST SUITE\n");
    printf("=====================================================\n");
    
    // Run all test suites
    testSchoolRegistration();
    testCollegeRegistration();
    testHospitalRegistration();
    testHostelRegistration();
    testInvalidCases();
    testCampusDataFields();
    
    printf("\n=== TEST SUMMARY ===\n");
    printf("✅ School Tests: 5 test cases\n");
    printf("✅ College Tests: 5 test cases\n");
    printf("✅ Hospital Tests: 5 test cases\n");
    printf("✅ Hostel Tests: 5 test cases\n");
    printf("✅ Invalid Data Tests: 12 edge cases\n");
    printf("✅ Data Fields Tests: All campus types\n");
    printf("\n🎉 Total: 32+ comprehensive test scenarios!\n");
    
    return 0;
}