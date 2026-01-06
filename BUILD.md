# Campus Management System - Build Guide

## Quick Build

### One-Command Build
```powershell
cd d:\campus
.\build.ps1
```

This will:
1. Install `ps2exe` module (if not installed)
2. Convert `CampusGUI.ps1` to `CampusGUI.exe`
3. Create a standalone Windows executable

---

## Build Options

```powershell
# Install ps2exe module only
.\build.ps1 -Install

# Build EXE only (default)
.\build.ps1 -Build

# Build and Run
.\build.ps1 -Build -Run

# Just Run (if already built)
.\build.ps1 -Run
```

---

## Manual Build Steps

### Step 1: Install ps2exe
```powershell
Install-Module -Name ps2exe -Scope CurrentUser -Force
```

### Step 2: Build EXE
```powershell
Invoke-ps2exe -InputFile "CampusGUI.ps1" -OutputFile "CampusGUI.exe" -NoConsole -Title "Campus Management System"
```

### Step 3: Run
```powershell
.\CampusGUI.exe
```
Or simply double-click `CampusGUI.exe`

---

## Output Files

| File | Description |
|------|-------------|
| `CampusGUI.exe` | Main application executable |
| `data\` folder | User data (JSON files) - created automatically |

---

## Requirements

- Windows 10/11
- PowerShell 5.1 or higher
- Internet connection (for first-time module installation)

---

## Distribution

To distribute the application:

1. Build the EXE: `.\build.ps1`
2. Copy these files to target computer:
   - `CampusGUI.exe`
   - `data\` folder (optional, for existing data)

The EXE is standalone and does not require PowerShell to be visible.

---

## Troubleshooting

### "Execution Policy" Error
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### "ps2exe not found" Error
```powershell
Install-Module -Name ps2exe -Scope CurrentUser -Force
```

### Antivirus Warning
Some antivirus software may flag ps2exe-generated files. This is a false positive. Add the EXE to your antivirus whitelist.
