# 🎓 Campus Management System

[![Windows](https://img.shields.io/badge/Platform-Windows-blue.svg)](https://www.microsoft.com/windows)
[![PowerShell](https://img.shields.io/badge/PowerShell-5.1+-purple.svg)](https://docs.microsoft.com/powershell/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](license.md)

A modern, accessible GUI application for managing multiple campus types including Schools, Colleges, Hospitals, and Hostels. Built with PowerShell and Windows Forms.

![Campus Management System](https://img.shields.io/badge/Version-2.0-brightgreen)

---

## ✨ Features

### 🏫 Multi-Campus Support
- **School** - Student marks, percentage calculation
- **College** - Course credits, grade management
- **Hospital** - Patient records, diagnosis tracking
- **Hostel** - Room allocation, rent management

### 🔐 User Management
- Secure registration with unique User ID generation
- Login with Mobile + User ID + Password verification
- Password change functionality
- Forgot User ID recovery via email

### 📊 Data Management
- Add, view, and manage campus-specific records
- Profile viewing with complete user information
- Real-time data updates

### 📁 Export Features
- **TXT Export** - Detailed text report with all records
- **CSV Export** - Spreadsheet-compatible data export

### ♿ Accessibility
- Screen reader compatible (NVDA/JAWS)
- Keyboard navigation support
- High contrast UI elements
- Clear focus indicators

---

## 🚀 Quick Start

### Option 1: Run Executable (Recommended)
1. Download `CampusGUI.exe`
2. Double-click to run
3. No installation required!

### Option 2: Run PowerShell Script
```powershell
# Navigate to project folder
cd d:\campus

# Run the script
powershell -ExecutionPolicy Bypass -File CampusGUI.ps1
```

### Option 3: Build from Source
```powershell
# Install PS2EXE if not already installed
Install-Module -Name ps2exe -Scope CurrentUser

# Build EXE
ps2exe -inputFile CampusGUI.ps1 -outputFile CampusGUI.exe -noConsole
```

---

## 📖 Usage Guide

### Registration
1. Click **Register** on main menu
2. Select campus type (School/College/Hospital/Hostel)
3. Fill in your details (Name, Email, Mobile, Password)
4. Note your generated **User ID** (e.g., SC1234, CO5678)

### Login
1. Click **Login**
2. Enter Mobile Number
3. Enter User ID
4. Enter Password

### Dashboard Options
| Option | Description |
|--------|-------------|
| Add Data | Enter campus-specific records |
| View Data | See all your records in a table |
| View Profile | Display your profile information |
| Export Report | Save data as TXT or CSV file |
| Change Password | Update your password |
| Logout | Return to main menu |

---

## 📁 Project Structure

```
campus/
├── CampusGUI.ps1      # Main PowerShell GUI application
├── CampusGUI.exe      # Compiled standalone executable
├── build.ps1          # Build script for EXE compilation
├── README.md          # This file
├── BUILD.md           # Build instructions
├── license.md         # MIT License
├── data/              # User data storage (JSON files)
│   └── {UserID}.json  # Individual user data
├── docs/              # Documentation
│   ├── USER_GUIDE.md
│   ├── USER_MANUAL.md
│   ├── API_DOCUMENTATION.md
│   ├── ARCHITECTURE.md
│   └── ...
├── include/           # C header files (backend reference)
└── src/               # C source files (backend reference)
```

---

## 💾 Data Storage

User data is stored in JSON format in the `data/` folder:

```json
{
  "UserID": "SC1234",
  "Name": "John Doe",
  "Campus": "School",
  "Email": "john@example.com",
  "Mobile": "9876543210",
  "Records": [...]
}
```

**Export files** are saved to:
- `data/{UserID}_report.txt` - Text reports
- `data/{UserID}_records.csv` - CSV exports

---

## 🛠️ Requirements

| Requirement | Version |
|-------------|---------|
| **OS** | Windows 10/11 |
| **PowerShell** | 5.1 or higher |
| **.NET Framework** | 4.5 or higher |

> **Note:** All requirements are pre-installed on modern Windows systems.

---

## 🔧 Troubleshooting

### EXE won't start
- Ensure Windows Defender isn't blocking it
- Right-click → Properties → Unblock

### Data not saving
- Check if `data/` folder exists
- Verify write permissions

### Export not working
- Ensure you're logged in
- Check if `data/` folder is writable

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| [User Guide](docs/USER_GUIDE.md) | Step-by-step usage instructions |
| [User Manual](docs/USER_MANUAL.md) | Complete feature documentation |
| [Architecture](docs/ARCHITECTURE.md) | System design overview |
| [API Documentation](docs/API_DOCUMENTATION.md) | Backend API reference |
| [Deployment Guide](docs/DEPLOYMENT_GUIDE.md) | Deployment instructions |
| [FAQ](docs/FAQ.md) | Frequently asked questions |

---

## 🤝 Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

---

## 📧 Support

- **Email:** [raj183576@gmail.com](mailto:raj183576@gmail.com)
- **GitHub Issues:** [Create Issue](https://github.com/ajay-EY-1859/campus/issues)

---

## 📄 License

This project is licensed under the MIT License - see the [license.md](license.md) file for details.

---

## 👨‍💻 Author

**Ajay Kumar**  
- GitHub: [@ajay-EY-1859](https://github.com/ajay-EY-1859)
- Email: raj183576@gmail.com

---

<p align="center">
  Made with ❤️ for educational and institutional management
</p>