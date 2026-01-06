# Campus Management System - Build Script
# Converts PowerShell GUI to standalone EXE

param(
    [switch]$Install,
    [switch]$Build,
    [switch]$Run
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$SourceFile = Join-Path $ScriptDir "CampusGUI.ps1"
$OutputExe = Join-Path $ScriptDir "CampusGUI.exe"
$IconFile = Join-Path $ScriptDir "campus.ico"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Campus Management System - Builder   " -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Function to check if ps2exe is installed
function Test-Ps2Exe {
    try {
        Get-Command ps2exe -ErrorAction Stop | Out-Null
        return $true
    }
    catch {
        return $false
    }
}

# Install ps2exe module
if ($Install -or -not (Test-Ps2Exe)) {
    Write-Host "[1/3] Installing ps2exe module..." -ForegroundColor Yellow
    
    # Check if running as admin for module installation
    $isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
    
    if (-not $isAdmin) {
        Write-Host "Installing for current user only (no admin rights)" -ForegroundColor Gray
        Install-Module -Name ps2exe -Scope CurrentUser -Force -AllowClobber
    }
    else {
        Install-Module -Name ps2exe -Scope AllUsers -Force -AllowClobber
    }
    
    Write-Host "[OK] ps2exe installed successfully!" -ForegroundColor Green
    Write-Host ""
}

# Build EXE
if ($Build -or (-not $Install -and -not $Run)) {
    Write-Host "[2/3] Building EXE..." -ForegroundColor Yellow
    Write-Host "  Source: $SourceFile" -ForegroundColor Gray
    Write-Host "  Output: $OutputExe" -ForegroundColor Gray
    Write-Host ""
    
    # Check if source exists
    if (-not (Test-Path $SourceFile)) {
        Write-Host "[ERROR] Source file not found: $SourceFile" -ForegroundColor Red
        exit 1
    }
    
    # Build parameters
    $buildParams = @{
        InputFile    = $SourceFile
        OutputFile   = $OutputExe
        NoConsole    = $true
        Title        = "Campus Management System"
        Description  = "Campus Management System - PowerShell GUI Application"
        Company      = "Campus Team"
        Product      = "Campus Management System"
        Version      = "1.0.0.0"
        Copyright    = "2026 Campus Team"
        RequireAdmin = $false
    }
    
    # Add icon if exists
    if (Test-Path $IconFile) {
        $buildParams.IconFile = $IconFile
        Write-Host "  Icon: $IconFile" -ForegroundColor Gray
    }
    
    # Run ps2exe
    try {
        Invoke-ps2exe @buildParams
        
        if (Test-Path $OutputExe) {
            $fileInfo = Get-Item $OutputExe
            Write-Host ""
            Write-Host "[OK] Build successful!" -ForegroundColor Green
            Write-Host "  File: $OutputExe" -ForegroundColor White
            Write-Host "  Size: $([math]::Round($fileInfo.Length / 1KB, 2)) KB" -ForegroundColor White
            Write-Host ""
        }
    }
    catch {
        Write-Host "[ERROR] Build failed: $_" -ForegroundColor Red
        exit 1
    }
}

# Run the EXE
if ($Run) {
    if (Test-Path $OutputExe) {
        Write-Host "[3/3] Running CampusGUI.exe..." -ForegroundColor Yellow
        Start-Process $OutputExe
    }
    else {
        Write-Host "[ERROR] EXE not found. Run build first: .\build.ps1 -Build" -ForegroundColor Red
    }
}

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Build Complete!                      " -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Usage:" -ForegroundColor White
Write-Host "  Double-click CampusGUI.exe to run" -ForegroundColor Gray
Write-Host "  Or run: .\CampusGUI.exe" -ForegroundColor Gray
Write-Host ""
