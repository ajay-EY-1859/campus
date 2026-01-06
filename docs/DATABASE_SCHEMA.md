# Campus Management System - SQLite Database Schema

## Overview
The application now uses SQLite (System.Data.SQLite) for persistent data storage instead of JSON files. All user data, records, and audit trails are stored in a structured relational database.

**Database File**: `data/campus_system.db`

---

## Database Tables

### 1. Users Table
Stores user account information and authentication details.

```sql
CREATE TABLE Users (
    UserID TEXT PRIMARY KEY,
    Name TEXT NOT NULL,
    Email TEXT NOT NULL UNIQUE,
    Mobile TEXT NOT NULL UNIQUE,
    PasswordHash TEXT NOT NULL,
    CampusType TEXT NOT NULL,
    CreatedDate DATETIME DEFAULT CURRENT_TIMESTAMP,
    LastLogin DATETIME,
    IsActive INTEGER DEFAULT 1
)
```

**Fields:**
- `UserID` (TEXT, PK): Unique user identifier (format: `USERID_XXXXXXXX`)
- `Name` (TEXT): Full name of the user
- `Email` (TEXT, UNIQUE): User's email address
- `Mobile` (TEXT, UNIQUE): User's mobile number
- `PasswordHash` (TEXT): SHA256 hashed password
- `CampusType` (TEXT): Campus type (School, College, Hospital, Hostel)
- `CreatedDate` (DATETIME): Account creation timestamp
- `LastLogin` (DATETIME): Last login timestamp
- `IsActive` (INTEGER): 1 if active, 0 if inactive

**Indexes:**
- Primary Key: `UserID`
- Unique: `Email`, `Mobile`

---

### 2. Records Table
Stores all campus-specific records for each user.

```sql
CREATE TABLE Records (
    RecordID INTEGER PRIMARY KEY AUTOINCREMENT,
    UserID TEXT NOT NULL,
    CampusType TEXT NOT NULL,
    RecordDate DATETIME DEFAULT CURRENT_TIMESTAMP,
    OverallPercentage REAL,
    PatientName TEXT,
    Diagnosis TEXT,
    Treatment TEXT,
    RoomNumber TEXT,
    Floor TEXT,
    RentAmount REAL,
    CourseName TEXT,
    Credits INTEGER,
    Grade TEXT,
    CreatedDate DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE
)
```

**Fields:**
- `RecordID` (INTEGER, PK): Auto-incrementing unique record identifier
- `UserID` (TEXT, FK): Reference to user who owns this record
- `CampusType` (TEXT): Type of campus (School, College, Hospital, Hostel)
- `RecordDate` (DATETIME): Date when record was created
- `OverallPercentage` (REAL): Overall percentage (School only)
- `PatientName` (TEXT): Patient name (Hospital only)
- `Diagnosis` (TEXT): Medical diagnosis (Hospital only)
- `Treatment` (TEXT): Treatment details (Hospital only)
- `RoomNumber` (TEXT): Room number (Hostel only)
- `Floor` (TEXT): Floor number (Hostel only)
- `RentAmount` (REAL): Rent amount (Hostel only)
- `CourseName` (TEXT): Course name (College only)
- `Credits` (INTEGER): Credit points (College only)
- `Grade` (TEXT): Grade (College only)
- `CreatedDate` (DATETIME): Server-side creation timestamp

**Constraints:**
- Foreign Key: `UserID` → `Users(UserID)` with CASCADE delete

**Indexes:**
- Primary Key: `RecordID`
- Recommended: `UserID` (for faster queries)

---

### 3. Subjects Table
Stores individual subject marks for School records.

```sql
CREATE TABLE Subjects (
    SubjectID INTEGER PRIMARY KEY AUTOINCREMENT,
    RecordID INTEGER NOT NULL,
    SubjectName TEXT NOT NULL,
    MarksObtained INTEGER NOT NULL,
    TotalMarks INTEGER NOT NULL,
    Percentage REAL NOT NULL,
    CreatedDate DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (RecordID) REFERENCES Records(RecordID) ON DELETE CASCADE
)
```

**Fields:**
- `SubjectID` (INTEGER, PK): Auto-incrementing unique subject identifier
- `RecordID` (INTEGER, FK): Reference to the parent School record
- `SubjectName` (TEXT): Name of the subject (e.g., "Mathematics", "Physics")
- `MarksObtained` (INTEGER): Marks scored by student
- `TotalMarks` (INTEGER): Maximum marks for the subject
- `Percentage` (REAL): Calculated percentage (MarksObtained/TotalMarks * 100)
- `CreatedDate` (DATETIME): Creation timestamp

**Constraints:**
- Foreign Key: `RecordID` → `Records(RecordID)` with CASCADE delete

**Indexes:**
- Primary Key: `SubjectID`
- Recommended: `RecordID` (for faster subject queries)

---

### 4. AuditLog Table
Stores all user actions for security and auditing purposes.

```sql
CREATE TABLE AuditLog (
    LogID INTEGER PRIMARY KEY AUTOINCREMENT,
    UserID TEXT,
    Action TEXT NOT NULL,
    Details TEXT,
    IPAddress TEXT,
    CreatedDate DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE SET NULL
)
```

**Fields:**
- `LogID` (INTEGER, PK): Auto-incrementing log entry identifier
- `UserID` (TEXT, FK): User who performed the action (can be NULL for system actions)
- `Action` (TEXT): Action type (e.g., "USER_REGISTERED", "RECORD_CREATED", "USER_DATA_SAVED")
- `Details` (TEXT): Additional details about the action
- `IPAddress` (TEXT): IP address (for future expansion)
- `CreatedDate` (DATETIME): When the action occurred

**Constraints:**
- Foreign Key: `UserID` → `Users(UserID)` with SET NULL on delete

**Indexes:**
- Primary Key: `LogID`
- Recommended: `UserID`, `Action`, `CreatedDate` (for audit queries)

---

## Data Types Mapping

| PowerShell Type | SQL Type | Notes |
|---|---|---|
| String | TEXT | Unicode text |
| Integer | INTEGER | Whole numbers |
| Decimal | REAL | Floating point numbers |
| DateTime | DATETIME | ISO 8601 format |
| Boolean | INTEGER | 0 or 1 |
| Null | NULL | Database NULL |

---

## Query Examples

### Get All User Records
```sql
SELECT * FROM Records WHERE UserID = 'USERID_ABC123' ORDER BY RecordDate DESC;
```

### Get School Record with Subjects
```sql
SELECT r.*, COUNT(s.SubjectID) as SubjectCount
FROM Records r
LEFT JOIN Subjects s ON r.RecordID = s.RecordID
WHERE r.UserID = 'USERID_ABC123' AND r.CampusType = 'School'
GROUP BY r.RecordID;
```

### Get Subjects for a Specific Record
```sql
SELECT SubjectName, MarksObtained, TotalMarks, Percentage
FROM Subjects
WHERE RecordID = 1
ORDER BY SubjectID;
```

### Get User Login Audit
```sql
SELECT UserID, Action, Details, CreatedDate
FROM AuditLog
WHERE UserID = 'USERID_ABC123' AND Action = 'LOGIN'
ORDER BY CreatedDate DESC
LIMIT 10;
```

### Calculate Overall Statistics (School)
```sql
SELECT 
    r.RecordID,
    r.RecordDate,
    r.OverallPercentage,
    COUNT(s.SubjectID) as TotalSubjects,
    AVG(s.Percentage) as AvgSubjectPercentage
FROM Records r
LEFT JOIN Subjects s ON r.RecordID = s.RecordID
WHERE r.UserID = 'USERID_ABC123' AND r.CampusType = 'School'
GROUP BY r.RecordID;
```

---

## Cascading Deletes

- **User Deletion**: When a user is deleted, all their records and subjects are automatically deleted
- **Record Deletion**: When a record is deleted, all associated subjects are automatically deleted
- **Audit Log**: When a user is deleted, audit entries are preserved with NULL UserID for historical tracking

---

## Performance Considerations

1. **Indexes**: Primary keys are indexed by default. Consider adding indexes on:
   - `Records.UserID` (foreign key)
   - `Subjects.RecordID` (foreign key)
   - `AuditLog.UserID` and `AuditLog.CreatedDate`

2. **Unique Constraints**: Email and Mobile must be unique across all users

3. **Connection Pooling**: SQLite supports connection pooling with max 100 connections configured

4. **Concurrent Access**: SQLite supports concurrent reads but serializes writes. GUI application is single-user per session

---

## Backup and Recovery

The database file can be backed up like any other file:
```powershell
# Backup database
Copy-Item "$DataFolder\campus_system.db" "$DataFolder\campus_system_backup.db"

# Restore from backup
Copy-Item "$DataFolder\campus_system_backup.db" "$DataFolder\campus_system.db"
```

---

## SQL Functions Used in PowerShell GUI

### Execute-Query
Executes INSERT, UPDATE, DELETE operations
```powershell
Execute-Query -Query "UPDATE Users SET LastLogin = CURRENT_TIMESTAMP WHERE UserID = @UserID" -Parameters @{ UserID = "USERID_123" }
```

### Get-QueryResult
Retrieves multiple rows
```powershell
$records = Get-QueryResult -Query "SELECT * FROM Records WHERE UserID = @UserID" -Parameters @{ UserID = "USERID_123" }
```

### Get-ScalarResult
Retrieves single value
```powershell
$count = Get-ScalarResult -Query "SELECT COUNT(*) FROM Records WHERE UserID = @UserID" -Parameters @{ UserID = "USERID_123" }
```

---

## Migration from JSON

If you have existing JSON files:
1. Old JSON files are not automatically imported
2. Users need to re-register to use the new system
3. JSON files can be archived for historical reference
4. Database provides better performance and data integrity

---

## Future Enhancements

1. Add more audit triggers for data changes
2. Implement data encryption for sensitive fields
3. Add reporting queries and analytics
4. Implement database versioning/migrations
5. Add backup scheduling functionality
