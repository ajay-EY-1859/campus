# Campus Management System

## Project Overview
This is a comprehensive C-based Campus Management System with a **GTK4 Desktop GUI** that supports multiple campus types including Schools, Colleges, Hospitals, and Hostels. The system features user authentication, data management, PDF report generation, and secure file-based database operations.

## Current Status
- **Last Updated**: December 14, 2025
- **Status**: Full GTK4 Desktop GUI Application implemented
- **Build System**: CMake with GTK4
- **Running Version**: Desktop GUI at `gui/build/campus_gui`
- **Display**: VNC output for desktop GUI

## Architecture
- **Language**: C (C11 standard)
- **Dependencies**: 
  - libharu for PDF generation
  - CMake for build system
  - Standard C libraries
- **Project Structure**:
  - `src/main/` - Main source files
  - `include/` - Header files
  - `lib/` - External libraries (Windows DLLs)
  - `data/` - Database and user data storage
  - `logs/` - Application logs
  - `credentials/` - User credential storage

## Build Configuration
The original project had some compilation conflicts between header declarations and implementations. For the Replit environment, we created a working demo version:

- **Working Executable**: `simple_campus` (compiled from `simple_main.c`)
- **Workflow**: Console application workflow configured
- **Output**: Demonstrates core functionality and menu system

## Key Features
- Multi-campus support (School, College, Hospital, Hostel)
- User registration and authentication system
- Two-factor authentication with OTP
- PDF report generation capability
- File-based database system
- Security features including login attempt tracking
- Audit logging system

## Development Notes
- Original CMake build has type conflicts that need resolution
- Created simplified working version for immediate functionality
- Full feature implementation requires fixing function signature mismatches
- All required directories (data, credentials, logs) have been created
- libharu PDF library successfully installed and available

## User Preferences
- Console-based application (not web-based)
- Prefers working solutions over broken complex implementations
- Focus on functionality over perfect code structure initially

## Recent Changes
- **2025-09-18**: Initial import and setup
- Installed C/C++ toolchain and CMake
- Installed libharu PDF generation library
- Created required directory structure
- Built working demo version
- Configured workflow for console output
- Verified application runs successfully