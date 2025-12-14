# Campus Management System

## Project Overview
This is a comprehensive C-based Campus Management System with a **GTK4 Desktop GUI** that supports multiple campus types including Schools, Colleges, Hospitals, and Hostels. The system features user authentication, data management, report generation, and secure file-based database operations.

## Current Status
- **Last Updated**: December 14, 2025
- **Status**: Full GTK4 Desktop GUI Application - COMPLETE
- **Build System**: CMake with GTK4
- **Running Version**: Desktop GUI at `gui/build/campus_gui`
- **Display**: VNC output for desktop GUI

## Architecture
- **Language**: C (C11 standard)
- **GUI Framework**: GTK4
- **Dependencies**: 
  - GTK4 for desktop GUI
  - libharu for PDF generation (available)
  - CMake for build system
  - Standard C libraries
- **Project Structure**:
  - `gui/` - GTK4 GUI application
    - `campus_gui.c` - Main application with all UI pages
    - `core/services.c` - Backend logic and data operations
    - `core/services.h` - Header with data structures
    - `build/` - Compiled executable
    - `CMakeLists.txt` - Build configuration
  - `src/main/` - Original source files (legacy)
  - `include/` - Header files (legacy)
  - `data/` - Database and user data storage
  - `logs/` - Application logs
  - `credentials/` - User credential storage

## Build Commands
```bash
cd gui && mkdir -p build && cd build && cmake .. && make
./gui/build/campus_gui
```

## Key Features (Implemented)
1. **User Authentication**
   - Login with User ID, mobile, and password
   - Registration with full validation
   - Password hashing and session management

2. **Multi-Campus Support**
   - School: Subject grades, marks, percentage, grade calculation
   - College: Course name, credits, GPA/CGPA
   - Hospital: Patient records, diagnosis, treatment
   - Hostel: Room allocation, block, floor, rent

3. **Data Management**
   - Add Data: Campus-specific entry forms
   - View Data: Display saved records
   - File-based storage per user

4. **Report Export**
   - Export user data to text file format
   - Includes all campus-specific information

5. **Activity Logging**
   - Login/logout tracking
   - Data save operations
   - Report exports

## Workflow Configuration
- **Name**: Campus Management System
- **Command**: `export DBUS_SESSION_BUS_ADDRESS=disabled; ./gui/build/campus_gui`
- **Output**: VNC display for GTK4 desktop application

## User Preferences
- Desktop GUI application (GTK4)
- VNC output for visualization
- File-based database storage
- Multi-campus type support

## Recent Changes
- **2025-12-14**: Implemented complete data management system
  - Added data entry forms for all 4 campus types
  - Added view data functionality
  - Added report export feature
  - Connected all dashboard quick action buttons
  - Added navigation between pages
