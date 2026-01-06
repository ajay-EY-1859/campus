# Campus Management System - GUI Documentation

## Features Implemented ✅

### 1. **Authentication System**
- **Secure Login Page**
  - User ID, Mobile, and Password authentication
  - Real-time status messages
  - Automatic field clearing on successful login
  - Link to registration page

- **User Registration**
  - Full name, email, mobile, and password entry
  - Password confirmation validation
  - Campus type selection
  - Error handling for duplicate users

### 2. **Dashboard**
- **Welcome Section** with user greeting
- **Quick Actions**:
  - Add New Record
  - View Records  
  - Profile Settings
  - Logout

### 3. **Data Management**
- **Add New Record Page**
  - Dynamic form for entering data
  - Support for all campus types
  - Status indicators
  - Save functionality

- **View Records Page**
  - Search records
  - Record filtering
  - Display records in list format
  - Edit/Delete options

### 4. **User Profile**
- View profile information
- Change password option
- Campus type display
- User statistics

## Campus Types Supported

1. **School** - Student records with marks and subjects
2. **College** - Enrollment, courses, GPA tracking
3. **Hospital** - Patient management, diagnosis, treatment
4. **Hostel** - Room allocation, rent management, check-in tracking

## Architecture

```
┌─────────────────────────────────────────┐
│         GTK4 User Interface             │
├─────────────────────────────────────────┤
│  Login → Register → Dashboard → Manage Data
├─────────────────────────────────────────┤
│  Backend: Core Campus Logic             │
│  - Authentication Module                │
│  - Database Layer                       │
│  - Security & Encryption                │
│  - File I/O & Logging                   │
├─────────────────────────────────────────┤
│  Storage: SQLite + File-based DB        │
│  - User Credentials & Sessions          │
│  - Campus Data                          │
│  - Audit Logs                           │
└─────────────────────────────────────────┘
```

## Build & Run

### Windows:
```bash
cd d:\campus\build
mingw32-make
gui\campus_gui.exe
```

### Linux:
```bash
cd /campus/build
cmake ..
make
./gui/campus_gui
```

## Pages & Navigation

### 1. Login Page (`login`)
- User authentication
- Register link
- Status messages

### 2. Register Page (`register`)
- New user account creation
- Campus type selection
- Back to login button

### 3. Dashboard (`dashboard`)
- User welcome message
- Quick action buttons
- Navigation hub

### 4. Add Data (`data_entry`)
- Form-based data entry
- Campus-type specific fields
- Save & return option

### 5. View Data (`view_data`)
- List existing records
- Search & filter
- Edit/Delete actions

### 6. Profile (`profile`)
- User information display
- Security settings
- Profile management

## Backend Integration

The GUI connects to core backend functions:

```c
// Authentication
authenticateUser(userID, mobile, password)
createUser(profile)
getUserByID(userID, profile)

// Data Management
saveSchoolData(data)
saveCollegeData(data)
saveHospitalData(data)
saveHostelData(data)

// File Operations
loadUserData(userID)
exportToFile(data, format)
```

## Data Flow

### Login Flow:
```
User Input → Validation → authenticateUser() → getUserByID() → Dashboard
```

### Registration Flow:
```
User Input → Validation → createUser() → Success Message → Login Page
```

### Data Entry Flow:
```
Form Input → Validation → Format Data → Save to Database → Status Message
```

### View/Export Flow:
```
Database Query → List View → Search/Filter → Export Options
```

## Security Features

- ✅ Password hashing (SHA256)
- ✅ Input validation
- ✅ Session management
- ✅ Audit logging
- ✅ File-based encryption for credentials
- ✅ Account lockout after failed attempts
- ✅ Secure password policies

## User Experience Features

- 🎨 Modern GTK4 interface
- 📱 Responsive design
- ✨ Smooth page transitions (300ms)
- 💬 Real-time status messages
- 🔄 Form auto-clearing on success
- 🎯 Intuitive navigation
- 📊 Professional styling with CSS classes

## File Locations

```
d:\campus\
├── gui/
│   ├── campus_gui_main.c      (Main GUI code - 530 lines)
│   ├── CMakeLists.txt         (Build configuration)
│   └── campus_gui.exe         (Compiled executable)
├── src/
│   ├── main/                  (Core logic)
│   ├── student/               (Student management)
│   └── admin/user/            (User management)
├── include/                   (Headers)
└── build/gui/                 (Build output)
```

## Key Structures

```c
typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *stack;           // Page switcher
    
    // Page references
    GtkWidget *login_page;
    GtkWidget *register_page;
    GtkWidget *dashboard_page;
    GtkWidget *data_entry_page;
    GtkWidget *view_data_page;
    GtkWidget *profile_page;
    
    // User state
    Profile current_user;
    int is_logged_in;
} AppData;
```

## Future Enhancements

- [ ] 2FA/OTP support in GUI
- [ ] PDF export from dashboard
- [ ] Data visualization & charts
- [ ] Advanced filtering & sorting
- [ ] Batch operations
- [ ] User role-based access
- [ ] Multi-language support
- [ ] Dark mode theme
- [ ] Mobile-responsive design
- [ ] Real-time notifications

## Testing

### Test Scenarios:
1. **Login**: Valid credentials → Dashboard
2. **Register**: New user → Success → Login
3. **Add Data**: Fill form → Save → Status message
4. **View Data**: List records → Search → Filter
5. **Navigation**: All page transitions work
6. **Error Handling**: Invalid input → Error messages

## Dependencies

- **GTK 4.0+** - UI Framework
- **GLib 2.0** - Core utilities
- **SQLite 3** - Database
- **MinGW/GCC** - C Compiler
- **CMake 3.10+** - Build system

## Compilation

```bash
# Configuration
cmake -B build -G "MinGW Makefiles"

# Build
cmake --build build

# Run
build\gui\campus_gui.exe
```

## Notes

- GUI starts on login page
- User must authenticate before accessing other pages
- All data is validated before saving
- Error messages provide clear feedback
- Session management ensures secure logout
- Page transitions are smooth and responsive

---

**Status**: ✅ Fully Functional
**Lines of Code**: 530
**Built with**: GTK4, C, CMake
**Platform**: Windows (MinGW) & Linux (GCC)
