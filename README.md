# Campus Management System v2.0

A comprehensive, secure, and modular C-based command-line application for managing multiple types of campuses including Schools, Colleges, Hospitals, and Hostels.

## 🌟 Features

### Core Functionality
- **Multi-Campus Support**: School, College, Hospital, Hostel management
- **Secure Authentication**: Two-factor authentication with OTP
- **User Management**: Registration, login, profile management
- **Data Management**: Campus-specific data entry and retrieval
- **PDF Reports**: Professional report generation using Libharu
- **Database Integration**: File-based database with backup/restore
- **Session Management**: Secure sessions with timeout
- **Audit Logging**: Comprehensive activity tracking

### Security Features
- 🔐 Two-Factor Authentication (2FA) with OTP
- 🛡️ Account lockout after failed attempts
- 🔑 Password strength validation
- 📊 Session management with timeout
- 🔍 Suspicious activity detection
- 📝 Security event logging
- 🔒 Data encryption utilities

### Campus-Specific Features

#### 🏫 School Management
- Student registration and profiles
- Subject-wise marks management
- Grade calculation (A+, A, B, C, D, F)
- Report cards with percentage

#### 🎓 College Management
- Course registration system
- Credit-based marking system
- CGPA calculation
- Academic transcripts

#### 🏥 Hospital Management
- Patient registration
- Medical data tracking
- Health records management
- Medical reports

#### 🏠 Hostel Management
- Resident registration
- Room allocation tracking
- Accommodation details
- Hostel reports

## 🚀 Quick Start

### Prerequisites
- Windows 10/11 or Linux
- CMake 3.10 or higher
- C Compiler (GCC/MSVC)
- Libharu library (included)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/ajay-EY-1859/campus.git
   cd campus
   ```

2. **Build the project**
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

3. **Run the application**
   ```bash
   # Windows
   .\build\Debug\campus.exe
   
   # Linux
   ./build/campus
   ```

## 📖 Usage Guide

### First Time Setup
1. Run the application
2. Select "1. Register New User"
3. Choose your campus type (School/College/Hospital/Hostel)
4. Fill in your details
5. Create a strong password (8+ chars, mixed case, numbers, symbols)

### Login Process
1. Select "2. Login"
2. Enter your User ID
3. Enter your mobile number
4. Enter your password
5. Enter the OTP sent to your mobile/email
6. Access your dashboard

### Dashboard Options
- **View Profile**: See your complete profile information
- **Edit Profile**: Update your personal details
- **Add Data**: Enter campus-specific data (marks, medical info, etc.)
- **View Data**: Display your stored data with calculations
- **Change Password**: Update your password securely
- **Export Report**: Generate PDF reports
- **Logout**: Return to main menu
- **Exit Application**: Close the program

## 🏗️ Project Structure

```
campus/
├── include/                 # Header files
│   ├── hpdf/               # Libharu PDF library headers
│   ├── auth.h              # Authentication functions
│   ├── config.h            # Configuration constants
│   ├── database.h          # Database operations
│   ├── fileio.h            # File I/O operations
│   ├── security.h          # Security features
│   ├── student.h           # Campus-specific functions
│   ├── ui.h                # User interface
│   └── utils.h             # Utility functions
├── lib/                    # Library files
│   ├── hpdf.dll           # Libharu dynamic library
│   ├── hpdf.exp           # Export file
│   └── hpdf.lib           # Static library
├── src/main/              # Source files
│   ├── auth.c             # Authentication implementation
│   ├── database.c         # Database operations
│   ├── fileio.c           # File operations
│   ├── main.c             # Main program entry
│   ├── security.c         # Security features
│   ├── signin.c           # Login functionality
│   ├── signup.c           # Registration functionality
│   ├── student.c          # Campus-specific logic
│   ├── ui.c               # User interface
│   └── utils.c            # Utility functions
├── docs/                  # Documentation
├── scripts/               # Utility scripts
├── CMakeLists.txt         # Build configuration
├── README.md              # This file
├── CONTRIBUTING.md        # Contribution guidelines
├── ROADMAP.md             # Development roadmap
└── LICENSE.md             # License information
```

## 🔧 Configuration

### Database Configuration
- Database files are stored in `data/` directory
- Automatic backup creation
- Configurable session timeout (default: 30 minutes)
- Account lockout duration (default: 15 minutes)

### Security Configuration
- OTP expiry time: 5 minutes
- Maximum login attempts: 3
- Password requirements: 8+ characters, mixed case, numbers, symbols
- Session timeout: 30 minutes

## 📊 Data Storage

### File Structure
```
data/
├── campus.db              # Main database file
├── audit.log              # Activity audit log
├── security.log           # Security events log
├── {userID}_data.dat      # User-specific data
├── {userID}_otp.dat       # OTP storage (temporary)
├── {userID}_attempts.dat  # Login attempts tracking
├── {userID}_lock.dat      # Account lock information
└── *.pdf                  # Generated reports
```

## 🛠️ Development

### Building from Source
```bash
# Debug build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### Adding New Campus Types
1. Update `CampusType` enum in `config.h`
2. Add campus-specific functions in `student.c`
3. Update UI options in `ui.c`
4. Add PDF export functionality
5. Update documentation

## 🔍 Troubleshooting

### Common Issues

**Build Errors**
- Ensure CMake 3.10+ is installed
- Check compiler compatibility
- Verify Libharu files are present

**Runtime Errors**
- Check file permissions for data directory
- Ensure sufficient disk space
- Verify all required DLLs are present

**Authentication Issues**
- Check if account is locked (wait 15 minutes)
- Verify OTP within 5-minute window
- Ensure strong password requirements are met

### Error Codes
- `0`: Success
- `1`: General error
- `2`: Authentication failure
- `3`: Database error
- `4`: File I/O error

## 📈 Performance

### System Requirements
- **RAM**: Minimum 512MB, Recommended 1GB
- **Storage**: 50MB for application, additional space for data
- **CPU**: Any modern processor (x86/x64)

### Optimization Tips
- Regular database cleanup
- Monitor log file sizes
- Use strong passwords to avoid lockouts
- Regular data backups

## 🔒 Security Best Practices

### For Users
- Use strong, unique passwords
- Don't share OTP codes
- Log out after use
- Report suspicious activity

### For Administrators
- Regular security audits
- Monitor failed login attempts
- Keep backups secure
- Update system regularly

## 📞 Support

### Getting Help
1. Check this README first
2. Review troubleshooting section
3. Check existing issues on GitHub
4. Create a new issue with detailed information

### Reporting Bugs
Please include:
- Operating system and version
- Steps to reproduce
- Expected vs actual behavior
- Error messages or logs
- Screenshots if applicable

## 📄 License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## 👥 Contributors

- **Ajay Kumar** - Initial development and architecture
- See [CONTRIBUTORS.md](CONTRIBUTORS.md) for full list

## 🙏 Acknowledgments

- [Libharu](https://github.com/libharu/libharu) for PDF generation
- CMake community for build system
- All contributors and testers

## 📚 Additional Resources

- [API Documentation](docs/API.md)
- [Architecture Guide](docs/ARCHITECTURE.md)
- [Security Guide](docs/SECURITY.md)
- [Deployment Guide](docs/DEPLOYMENT.md)

---

**Made with ❤️ for educational and institutional management**