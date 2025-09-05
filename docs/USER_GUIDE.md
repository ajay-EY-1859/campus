# User Guide - Campus Management System

A comprehensive guide for using the Campus Management System effectively.

## 🎯 Getting Started

### First Time Users
1. **Launch the application**
2. **Select "1. Register New User"**
3. **Follow the registration process**
4. **Login with your credentials**
5. **Explore your dashboard**

### System Overview
The Campus Management System supports four types of campuses:
- 🏫 **School**: Student marks and grade management
- 🎓 **College**: Course credits and CGPA tracking
- 🏥 **Hospital**: Patient records and medical data
- 🏠 **Hostel**: Resident information and accommodation

## 📝 Registration Process

### Step 1: Personal Information
```
Enter your name: John Doe
```
- Use your full legal name
- Avoid special characters
- Maximum 99 characters

### Step 2: Campus Type Selection
```
Select Campus Type:
1. School
2. College
3. Hospital
4. Hostel
Enter choice: 1
```

### Step 3: Institution Details
```
Enter School name: Delhi Public School
Department/Stream: Science
```
- Enter the full official name
- Department/Stream helps in categorization

### Step 4: Campus-Specific Setup

#### For School Users
```
Number of subjects: 5
Subject 1: Mathematics
Subject 2: Physics
Subject 3: Chemistry
Subject 4: Biology
Subject 5: English
```

#### For College Users
```
Number of courses: 4
Course 1: Data Structures
Course 2: Database Systems
Course 3: Computer Networks
Course 4: Software Engineering
```

#### For Hospital Users
- System automatically sets up medical fields:
  - Blood Pressure
  - Temperature
  - Weight
  - Diagnosis

#### For Hostel Users
- System automatically sets up accommodation fields:
  - Room Number
  - Floor
  - Mess Plan

### Step 5: Contact Information
```
Email: john.doe@email.com
Mobile: 9876543210
```
- **Email**: Must be valid format (user@domain.com)
- **Mobile**: Must be 10 digits starting with 6-9

### Step 6: Password Creation
```
Password (input hidden): ********
```
**Password Requirements:**
- Minimum 8 characters
- At least one uppercase letter
- At least one lowercase letter
- At least one digit
- At least one special character (!@#$%^&*...)

### Step 7: Account Creation
```
Registration successful! Your ID: dp25123
View profile? (y/n): y
```
- Your User ID is automatically generated
- Save this ID for future logins

## 🔐 Login Process

### Step 1: User Identification
```
User ID: dp25123
```
- Enter the ID provided during registration
- Case-sensitive

### Step 2: Mobile Verification
```
Mobile Number: 9876543210
```
- Enter the mobile number from registration
- Used for OTP delivery

### Step 3: Password Entry
```
Password (input hidden): ********
```
- Enter your registration password
- Input is hidden for security

### Step 4: Two-Factor Authentication
```
OTP sent to your mobile and email. Enter OTP: 123456
```
- Check your mobile for SMS
- Check your email for OTP
- OTP expires in 5 minutes
- Enter the 6-digit code

### Step 5: Successful Login
```
Login successful! Welcome, John Doe [dp25123]
```

## 🏠 Dashboard Navigation

### Main Menu Options
```
School Dashboard
1. View Profile
2. Edit Profile
3. Add Data
4. View Data
5. Change Password
6. Export Report
7. Logout
8. Exit Application
Select option: 
```

## 👤 Profile Management

### View Profile
```
School Profile
Name: John Doe
School: Delhi Public School
Department: Science
Email: john.doe@email.com
Mobile: 9876543210
User ID: dp25123
Data Fields (5):
  1. Mathematics
  2. Physics
  3. Chemistry
  4. Biology
  5. English
```

### Edit Profile
- Update personal information
- Change institution details
- Modify contact information
- Cannot change User ID or Campus Type

```
Edit Profile
Current Name: John Doe
Enter new name (or press Enter to keep): John Smith

Current School: Delhi Public School
Enter new School (or press Enter to keep): [Enter]

Profile updated successfully!
```

## 📊 Data Management

### Adding Data

#### School Data Entry
```
Enter marks for subjects:
Mathematics - Marks: 85
Full marks: 100
Physics - Marks: 78
Full marks: 100
...
School marks saved
```

#### College Data Entry
```
Enter semester marks:
Data Structures - Marks: 85
Credits: 4
Database Systems - Marks: 90
Credits: 3
...
College marks saved
```

#### Hospital Data Entry
```
Enter medical data:
Blood Pressure: 120/80
Temperature: 98.6
Weight: 70
Diagnosis: Normal checkup
Medical data saved
```

#### Hostel Data Entry
```
Enter hostel data:
Room Number: 201
Floor: 2
Mess Plan: Vegetarian
Hostel data saved
```

### Viewing Data

#### School Data Display
```
1. Mathematics - 85/100
2. Physics - 78/100
3. Chemistry - 92/100
4. Biology - 88/100
5. English - 79/100
Total: 422 / 500
Percentage: 84.40%
Grade: A
```

#### College Data Display
```
1. Data Structures - 85 marks (4 credits)
2. Database Systems - 90 marks (3 credits)
3. Computer Networks - 82 marks (4 credits)
4. Software Engineering - 88 marks (3 credits)
CGPA: 8.64
```

## 📄 Report Generation

### PDF Export Process
```
Select option: 6
School report PDF exported to data/dp25123_school_report.pdf
```

### Report Contents

#### School Report
- Student information
- Subject-wise marks
- Total marks and percentage
- Grade assignment
- Professional formatting

#### College Transcript
- Student details
- Course-wise marks and credits
- CGPA calculation
- Academic summary

#### Medical Report
- Patient information
- Medical data entries
- Health metrics
- Professional medical format

#### Hostel Report
- Resident details
- Accommodation information
- Room and facility details

## 🔒 Security Features

### Password Management
```
Enter current password: (input hidden): ********
Enter new password: (input hidden): ********
Confirm new password: (input hidden): ********
Password changed successfully!
```

### Account Security
- **Account Lockout**: After 3 failed login attempts
- **Lockout Duration**: 15 minutes
- **Session Timeout**: 30 minutes of inactivity
- **OTP Expiry**: 5 minutes

### Security Best Practices
1. **Use Strong Passwords**
   - Mix of uppercase, lowercase, numbers, symbols
   - Avoid common words or personal information
   - Change regularly

2. **Protect Your Credentials**
   - Don't share User ID or password
   - Don't share OTP codes
   - Log out after use

3. **Monitor Account Activity**
   - Check for suspicious login attempts
   - Report unusual activity immediately

## 🚨 Error Handling

### Common Errors and Solutions

#### "Invalid input"
- **Cause**: Incorrect data format
- **Solution**: Follow the specified format (e.g., 10-digit mobile)

#### "Account is temporarily locked"
- **Cause**: Too many failed login attempts
- **Solution**: Wait 15 minutes before trying again

#### "Invalid OTP"
- **Cause**: Wrong OTP or expired OTP
- **Solution**: Request new OTP or check SMS/email

#### "Registration failed"
- **Cause**: Weak password or invalid data
- **Solution**: Use stronger password, check all fields

#### "Cannot load profile"
- **Cause**: Corrupted data or file permissions
- **Solution**: Contact support for data recovery

### Data Recovery
If you lose access to your account:
1. Check if account is locked (wait 15 minutes)
2. Verify User ID spelling
3. Ensure mobile number is correct
4. Contact support with proof of identity

## 💡 Tips and Best Practices

### Efficient Usage
1. **Regular Data Entry**: Update marks/data regularly
2. **Backup Reports**: Save PDF reports to external storage
3. **Profile Updates**: Keep contact information current
4. **Security Checks**: Monitor login attempts in logs

### Data Organization
1. **Consistent Naming**: Use standard subject/course names
2. **Regular Backups**: Export reports periodically
3. **Data Validation**: Double-check entries before saving

### Performance Tips
1. **Close Unused Sessions**: Always logout when done
2. **Regular Cleanup**: Clear old temporary files
3. **System Maintenance**: Keep system updated

## 🔧 Troubleshooting

### Application Won't Start
1. Check if all files are present
2. Verify file permissions
3. Ensure sufficient disk space
4. Run as administrator (if needed)

### Slow Performance
1. Close other applications
2. Check available memory
3. Restart the application
4. Check disk space

### Data Not Saving
1. Check file permissions
2. Ensure disk space available
3. Verify data directory exists
4. Check for file locks

### PDF Generation Fails
1. Ensure data is entered first
2. Check disk space for output
3. Verify PDF library files
4. Check file permissions

## 📞 Getting Help

### Self-Help Resources
1. **This User Guide**: Comprehensive usage instructions
2. **FAQ Section**: Common questions and answers
3. **Error Messages**: Built-in help for error codes
4. **Community Forum**: User discussions and tips

### Contact Support
- **GitHub Issues**: Technical problems and bugs
- **Email**: support@campus-project.com
- **Documentation**: Online help and tutorials

### Reporting Issues
When reporting problems, include:
- Your operating system
- Steps to reproduce the issue
- Error messages (exact text)
- Screenshots if helpful
- Your User ID (for account-specific issues)

## 📚 Advanced Features

### Keyboard Shortcuts
- **Ctrl+C**: Cancel current operation
- **Enter**: Confirm selection
- **Esc**: Go back (where applicable)

### Batch Operations
- Multiple data entries in sequence
- Bulk report generation (future feature)
- Data import/export (future feature)

### Integration Options
- Export data for external analysis
- Import from other systems (future feature)
- API access (future feature)

---

**Need more help? Check our [FAQ](FAQ.md) or [contact support](mailto:support@campus-project.com)! 📧**