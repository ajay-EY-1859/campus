# Installation Guide - Campus Management System

This guide provides detailed installation instructions for the Campus Management System on different platforms.

## System Requirements

### Minimum Requirements
- **Operating System**: Windows 10/11, Ubuntu 18.04+, macOS 10.14+
- **RAM**: 512 MB
- **Storage**: 100 MB free space
- **Processor**: Any modern x86/x64 processor

### Recommended Requirements
- **RAM**: 1 GB or more
- **Storage**: 500 MB free space (for data and reports)
- **Network**: Internet connection for OTP delivery simulation

## Prerequisites

### Windows
1. **Visual Studio 2019/2022** (Community Edition is sufficient)
   - Download from: https://visualstudio.microsoft.com/
   - Install with "Desktop development with C++" workload

2. **CMake 3.10 or higher**
   - Download from: https://cmake.org/download/
   - Add to system PATH during installation

3. **Git** (optional, for cloning repository)
   - Download from: https://git-scm.com/

### Linux (Ubuntu/Debian)
```bash
# Update package list
sudo apt update

# Install build essentials
sudo apt install build-essential cmake git

# Install additional dependencies
sudo apt install libssl-dev
```

### Linux (CentOS/RHEL/Fedora)
```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake git

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake git
```

### macOS
1. **Xcode Command Line Tools**
   ```bash
   xcode-select --install
   ```

2. **CMake** (using Homebrew)
   ```bash
   # Install Homebrew if not already installed
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   
   # Install CMake
   brew install cmake
   ```

## Installation Methods

### Method 1: Download Release (Recommended)
1. Go to the [Releases page](https://github.com/ajay-EY-1859/campus/releases)
2. Download the latest release for your platform
3. Extract the archive to your desired location
4. Follow the "Running the Application" section below

### Method 2: Build from Source

#### Step 1: Clone Repository
```bash
# Using HTTPS
git clone https://github.com/ajay-EY-1859/campus.git

# Using SSH (if configured)
git clone git@github.com:ajay-EY-1859/campus.git

# Navigate to project directory
cd campus
```

#### Step 2: Build the Project

**Windows (Command Prompt/PowerShell)**
```cmd
# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build . --config Release

# Or for debug build
cmake --build . --config Debug
```

**Linux/macOS**
```bash
# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project
make -j$(nproc)

# Or using cmake
cmake --build . -j$(nproc)
```

#### Step 3: Verify Installation
```bash
# Windows
.\build\Release\campus.exe --version

# Linux/macOS
./build/campus --version
```

## Running the Application

### Windows
```cmd
# Navigate to build directory
cd build\Release

# Run the application
campus.exe
```

### Linux/macOS
```bash
# Navigate to build directory
cd build

# Run the application
./campus
```

## Directory Structure After Installation

```
campus/
├── build/                  # Build output directory
│   ├── Release/           # Release binaries (Windows)
│   │   ├── campus.exe     # Main executable
│   │   └── hpdf.dll       # PDF library
│   └── campus             # Main executable (Linux/macOS)
├── data/                  # Data directory (created at runtime)
│   ├── campus.db          # Database file
│   ├── audit.log          # Audit log
│   └── *.pdf              # Generated reports
├── credentials/           # User profiles (created at runtime)
├── logs/                  # Application logs (created at runtime)
└── include/               # Header files
```

## Configuration

### Environment Variables (Optional)
```bash
# Set custom data directory
export CAMPUS_DATA_DIR="/path/to/custom/data"

# Set log level
export CAMPUS_LOG_LEVEL="DEBUG"

# Set session timeout (in seconds)
export CAMPUS_SESSION_TIMEOUT="1800"
```

### Configuration File (Optional)
Create `config.ini` in the application directory:
```ini
[database]
path=data/campus.db
backup_interval=3600

[security]
session_timeout=1800
max_login_attempts=3
account_lock_duration=900

[logging]
level=INFO
audit_enabled=true
security_log_enabled=true
```

## Troubleshooting

### Common Build Issues

#### Windows: "CMake not found"
```cmd
# Add CMake to PATH or use full path
"C:\Program Files\CMake\bin\cmake.exe" ..
```

#### Windows: "MSVC not found"
- Install Visual Studio with C++ development tools
- Or use Visual Studio Developer Command Prompt

#### Linux: "cmake: command not found"
```bash
# Ubuntu/Debian
sudo apt install cmake

# CentOS/RHEL
sudo yum install cmake

# Fedora
sudo dnf install cmake
```

#### macOS: "No such file or directory: 'cmake'"
```bash
# Install using Homebrew
brew install cmake

# Or download from cmake.org
```

### Runtime Issues

#### "Failed to initialize database"
- Check write permissions in application directory
- Ensure sufficient disk space
- Create data directory manually if needed:
  ```bash
  mkdir data credentials logs
  ```

#### "Cannot open file for writing"
- Check file permissions
- Ensure no antivirus blocking
- Run as administrator (Windows) or with sudo (Linux) if needed

#### "DLL not found" (Windows)
- Ensure `hpdf.dll` is in the same directory as `campus.exe`
- Install Visual C++ Redistributable if needed

### Performance Issues

#### Slow startup
- Check antivirus real-time scanning
- Ensure SSD storage for better performance
- Close unnecessary background applications

#### High memory usage
- This is normal for the current file-based implementation
- Future versions will include memory optimizations

## Updating

### From Release
1. Download new release
2. Backup your data directory
3. Replace executable files
4. Keep your data, credentials, and logs directories

### From Source
```bash
# Pull latest changes
git pull origin main

# Clean previous build
rm -rf build

# Rebuild
mkdir build && cd build
cmake ..
cmake --build .
```

## Uninstallation

### Complete Removal
```bash
# Remove application directory
rm -rf /path/to/campus

# Remove data (if desired)
rm -rf ~/.campus  # Linux/macOS
# or manually delete data folder on Windows
```

### Keep Data Only
```bash
# Backup data first
cp -r data/ ~/campus-backup/

# Remove application
rm -rf campus/

# Data can be used with future installations
```

### Installation Help
If you encounter issues during installation:

1. **Check Prerequisites**: Ensure all required software is installed
2. **Review Error Messages**: Look for specific error details
3. **Check Permissions**: Ensure proper file/directory permissions
4. **Search Issues**: Check GitHub issues for similar problems
5. **Create Issue**: If problem persists, create a detailed issue report

### Issue Report Template
```markdown
**Installation Issue**

**Environment:**
- OS: [Windows 10/Ubuntu 20.04/macOS 11.0]
- Compiler: [MSVC 2022/GCC 9.4/Clang 12.0]
- CMake Version: [3.20.0]

**Steps Taken:**
1. [List installation steps]

**Error Message:**
```
[Paste exact error message]
```

**Additional Context:**
[Any other relevant information]
```

### Contact
- **GitHub Issues**: https://github.com/ajay-EY-1859/campus/issues
- **Email**: raj183576@gmail.com

---
```
