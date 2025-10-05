# Campus Management System v2.0

[![Build Test](https://github.com/ajay-EY-1859/campus/actions/workflows/build-test.yml/badge.svg)](https://github.com/ajay-EY-1859/campus/actions/workflows/build-test.yml)
[![CI](https://github.com/ajay-EY-1859/campus/actions/workflows/ci.yml/badge.svg)](https://github.com/ajay-EY-1859/campus/actions/workflows/ci.yml)

A comprehensive, secure, and modular C-based command-line application for managing multiple types of campuses including Schools, Colleges, Hospitals, and Hostels.

## Features

### Core Functionality

- **Multi-Campus Support**: School, College, Hospital, Hostel management
- **Secure Authentication**: Two-factor authentication with OTP
- **User Management**: Registration, login, profile management
- **User ID Recovery**: Forgot User ID feature with mobile/email search
- **Profile Export**: Multiple format export (PDF, TXT, CSV)
- **Duplicate Prevention**: Prevents multiple registrations with same email/mobile
- **Data Management**: Campus-specific data entry and retrieval
- **PDF Reports**: Professional report generation using Libharu
- **Database Integration**: File-based database with backup/restore
- **Session Management**: Secure sessions with timeout
- **Audit Logging**: Comprehensive activity tracking
- **Hidden Features**: (For developers) Special commands for quick application exit (see code comments)

### Security Features

- Two-Factor Authentication (2FA) with OTP
- Account lockout after failed attempts
- Password strength validation
- Session management with timeout
- Suspicious activity detection
- Security event logging
- Data encryption utilities

### Campus-Specific Features

#### School Management

- Student registration and profiles
- Subject-wise marks management
- Grade calculation (A+, A, B, C, D, F)
- Report cards with percentage

#### College Management

- Course registration system
- Credit-based marking system
- CGPA calculation
- Academic transcripts

#### Hospital Management

- Patient registration
- Medical data tracking
- Health records management
- Medical reports

#### Hostel Management

- Resident registration
- Room allocation tracking
- Accommodation details
- Hostel reports

## Quick Start

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

## Usage Guide

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

### Main Menu Options

1. **Register New User**: Create new account with duplicate prevention
2. **Login**: Secure 2FA login with OTP verification
3. **Forgot User ID**: Recover User ID using mobile/email with export option
4. **Exit**: Close application

### Dashboard Options

- **View Profile**: See your complete profile information
- **Edit Profile**: Update your personal details
- **Add Data**: Enter campus-specific data (marks, medical info, etc.)
- **View Data**: Display your stored data with calculations
- **Change Password**: Update your password securely
- **Export Report**: Generate campus-specific PDF reports
- **Export Profile**: Export profile in PDF/TXT/CSV formats
- **Logout**: Return to main menu
- **Exit Application**: Close the program

## Project Structure

```text
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
├── lib/                    # Library files (Libharu, curl, and dependencies)
│   ├── hpdf.dll           # Libharu dynamic library
│   ├── hpdf.exp           # Export file
│   ├── hpdf.lib           # Static library
│   ├── libcurl.*          # Curl libraries
│   └── ...                # Other third-party libraries
├── src/                   # Source files (modular)
│   ├── main/              # Core logic (auth, db, UI, etc.)
│   ├── student/           # Student/campus logic
│   ├── admin/             # Admin/user management
│   ├── api/               # API logic
│   ├── core/              # Core utilities
│   └── tests/             # Test code
├── docs/                  # Documentation
├── scripts/               # Utility scripts
├── CMakeLists.txt         # Build configuration
├── README.md              # This file
├── CONTRIBUTING.md        # Contribution guidelines
├── ROADMAP.md             # Development roadmap
└── LICENSE.md             # License information (file is named 'license.md', consider renaming to 'LICENSE.md' for convention)
```

## Configuration

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

## Data Storage

### File Structure

```text
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

## Development

### CI/CD Pipeline

The project uses GitHub Actions for continuous integration:

- **Windows Build**: MSVC with bundled libharu
- **Ubuntu Build**: GCC with system libharu
- **Automated Testing**: Build verification and artifact upload
- **Code Quality**: Static analysis with cppcheck

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

### Platform-Specific Notes

**Windows:**

- Uses bundled libharu from `lib/` directory
- Requires MSVC or MinGW
- DLL automatically copied to output

**Linux:**

- Requires system libharu: `sudo apt-get install libhpdf-dev`
- Uses pkg-config for library detection
- GCC recommended

### Adding New Campus Types

1. Update `CampusType` enum in `config.h`
2. Add campus-specific functions in `student.c`
3. Update UI options in `ui.c`
4. Add PDF export functionality
5. Update documentation

### Advanced Suggestions for Future Contributors

- Add REST API endpoints for web/mobile integration
- Implement a web dashboard (React, Vue, or Svelte)
- Add SQLite/Postgres support for scalable data
- Integrate AI/ML for analytics and predictions
- Add Docker support for easy deployment
- Implement plugin/module system for extensibility
- Add automated test coverage and CI/CD badges
- Improve accessibility and internationalization

## Troubleshooting

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

## Error Codes

- `0`: Success
- `1`: General error
- `2`: Authentication failure
- `3`: Database error
- `4`: File I/O error

## Performance

### System Requirements

- **RAM**: Minimum 512MB, Recommended 1GB
- **Storage**: 50MB for application, additional space for data
- **CPU**: Any modern processor (x86/x64)

### Optimization Tips

- Regular database cleanup
- Monitor log file sizes
- Use strong passwords to avoid lockouts
- Regular data backups

## Security Best Practices

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

## Support

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

### Contact Support

- **Email:** [raj183576@gmail.com](mailto:raj183576@gmail.com)
- **GitHub Issues:** [https://github.com/ajay-EY-1859/campus/issues](https://github.com/ajay-EY-1859/campus/issues)

## License

This project is licensed under the MIT License - see the [license.md](license.md) file for details. (Consider renaming to LICENSE.md)

## Contributors

- **Ajay Kumar** - Initial development and architecture
- (Contributors file not present. Please add a CONTRIBUTORS.md for a full list.)

## Acknowledgments

- [Libharu](https://github.com/libharu/libharu) for PDF generation
- CMake community for build system
- All contributors and testers

## Additional Resources

- [API Documentation](docs/API_DOCUMENTATION.md)
- [Architecture Guide](docs/ARCHITECTURE.md)
- [Deployment Guide](docs/DEPLOYMENT_GUIDE.md)
- [User Guide](docs/USER_GUIDE.md)
- [User Manual](docs/USER_MANUAL.md)

---

_Made with care for educational and institutional management_

---

**Missing Files/Notes:**

- No `SECURITY.md` file (consider adding for GitHub security policy compliance)
- No `CONTRIBUTORS.md` file (add to list all contributors)