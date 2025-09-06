# Campus Management System v2.0 - API Documentation

## **API Overview**

### **Core Functions**
The Campus Management System provides a comprehensive C API for managing multi-campus educational institutions.

### **Error Handling**
All functions return `ErrorCode` enum values for consistent error handling.

```c
typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_INPUT = 1,
    ERROR_FILE_IO = 2,
    ERROR_AUTH_FAILED = 3,
    ERROR_NOT_FOUND = 4,
    ERROR_PERMISSION = 5,
    ERROR_NETWORK = 6,
    ERROR_DATABASE = 7,
    ERROR_GENERAL = 8
} ErrorCode;
```

---

## **Authentication API**

### **User Registration**
```c
ErrorCode signup(void);
```
**Description:** Interactive user registration process  
**Returns:** `SUCCESS` on successful registration, error code otherwise  
**Usage:**
```c
ErrorCode result = signup();
if (result == SUCCESS) {
    printf("Registration successful\n");
}
```

### **User Login**
```c
ErrorCode signin(void);
```
**Description:** Interactive user login with 2FA  
**Returns:** `SUCCESS` on successful login, error code otherwise  
**Usage:**
```c
ErrorCode result = signin();
if (result == SUCCESS) {
    printf("Login successful\n");
}
```

### **Password Hashing**
```c
ErrorCode hashPassword(const char *password, char *hashOut);
```
**Parameters:**
- `password`: Input password string
- `hashOut`: Output buffer for hashed password (minimum 65 bytes)

**Returns:** `SUCCESS` or `ERROR_INVALID_INPUT`  
**Usage:**
```c
char hash[65];
ErrorCode result = hashPassword("MyPassword@123", hash);
```

### **Password Input (Hidden)**
```c
ErrorCode getHiddenPassword(char *password);
```
**Parameters:**
- `password`: Output buffer for password (minimum MAX_LEN bytes)

**Returns:** `SUCCESS` or `ERROR_INVALID_INPUT`  
**Usage:**
```c
char password[MAX_LEN];
ErrorCode result = getHiddenPassword(password);
```

---

## **Profile Management API**

### **View Profile**
```c
ErrorCode viewProfile(const char *userID);
```
**Parameters:**
- `userID`: User identification string

**Returns:** `SUCCESS`, `ERROR_NOT_FOUND`, or `ERROR_FILE_IO`  
**Usage:**
```c
ErrorCode result = viewProfile("sa25123");
```

### **Edit Profile**
```c
ErrorCode editProfile(const char *userID);
```
**Parameters:**
- `userID`: User identification string

**Returns:** `SUCCESS`, `ERROR_NOT_FOUND`, or `ERROR_FILE_IO`  
**Usage:**
```c
ErrorCode result = editProfile("sa25123");
```

### **Change Password**
```c
ErrorCode changePassword(const char *userID);
```
**Parameters:**
- `userID`: User identification string

**Returns:** `SUCCESS`, `ERROR_AUTH_FAILED`, or `ERROR_FILE_IO`  
**Usage:**
```c
ErrorCode result = changePassword("sa25123");
```

---

## **Campus Management API**

### **Campus Type Selection**
```c
CampusType selectCampusType(void);
```
**Returns:** Selected campus type enum value  
**Usage:**
```c
CampusType type = selectCampusType();
switch(type) {
    case CAMPUS_SCHOOL: /* handle school */ break;
    case CAMPUS_COLLEGE: /* handle college */ break;
    // ...
}
```

### **Campus Name Lookup**
```c
const char* getCampusName(CampusType type);
```
**Parameters:**
- `type`: Campus type enum value

**Returns:** String representation of campus type  
**Usage:**
```c
const char* name = getCampusName(CAMPUS_SCHOOL);
printf("Campus: %s\n", name); // Output: "Campus: School"
```

---

## **Data Management API**

### **School Data Management**
```c
void saveSchoolData(const char *studentID);
void loadSchoolData(const char *studentID);
```

### **College Data Management**
```c
void saveCollegeData(const char *studentID);
void loadCollegeData(const char *studentID);
```

### **Hospital Data Management**
```c
void saveHospitalData(const char *patientID);
void loadHospitalData(const char *patientID);
```

### **Hostel Data Management**
```c
void saveHostelData(const char *residentID);
void loadHostelData(const char *residentID);
```

**Usage Example:**
```c
// Save school marks
saveSchoolData("sa25123");

// Load and display data
loadSchoolData("sa25123");
```

---

## **Grade Calculation API**

### **Grade Assignment**
```c
const char* getGrade(float percent);
```
**Parameters:**
- `percent`: Percentage value (0.0 - 100.0)

**Returns:** Grade string (A+, A, B, C, D, F)  
**Usage:**
```c
float percentage = 86.5f;
const char* grade = getGrade(percentage);
printf("Grade: %s\n", grade); // Output: "Grade: A"
```

### **Summary Calculation**
```c
void printSummary(int total, int full, CampusType type);
```
**Parameters:**
- `total`: Total marks obtained
- `full`: Total possible marks
- `type`: Campus type for grade calculation

**Usage:**
```c
printSummary(433, 500, CAMPUS_SCHOOL);
// Output: Total: 433 / 500
//         Percentage: 86.60%
//         Grade: A
```

---

## **PDF Export API**

### **School Report Export**
```c
void exportSchoolPDF(const char *studentID);
```

### **College Transcript Export**
```c
void exportCollegePDF(const char *studentID);
```

### **Medical Report Export**
```c
void exportHospitalPDF(const char *patientID);
```

### **Hostel Report Export**
```c
void exportHostelPDF(const char *residentID);
```

**Usage Example:**
```c
// Generate school report card
exportSchoolPDF("sa25123");
// Creates: data/sa25123_school_report.pdf
```

---

## **Utility API**

### **Email Validation**
```c
ErrorCode validateEmail(const char *email);
```
**Parameters:**
- `email`: Email address string

**Returns:** `SUCCESS` if valid, `ERROR_INVALID_INPUT` if invalid  
**Usage:**
```c
ErrorCode result = validateEmail("user@example.com");
if (result == SUCCESS) {
    printf("Valid email\n");
}
```

### **Mobile Validation**
```c
ErrorCode validateMobile(const char *mobile);
```
**Parameters:**
- `mobile`: Mobile number string

**Returns:** `SUCCESS` if valid, `ERROR_INVALID_INPUT` if invalid  
**Usage:**
```c
ErrorCode result = validateMobile("9876543210");
if (result == SUCCESS) {
    printf("Valid mobile\n");
}
```

### **Logging**
```c
ErrorCode logEvent(const char *userID, const char *action);
```
**Parameters:**
- `userID`: User identification
- `action`: Action description

**Returns:** `SUCCESS` or `ERROR_FILE_IO`  
**Usage:**
```c
logEvent("sa25123", "User logged in successfully");
```

### **File Path Generation**
```c
ErrorCode getProfilePath(char *path, const char *userID);
```
**Parameters:**
- `path`: Output buffer for file path (minimum 150 bytes)
- `userID`: User identification

**Returns:** `SUCCESS` or `ERROR_INVALID_INPUT`  
**Usage:**
```c
char filepath[150];
ErrorCode result = getProfilePath(filepath, "sa25123");
```

---

## **Unified Campus API**

### **Campus Configuration**
```c
CampusConfig getCampusConfig(CampusType type);
```
**Parameters:**
- `type`: Campus type enum

**Returns:** Configuration structure for campus type  
**Usage:**
```c
CampusConfig config = getCampusConfig(CAMPUS_SCHOOL);
if (config.usesGrades) {
    // Handle grade-based system
}
```

### **Unified Data Operations**
```c
ErrorCode saveCampusData(const char* userID, CampusType campusType);
ErrorCode loadCampusData(const char* userID);
```
**Parameters:**
- `userID`: User identification
- `campusType`: Type of campus

**Returns:** `SUCCESS` or appropriate error code  
**Usage:**
```c
// Save data for any campus type
ErrorCode result = saveCampusData("sa25123", CAMPUS_SCHOOL);

// Load data (auto-detects campus type)
ErrorCode result = loadCampusData("sa25123");
```

---

## **Security API**

### **Account Lockout Management**
```c
bool isAccountLocked(const char *userID);
void lockAccount(const char *userID, int minutes);
```

### **OTP Management**
```c
bool generateOTP(const char *userID, char *otp);
bool verifyOTP(const char *userID, const char *inputOTP);
void sendOTPSMS(const char *mobile, const char *otp);
void sendOTPEmail(const char *email, const char *otp);
```

### **Session Management**
```c
ErrorCode createSession(const char *userID, int authLevel, Session *session);
void destroySession(const char *sessionToken);
```

---

## **Data Structures**

### **Profile Structure**
```c
typedef struct {
    char userID[20];
    char name[MAX_LEN];
    char instituteName[MAX_LEN];
    char department[50];
    char email[MAX_LEN];
    char mobile[15];
    char passwordHash[64];
    CampusType campusType;
    int dataCount;
    char dataFields[MAX_SUBJECTS][MAX_LEN];
} Profile;
```

### **Unified Campus Data**
```c
typedef struct {
    char userID[20];
    char name[MAX_LEN];
    CampusType campusType;
    
    // School/College fields
    int marks[MAX_SUBJECTS];
    int credits[MAX_SUBJECTS];
    
    // Hospital fields
    char bloodPressure[20];
    char diagnosis[MAX_LEN];
    
    // Hostel fields
    char roomNumber[10];
    char messPlan[50];
    
    time_t lastUpdated;
} UnifiedCampusData;
```

### **Campus Configuration**
```c
typedef struct {
    CampusType type;
    bool usesGrades;
    bool usesCredits;
    bool usesMedicalData;
    bool usesAccommodation;
    const char* dataTypeName;
    const char* errorPrefix;
} CampusConfig;
```

---

## **Usage Examples**

### **Complete Registration Flow**
```c
// 1. Register new user
ErrorCode result = signup();
if (result != SUCCESS) {
    printf("Registration failed: %d\n", result);
    return result;
}

// 2. Login user
result = signin();
if (result != SUCCESS) {
    printf("Login failed: %d\n", result);
    return result;
}

// 3. Add data based on campus type
CampusType type = selectCampusType();
switch(type) {
    case CAMPUS_SCHOOL:
        saveSchoolData("sa25123");
        break;
    case CAMPUS_COLLEGE:
        saveCollegeData("ii25124");
        break;
    // ... other cases
}

// 4. Generate report
exportSchoolPDF("sa25123");
```

### **Data Validation Example**
```c
// Validate user inputs
ErrorCode emailResult = validateEmail("user@example.com");
ErrorCode mobileResult = validateMobile("9876543210");

if (emailResult == SUCCESS && mobileResult == SUCCESS) {
    printf("All inputs valid\n");
} else {
    printf("Invalid input detected\n");
}
```

---

## **API Support**

### **Error Handling Best Practices**
```c
ErrorCode result = someFunction();
switch(result) {
    case SUCCESS:
        // Handle success
        break;
    case ERROR_INVALID_INPUT:
        printf("Invalid input provided\n");
        break;
    case ERROR_FILE_IO:
        printf("File operation failed\n");
        break;
    default:
        printf("Unexpected error: %d\n", result);
        break;
}
```

### **Memory Management**
- All string buffers should be properly sized
- Use provided constants (MAX_LEN, etc.)
- Check return values for all API calls
- Handle error conditions appropriately

---

**Campus Management System v2.0 - API Documentation Complete!**

---

## **Support & Repository**

- **GitHub Repository:** https://github.com/ajay-EY-1859/campus
- **Email Support:** raj183576@gmail.com
- **Issues & Bug Reports:** https://github.com/ajay-EY-1859/campus/issues