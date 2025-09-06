# 📚 Campus Management System v2.0 - User Manual

## 🎯 **Getting Started**

### **System Overview**
Campus Management System is a comprehensive solution for managing different types of educational and institutional campuses including Schools, Colleges, Hospitals, and Hostels.

### **Key Features**
- ✅ Multi-campus support (4 types)
- ✅ Secure user authentication with 2FA
- ✅ Campus-specific data management
- ✅ Professional PDF report generation
- ✅ Grade and CGPA calculations
- ✅ Comprehensive audit logging

---

## 🚀 **Quick Start Guide**

### **First Time Setup**
1. **Launch Application**
   ```
   Double-click campus.exe or run from command line
   ```

2. **Main Menu Options**
   ```
   ========================================
   Campus Management System v2.0
   ========================================
   1. Register New User
   2. Login
   3. Exit
   ```

3. **Choose Registration**
   - Select option `1` for new user registration
   - Follow the step-by-step process

---

## 👤 **User Registration Process**

### **Step 1: Personal Information**
```
Enter your name: John Doe
Enter email: john.doe@example.com
Enter mobile number: 9876543210
```

**Requirements:**
- **Name:** 2-50 characters
- **Email:** Valid format (user@domain.com)
- **Mobile:** 10 digits, starting with 6-9

### **Step 2: Campus Type Selection**
```
Select Campus Type:
1. School
2. College  
3. Hospital
4. Hostel
Enter choice: 1
```

### **Step 3: Campus-Specific Information**

#### **🏫 For School:**
```
Enter School Name: Delhi Public School
Enter Stream (Science/Commerce/Arts): Science
Number of subjects: 5
Subject 1: Physics
Subject 2: Chemistry
Subject 3: Mathematics
Subject 4: Biology
Subject 5: English
```

#### **🎓 For College:**
```
Enter College Name: IIT Delhi
Enter Department: Computer Science Engineering
Number of courses: 4
Course 1: Data Structures
Course 2: Algorithms
Course 3: Database Systems
Course 4: Computer Networks
```

#### **🏥 For Hospital:**
```
Enter Hospital Name: AIIMS Delhi
Enter Department: Cardiology
Medical data fields will be: Blood Pressure, Temperature, Weight, Diagnosis
```

#### **🏠 For Hostel:**
```
Enter Hostel Name: Boys Hostel Block A
Enter Block/Wing: Block A
Accommodation fields will be: Room Number, Floor, Mess Plan
```

### **Step 4: Password Creation**
```
Create password: ********
Confirm password: ********
```

**Password Requirements:**
- Minimum 8 characters
- At least one uppercase letter
- At least one lowercase letter  
- At least one digit
- At least one special character (@#$%^&*)

### **Step 5: User ID Generation**
```
Registration successful!
Your User ID: sa25123
Please save this ID for login.
```

---

## 🔐 **Login Process**

### **Step 1: Basic Authentication**
```
User ID: sa25123
Mobile Number: 9876543210
Password: ********
```

### **Step 2: Two-Factor Authentication**
```
OTP sent to your mobile and email.
Enter OTP: 123456
```

**OTP Details:**
- Valid for 5 minutes
- 6-digit numeric code
- Sent via SMS and email

### **Step 3: Successful Login**
```
Login successful! Welcome, John Doe [sa25123]
```

---

## 🏠 **Dashboard Overview**

### **Main Dashboard Menu**
```
========================================
Dashboard - John Doe [sa25123]
========================================
1. View Profile
2. Edit Profile  
3. Add Data
4. View Data
5. Change Password
6. Export Report (PDF)
7. Logout
8. Exit Application
```

---

## 📊 **Data Management**

### **Adding Data (Option 3)**

#### **🏫 School - Enter Marks**
```
Enter marks for subjects:
Physics - Marks: 85
Full marks: 100
Chemistry - Marks: 92  
Full marks: 100
Mathematics - Marks: 78
Full marks: 100
Biology - Marks: 88
Full marks: 100
English - Marks: 90
Full marks: 100
```

#### **🎓 College - Enter Semester Data**
```
Enter semester marks:
Data Structures - Marks: 88
Credits: 4
Algorithms - Marks: 92
Credits: 4
Database Systems - Marks: 85
Credits: 3
Computer Networks - Marks: 90
Credits: 3
```

#### **🏥 Hospital - Enter Medical Data**
```
Enter medical data:
Blood Pressure: 120/80 mmHg
Temperature: 98.6°F
Weight: 70 kg
Diagnosis: Hypertension - Stage 1, medication required
```

#### **🏠 Hostel - Enter Accommodation Data**
```
Enter hostel data:
Room Number: A-101
Floor: Ground Floor
Mess Plan: Vegetarian - Full Meal Plan (Breakfast, Lunch, Dinner)
```

### **Viewing Data (Option 4)**

#### **🏫 School Results Display**
```
School Data for sa25123:
1. Physics - 85/100
2. Chemistry - 92/100  
3. Mathematics - 78/100
4. Biology - 88/100
5. English - 90/100

Total: 433 / 500
Percentage: 86.60%
Grade: A
```

#### **🎓 College Results Display**
```
College Data for ii25124:
1. Data Structures - 88 marks (4 credits)
2. Algorithms - 92 marks (4 credits)
3. Database Systems - 85 marks (3 credits)
4. Computer Networks - 90 marks (3 credits)

CGPA: 8.76
```

---

## 📄 **PDF Report Generation**

### **Export Report (Option 6)**
```
Generating PDF report...
School report PDF exported to data/sa25123_school_report.pdf
```

### **Report Contents**

#### **🏫 School Report Card**
- Student name and details
- School information
- Subject-wise marks breakdown
- Total marks and percentage
- Grade assignment
- Professional formatting

#### **🎓 College Transcript**
- Student information
- College and department details
- Course-wise marks and credits
- CGPA calculation
- Academic summary

#### **🏥 Medical Report**
- Patient information
- Hospital and department
- Complete medical data
- Health assessment
- Professional medical format

#### **🏠 Hostel Report**
- Resident information
- Hostel details
- Accommodation information
- Room allocation details
- Mess plan information

---

## ⚙️ **Profile Management**

### **View Profile (Option 1)**
```
School Profile
Name: John Doe
School: Delhi Public School
Department: Science
Email: john.doe@example.com
Mobile: 9876543210
User ID: sa25123
Data Fields (5):
  1. Physics
  2. Chemistry
  3. Mathematics
  4. Biology
  5. English
```

### **Edit Profile (Option 2)**
```
Edit Profile
Current Name: John Doe
Enter new name (or press Enter to keep): 
Current School: Delhi Public School
Enter new School (or press Enter to keep): 
Current Department: Science
Enter new department (or press Enter to keep): 
Current Email: john.doe@example.com
Enter new email (or press Enter to keep): 
Current Mobile: 9876543210
Enter new mobile (or press Enter to keep): 

Profile updated successfully!
```

### **Change Password (Option 5)**
```
Enter current password: ********
Enter new password: ********
Confirm new password: ********
Password changed successfully!
```

---

## 🎯 **Grade System Guide**

### **🏫 School Grading**
| Percentage | Grade | Status |
|---|---|---|
| 90% - 100% | A+ | Excellent |
| 80% - 89% | A | Very Good |
| 70% - 79% | B | Good |
| 60% - 69% | C | Satisfactory |
| 50% - 59% | D | Pass |
| Below 50% | F | Fail |

### **🎓 College CGPA System**
| CGPA Range | Grade | Status |
|---|---|---|
| 9.0 - 10.0 | A+ | Outstanding |
| 8.0 - 8.9 | A | Excellent |
| 7.0 - 7.9 | B+ | Very Good |
| 6.0 - 6.9 | B | Good |
| 5.0 - 5.9 | C | Average |
| Below 5.0 | F | Fail |

**CGPA Calculation:**
```
CGPA = (Sum of (Marks × Credits)) / (Total Credits × 10)
```

---

## 🔒 **Security Features**

### **Account Security**
- **Password Protection:** Strong password requirements
- **Two-Factor Authentication:** OTP verification required
- **Account Lockout:** 3 failed attempts = 15-minute lockout
- **Session Management:** 30-minute automatic timeout

### **Data Security**
- **Input Validation:** All inputs sanitized and validated
- **Audit Logging:** All activities logged with timestamps
- **Data Encryption:** Sensitive data protected
- **Backup Protection:** Automatic data backups

---

## ❗ **Troubleshooting**

### **Common Issues**

**Issue: Login Failed**
```
Possible causes:
- Incorrect User ID format (should be 2 letters + 25 + 3 digits)
- Wrong mobile number
- Incorrect password
- Account locked (wait 15 minutes)

Solution: Verify credentials and try again
```

**Issue: OTP Not Received**
```
Possible causes:
- OTP expired (5-minute limit)
- Network connectivity issues
- Incorrect mobile/email

Solution: Request new OTP or check connectivity
```

**Issue: PDF Generation Failed**
```
Possible causes:
- No data entered yet
- Insufficient disk space
- Missing hpdf.dll file

Solution: Add data first, check disk space, verify installation
```

**Issue: Data Not Saved**
```
Possible causes:
- Invalid input format
- File permission issues
- Disk space full

Solution: Check input format, verify permissions, free disk space
```

---

## 📞 **Support Information**

### **Getting Help**
1. **Check this manual first**
2. **Review troubleshooting section**
3. **Contact system administrator**
4. **Submit support ticket**

### **Contact Information**
- **Email:** support@campus-system.com
- **Phone:** +1-800-CAMPUS
- **Website:** www.campus-system.com/support

---

## 🎉 **Tips for Best Experience**

### **✅ Best Practices**
- Use strong, unique passwords
- Log out after each session
- Keep your User ID secure
- Regular data backups
- Update contact information

### **✅ Performance Tips**
- Close application when not in use
- Regular system maintenance
- Keep sufficient disk space
- Use recommended system specifications

---

**📚 Campus Management System v2.0 - User Manual Complete! 🎓**