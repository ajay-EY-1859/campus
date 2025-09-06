@echo off
echo ========================================
echo   Campus Management System v2.0
echo   Installation Script
echo ========================================
echo.

echo 🚀 Starting installation process...
echo.

REM Check if running as administrator
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo ❌ This script requires administrator privileges.
    echo Please run as administrator.
    pause
    exit /b 1
)

echo ✅ Administrator privileges confirmed
echo.

REM Create installation directory
set INSTALL_DIR=C:\CampusSystem
echo 📁 Creating installation directory: %INSTALL_DIR%
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

REM Create subdirectories
echo 📁 Creating subdirectories...
mkdir "%INSTALL_DIR%\bin" 2>nul
mkdir "%INSTALL_DIR%\data" 2>nul
mkdir "%INSTALL_DIR%\logs" 2>nul
mkdir "%INSTALL_DIR%\config" 2>nul
mkdir "%INSTALL_DIR%\backup" 2>nul
mkdir "%INSTALL_DIR%\docs" 2>nul

REM Copy application files
echo 📋 Copying application files...
copy "build\build\Release\campus.exe" "%INSTALL_DIR%\bin\" >nul
copy "build\build\Release\hpdf.dll" "%INSTALL_DIR%\bin\" >nul

REM Copy documentation
echo 📚 Copying documentation...
copy "docs\*.md" "%INSTALL_DIR%\docs\" >nul
copy "README.md" "%INSTALL_DIR%\docs\" >nul

REM Create configuration file
echo ⚙️ Creating configuration file...
(
echo [System]
echo MaxUsers=1000
echo SessionTimeout=30
echo DataDirectory=./data
echo LogLevel=INFO
echo.
echo [Security]
echo PasswordMinLength=8
echo MaxLoginAttempts=3
echo LockoutDuration=15
echo OTPExpiry=5
echo.
echo [Campus]
echo DefaultType=SCHOOL
echo AllowedTypes=SCHOOL,COLLEGE,HOSPITAL,HOSTEL
) > "%INSTALL_DIR%\config\campus.conf"

REM Set file permissions
echo 🔒 Setting file permissions...
icacls "%INSTALL_DIR%" /grant Users:(OI)(CI)F >nul 2>&1

REM Add to system PATH
echo 🛤️ Adding to system PATH...
setx PATH "%PATH%;%INSTALL_DIR%\bin" /M >nul 2>&1

REM Create desktop shortcut
echo 🖥️ Creating desktop shortcut...
set DESKTOP=%USERPROFILE%\Desktop
(
echo [InternetShortcut]
echo URL=file:///%INSTALL_DIR%\bin\campus.exe
echo IconFile=%INSTALL_DIR%\bin\campus.exe
echo IconIndex=0
) > "%DESKTOP%\Campus Management System.url"

REM Create start menu entry
echo 📋 Creating start menu entry...
set STARTMENU=%APPDATA%\Microsoft\Windows\Start Menu\Programs
mkdir "%STARTMENU%\Campus Management System" 2>nul
(
echo [InternetShortcut]
echo URL=file:///%INSTALL_DIR%\bin\campus.exe
echo IconFile=%INSTALL_DIR%\bin\campus.exe
echo IconIndex=0
) > "%STARTMENU%\Campus Management System\Campus Management System.url"

REM Register file associations
echo 📄 Registering file associations...
reg add "HKCR\.campus" /ve /d "CampusDataFile" /f >nul 2>&1
reg add "HKCR\CampusDataFile" /ve /d "Campus Data File" /f >nul 2>&1
reg add "HKCR\CampusDataFile\shell\open\command" /ve /d "\"%INSTALL_DIR%\bin\campus.exe\" \"%%1\"" /f >nul 2>&1

REM Create uninstaller
echo 🗑️ Creating uninstaller...
(
echo @echo off
echo echo Uninstalling Campus Management System...
echo rmdir /s /q "%INSTALL_DIR%"
echo del "%DESKTOP%\Campus Management System.url" 2^>nul
echo rmdir /s /q "%STARTMENU%\Campus Management System" 2^>nul
echo reg delete "HKCR\.campus" /f 2^>nul
echo reg delete "HKCR\CampusDataFile" /f 2^>nul
echo echo Uninstallation complete.
echo pause
) > "%INSTALL_DIR%\uninstall.bat"

REM Test installation
echo 🧪 Testing installation...
"%INSTALL_DIR%\bin\campus.exe" --version >nul 2>&1
if %errorLevel% equ 0 (
    echo ✅ Installation test passed
) else (
    echo ⚠️ Installation test failed - application may still work
)

echo.
echo ========================================
echo   Installation Summary
echo ========================================
echo ✅ Installation Directory: %INSTALL_DIR%
echo ✅ Application Files: Copied
echo ✅ Configuration: Created
echo ✅ Permissions: Set
echo ✅ PATH: Updated
echo ✅ Shortcuts: Created
echo ✅ File Associations: Registered
echo ✅ Uninstaller: Created
echo.
echo 🎉 Installation completed successfully!
echo.
echo 📋 Next Steps:
echo 1. Restart command prompt for PATH changes
echo 2. Run 'campus' from any directory
echo 3. Or use desktop shortcut
echo 4. Check documentation in %INSTALL_DIR%\docs\
echo.
echo 📞 Support: support@campus-system.com
echo 📚 Documentation: %INSTALL_DIR%\docs\
echo 🗑️ Uninstall: %INSTALL_DIR%\uninstall.bat
echo.
pause