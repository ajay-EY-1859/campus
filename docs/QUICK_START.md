# Campus Management System - Quick Start Guide

## Running the Application

### Windows:
```bash
cd d:\campus\build\gui
campus_gui.exe
```

### Linux:
```bash
cd /campus/build/gui
./campus_gui
```

## First Time Setup

### 1. Register a New Account
- Click "Create New Account"
- Enter: Name, Email, Mobile, Password
- Select Campus Type (School/College/Hospital/Hostel)
- Click "Register"

### 2. Login
- Use the credentials you just created
- Click "Login"

### 3. Add Your First Record
- Click "Add New Record"
- Fill in the required fields
- Click "Save Record"

## Main Features

### Dashboard
Your home page after login. Quick access to:
- 📝 Add New Record
- 👁️ View Records  
- 👤 Profile Settings
- 🚪 Logout

### Add New Record
Create and save new entries:
- Title and description fields
- Detailed information
- Automatic status updates
- Save with confirmation

### View Records
Manage existing data:
- Search functionality
- Filter options
- View all records
- Edit/Delete records

### Profile
Manage your account:
- View personal information
- Change password
- Update campus details

## Campus Type Specific Features

### School
- Student name, board, class
- Roll number
- Subject marks entry
- GPA calculation

### College  
- Student name & enrollment
- Course selection
- Semester tracking
- GPA & credits

### Hospital
- Patient ID & name
- Age & diagnosis
- Doctor assignment
- Room allocation
- Treatment status

### Hostel
- Student ID & name
- Room & block assignment
- Floor level
- Room type (Single/Double)
- Rent amount
- Check-in date

## Keyboard Shortcuts

| Action | Shortcut |
|--------|----------|
| Focus Next Field | Tab |
| Hide Password | Shift+Enter |
| Submit Form | Ctrl+S |
| Go Back | Esc |

## Password Requirements

- Minimum 8 characters
- At least one uppercase letter
- At least one number
- Special characters recommended

## Data Export

After adding records, you can:
1. View all records
2. Filter by date range
3. Export as PDF/CSV/Excel/JSON

## Troubleshooting

### Can't Login
- Check User ID and password
- Verify mobile number matches registration
- Wait 15 mins if account locked (5+ failed attempts)

### Records Not Saving
- Check all required fields are filled
- Verify database file is writable
- Check available disk space

### GUI Won't Start
- Ensure GTK4 is installed
- Check libhpdf.dll exists in path
- Verify file permissions

### Missing Libraries
```bash
# Windows (MinGW)
- GTK 4.20.3+
- GLib 2.x
- libhpdf.dll

# Linux
sudo apt install libgtk-4-dev libglib2.0-dev libhpdf-dev
```

## File Structure After First Run

```
d:\campus\
├── data/                 (Saved records)
├── credentials/          (User accounts)
├── logs/                 (Activity logs)
│   └── login_audit.log   (Login history)
└── build/gui/campus_gui.exe
```

## Common Tasks

### Add Student Record
1. Click "Add New Record"
2. Select "School" if not auto-selected
3. Enter: Name, Board, Class, Roll No
4. Add subjects and marks
5. Click "Save Record"

### View My Data
1. Click "View Records"
2. Type your name in search box
3. Click "Search"
4. Select record to view details

### Change Password
1. Go to "Profile"
2. Click "Change Password"
3. Enter old password
4. Enter new password (2x)
5. Save changes

### Export Records
1. Click "Export Reports"
2. Choose format (PDF/CSV/Excel)
3. Select date range
4. Click "Generate Report"

## Contact & Support

For issues or feature requests:
- Check docs/USER_MANUAL.md
- See TROUBLESHOOTING section
- Review logs in logs/ directory

## Security Tips

✅ DO:
- Use strong passwords
- Change password regularly  
- Logout after finishing
- Keep credentials private
- Report suspicious activity

❌ DON'T:
- Share your login credentials
- Save password in plain text
- Leave system unattended
- Use weak passwords
- Share sensitive data

## Performance Tips

- Keep database under 10MB for best performance
- Archive old records periodically
- Clear logs monthly
- Rebuild database yearly

## FAQs

**Q: How many users can register?**
A: Unlimited. Each gets unique ID.

**Q: Can I recover a deleted record?**
A: Check logs/ directory for backups.

**Q: What's the maximum file size?**
A: 50MB per database file.

**Q: Is my data encrypted?**
A: Yes, credentials are hashed. Data uses file-based security.

**Q: Can multiple users login simultaneously?**
A: No, single-session per instance.

---

**Version**: 2.0
**Last Updated**: December 2025
**Status**: ✅ Production Ready
