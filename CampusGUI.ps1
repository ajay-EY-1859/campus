# Campus Management System - PowerShell GUI Frontend
# Screen Reader Accessible (NVDA/JAWS compatible)
# JSON-Based Storage Version

Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

# ==================== GLOBAL VARIABLES ====================
$script:MainForm = $null
$script:MainPanel = $null
$script:StatusLabel = $null
$script:CurrentPage = "MainMenu"

# Determine script/exe location - works for both .ps1 and compiled .exe
if ($PSScriptRoot) {
    $script:ScriptPath = $PSScriptRoot
}
elseif ($MyInvocation.MyCommand.Path) {
    $script:ScriptPath = Split-Path -Parent $MyInvocation.MyCommand.Path
}
else {
    # Fallback for compiled EXE - use the EXE's directory
    $script:ScriptPath = Split-Path -Parent ([System.Diagnostics.Process]::GetCurrentProcess().MainModule.FileName)
}

$script:DataFolder = "$script:ScriptPath\data"
$script:SelectedCampus = ""
$script:LoggedInUser = ""
$script:CurrentUserData = @{}

# Create data folder if not exists
if (-not (Test-Path $script:DataFolder)) {
    New-Item -ItemType Directory -Path $script:DataFolder -Force | Out-Null
}

# ==================== DATA HELPERS (JSON-BASED) ====================
function Save-UserData {
    param([hashtable]$Data)
    $filePath = "$script:DataFolder\$($script:LoggedInUser).json"
    $Data | ConvertTo-Json -Depth 10 | Out-File -FilePath $filePath -Encoding UTF8
}

function Load-UserData {
    $filePath = "$script:DataFolder\$($script:LoggedInUser).json"
    if (Test-Path $filePath) {
        $json = Get-Content -Path $filePath -Raw -Encoding UTF8
        return $json | ConvertFrom-Json
    }
    return $null
}

function Get-AllUsers {
    $users = @()
    if (Test-Path $script:DataFolder) {
        Get-ChildItem -Path $script:DataFolder -Filter "*.json" | ForEach-Object {
            $users += $_.BaseName
        }
    }
    return $users
}

# ==================== UI HELPERS ====================
function New-Label {
    param(
        [string]$Text,
        [int]$X, [int]$Y,
        [int]$Width = 300, [int]$Height = 25,
        [string]$FontSize = "12"
    )
    $label = New-Object System.Windows.Forms.Label
    $label.Text = $Text
    $label.Location = New-Object System.Drawing.Point($X, $Y)
    $label.Size = New-Object System.Drawing.Size($Width, $Height)
    $label.Font = New-Object System.Drawing.Font("Segoe UI", $FontSize)
    $label.AccessibleName = $Text
    $label.AccessibleRole = [System.Windows.Forms.AccessibleRole]::StaticText
    return $label
}

function New-TextBox {
    param(
        [string]$Name,
        [string]$AccessibleName,
        [int]$X, [int]$Y,
        [int]$Width = 300, [int]$Height = 30,
        [bool]$IsPassword = $false,
        [bool]$Multiline = $false
    )
    $textbox = New-Object System.Windows.Forms.TextBox
    $textbox.Name = $Name
    $textbox.Location = New-Object System.Drawing.Point($X, $Y)
    $textbox.Size = New-Object System.Drawing.Size($Width, $Height)
    $textbox.Font = New-Object System.Drawing.Font("Segoe UI", 11)
    $textbox.AccessibleName = $AccessibleName
    $textbox.AccessibleRole = [System.Windows.Forms.AccessibleRole]::Text
    if ($IsPassword) { $textbox.UseSystemPasswordChar = $true }
    if ($Multiline) { 
        $textbox.Multiline = $true 
        $textbox.ScrollBars = [System.Windows.Forms.ScrollBars]::Vertical
    }
    return $textbox
}

function New-Button {
    param(
        [string]$Text,
        [string]$AccessibleName,
        [int]$X, [int]$Y,
        [int]$Width = 200, [int]$Height = 40,
        [scriptblock]$OnClick,
        [string]$Color = "Blue"
    )
    $button = New-Object System.Windows.Forms.Button
    $button.Text = $Text
    $button.Location = New-Object System.Drawing.Point($X, $Y)
    $button.Size = New-Object System.Drawing.Size($Width, $Height)
    $button.Font = New-Object System.Drawing.Font("Segoe UI", 11, [System.Drawing.FontStyle]::Bold)
    $button.ForeColor = [System.Drawing.Color]::White
    $button.FlatStyle = [System.Windows.Forms.FlatStyle]::Flat
    $button.FlatAppearance.BorderSize = 0
    $button.Cursor = [System.Windows.Forms.Cursors]::Hand
    $button.AccessibleName = $AccessibleName
    $button.AccessibleRole = [System.Windows.Forms.AccessibleRole]::PushButton
    
    switch ($Color) {
        "Blue" { $button.BackColor = [System.Drawing.Color]::FromArgb(0, 120, 212) }
        "Green" { $button.BackColor = [System.Drawing.Color]::FromArgb(0, 150, 80) }
        "Red" { $button.BackColor = [System.Drawing.Color]::FromArgb(200, 50, 50) }
        "Gray" { $button.BackColor = [System.Drawing.Color]::Gray }
        "Orange" { $button.BackColor = [System.Drawing.Color]::FromArgb(220, 120, 0) }
    }
    
    $button.Add_Click($OnClick)
    return $button
}

# ==================== STATUS MESSAGE ====================
function Set-StatusMessage {
    param([string]$Message)
    $script:StatusLabel.Text = $Message
    $script:StatusLabel.AccessibleName = $Message
    $script:StatusLabel.Focus() | Out-Null
}

# ==================== CLEAR PAGE ====================
function Clear-Page {
    $script:MainPanel.Controls.Clear()
    
    $script:StatusLabel = New-Object System.Windows.Forms.Label
    $script:StatusLabel.Location = New-Object System.Drawing.Point(10, 5)
    $script:StatusLabel.Size = New-Object System.Drawing.Size(760, 28)
    $script:StatusLabel.Font = New-Object System.Drawing.Font("Segoe UI", 10)
    $script:StatusLabel.ForeColor = [System.Drawing.Color]::FromArgb(0, 80, 150)
    $script:StatusLabel.BackColor = [System.Drawing.Color]::FromArgb(220, 235, 250)
    $script:StatusLabel.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:StatusLabel.AccessibleRole = [System.Windows.Forms.AccessibleRole]::Alert
    $script:StatusLabel.TabStop = $true
    $script:StatusLabel.TabIndex = 0
    $script:MainPanel.Controls.Add($script:StatusLabel)
}

# ==================== PAGE: MAIN MENU ====================
function Show-MainMenuPage {
    Clear-Page
    $script:CurrentPage = "MainMenu"
    Set-StatusMessage "Welcome to Campus Management System. Please select an option."
    
    $title = New-Label -Text "CAMPUS MANAGEMENT SYSTEM" -X 200 -Y 45 -Width 400 -Height 40 -FontSize "18"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $btnRegister = New-Button -Text "Register" -AccessibleName "Register New User" -X 250 -Y 110 -Width 300 -Height 45 -OnClick { Show-CampusSelectionPage }
    $script:MainPanel.Controls.Add($btnRegister)
    
    $btnLogin = New-Button -Text "Login" -AccessibleName "Login Existing User" -X 250 -Y 170 -Width 300 -Height 45 -OnClick { Show-LoginPage }
    $script:MainPanel.Controls.Add($btnLogin)
    
    $btnForgot = New-Button -Text "Forgot User ID" -AccessibleName "Recover User ID" -X 250 -Y 230 -Width 300 -Height 45 -OnClick {
        Set-StatusMessage "Enter your email to recover User ID."
        Show-ForgotUserIDPage
    }
    $script:MainPanel.Controls.Add($btnForgot)
    
    $btnExit = New-Button -Text "Exit" -AccessibleName "Exit Application" -X 250 -Y 290 -Width 300 -Height 45 -Color "Red" -OnClick { $script:MainForm.Close() }
    $script:MainPanel.Controls.Add($btnExit)
    
    $btnRegister.Focus() | Out-Null
}

# ==================== PAGE: CAMPUS SELECTION ====================
function Show-CampusSelectionPage {
    Clear-Page
    $script:CurrentPage = "CampusSelection"
    Set-StatusMessage "Please select your campus type."
    
    $title = New-Label -Text "SELECT CAMPUS TYPE" -X 250 -Y 45 -Width 300 -Height 35 -FontSize "16"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $btnSchool = New-Button -Text "School" -AccessibleName "School Campus" -X 150 -Y 100 -Width 200 -Height 45 -OnClick {
        $script:SelectedCampus = "School"
        Show-RegistrationPage
    }
    $script:MainPanel.Controls.Add($btnSchool)
    
    $btnCollege = New-Button -Text "College" -AccessibleName "College Campus" -X 450 -Y 100 -Width 200 -Height 45 -OnClick {
        $script:SelectedCampus = "College"
        Show-RegistrationPage
    }
    $script:MainPanel.Controls.Add($btnCollege)
    
    $btnHospital = New-Button -Text "Hospital" -AccessibleName "Hospital Campus" -X 150 -Y 160 -Width 200 -Height 45 -OnClick {
        $script:SelectedCampus = "Hospital"
        Show-RegistrationPage
    }
    $script:MainPanel.Controls.Add($btnHospital)
    
    $btnHostel = New-Button -Text "Hostel" -AccessibleName "Hostel Campus" -X 450 -Y 160 -Width 200 -Height 45 -OnClick {
        $script:SelectedCampus = "Hostel"
        Show-RegistrationPage
    }
    $script:MainPanel.Controls.Add($btnHostel)
    
    $btnBack = New-Button -Text "Back" -AccessibleName "Go Back" -X 300 -Y 240 -Width 200 -Height 40 -Color "Gray" -OnClick { Show-MainMenuPage }
    $script:MainPanel.Controls.Add($btnBack)
    
    $btnSchool.Focus() | Out-Null
}

# ==================== PAGE: REGISTRATION ====================
function Show-RegistrationPage {
    Clear-Page
    $script:CurrentPage = "Registration"
    Set-StatusMessage "Registration Form for $($script:SelectedCampus). Fill all fields."
    
    $title = New-Label -Text "REGISTRATION - $($script:SelectedCampus)" -X 250 -Y 40 -Width 300 -Height 30 -FontSize "14"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $yPos = 80
    
    $script:MainPanel.Controls.Add((New-Label -Text "Name:" -X 50 -Y $yPos -Width 120))
    $txtName = New-TextBox -Name "txtName" -AccessibleName "Enter your name" -X 180 -Y $yPos -Width 350
    $script:MainPanel.Controls.Add($txtName)
    $yPos += 38
    
    $script:MainPanel.Controls.Add((New-Label -Text "$($script:SelectedCampus):" -X 50 -Y $yPos -Width 120))
    $txtInstitute = New-TextBox -Name "txtInstitute" -AccessibleName "Enter $($script:SelectedCampus) name" -X 180 -Y $yPos -Width 350
    $script:MainPanel.Controls.Add($txtInstitute)
    $yPos += 38
    
    $deptLabel = switch ($script:SelectedCampus) { "School" { "Stream:" } "College" { "Department:" } "Hospital" { "Department:" } "Hostel" { "Block:" } }
    $script:MainPanel.Controls.Add((New-Label -Text $deptLabel -X 50 -Y $yPos -Width 120))
    $txtDept = New-TextBox -Name "txtDept" -AccessibleName "Enter $deptLabel" -X 180 -Y $yPos -Width 350
    $script:MainPanel.Controls.Add($txtDept)
    $yPos += 38
    
    $script:MainPanel.Controls.Add((New-Label -Text "Email:" -X 50 -Y $yPos -Width 120))
    $txtEmail = New-TextBox -Name "txtEmail" -AccessibleName "Enter email" -X 180 -Y $yPos -Width 350
    $script:MainPanel.Controls.Add($txtEmail)
    $yPos += 38
    
    $script:MainPanel.Controls.Add((New-Label -Text "Mobile:" -X 50 -Y $yPos -Width 120))
    $txtMobile = New-TextBox -Name "txtMobile" -AccessibleName "Enter mobile number" -X 180 -Y $yPos -Width 350
    $script:MainPanel.Controls.Add($txtMobile)
    $yPos += 38
    
    $script:MainPanel.Controls.Add((New-Label -Text "Password:" -X 50 -Y $yPos -Width 120))
    $txtPassword = New-TextBox -Name "txtPassword" -AccessibleName "Enter password" -X 180 -Y $yPos -Width 350 -IsPassword $true
    $script:MainPanel.Controls.Add($txtPassword)
    $yPos += 45
    
    $btnRegister = New-Button -Text "Register" -AccessibleName "Submit Registration" -X 150 -Y $yPos -Width 180 -Height 40 -Color "Green" -OnClick {
        $name = $script:MainPanel.Controls["txtName"].Text
        $institute = $script:MainPanel.Controls["txtInstitute"].Text
        $dept = $script:MainPanel.Controls["txtDept"].Text
        $email = $script:MainPanel.Controls["txtEmail"].Text
        $mobile = $script:MainPanel.Controls["txtMobile"].Text
        $password = $script:MainPanel.Controls["txtPassword"].Text
        
        if ([string]::IsNullOrWhiteSpace($name) -or [string]::IsNullOrWhiteSpace($email) -or [string]::IsNullOrWhiteSpace($mobile) -or [string]::IsNullOrWhiteSpace($password)) {
            Set-StatusMessage "Error: Please fill all required fields."
            return
        }
        
        # Generate User ID
        $prefix = $script:SelectedCampus.Substring(0, 2).ToUpper()
        $random = Get-Random -Minimum 1000 -Maximum 9999
        $userID = "$prefix$random"
        
        # Save user data
        $userData = @{
            UserID     = $userID
            Name       = $name
            Campus     = $script:SelectedCampus
            Institute  = $institute
            Department = $dept
            Email      = $email
            Mobile     = $mobile
            Password   = $password
            Records    = @()
            CreatedAt  = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
        }
        
        $script:LoggedInUser = $userID
        Save-UserData -Data $userData
        $script:CurrentUserData = $userData
        
        Set-StatusMessage "Registration successful! Your User ID: $userID"
        [System.Windows.Forms.MessageBox]::Show("Registration Successful!`n`nYour User ID: $userID`n`nPlease remember this ID for login.", "Success", [System.Windows.Forms.MessageBoxButtons]::OK, [System.Windows.Forms.MessageBoxIcon]::Information)
        Show-DashboardPage
    }
    $script:MainPanel.Controls.Add($btnRegister)
    
    $btnBack = New-Button -Text "Back" -AccessibleName "Go Back" -X 450 -Y $yPos -Width 180 -Height 40 -Color "Gray" -OnClick { Show-CampusSelectionPage }
    $script:MainPanel.Controls.Add($btnBack)
    
    $txtName.Focus() | Out-Null
}

# ==================== PAGE: LOGIN ====================
function Show-LoginPage {
    Clear-Page
    $script:CurrentPage = "Login"
    Set-StatusMessage "Login Page. Enter your Mobile, User ID and Password."
    
    $title = New-Label -Text "LOGIN" -X 300 -Y 50 -Width 200 -Height 35 -FontSize "16"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $yPos = 100
    
    # Mobile Number
    $script:MainPanel.Controls.Add((New-Label -Text "Mobile Number:" -X 100 -Y $yPos -Width 140))
    $txtMobile = New-TextBox -Name "txtMobile" -AccessibleName "Enter your registered mobile number" -X 250 -Y $yPos -Width 320
    $script:MainPanel.Controls.Add($txtMobile)
    $yPos += 45
    
    # User ID
    $script:MainPanel.Controls.Add((New-Label -Text "User ID:" -X 100 -Y $yPos -Width 140))
    $txtUserID = New-TextBox -Name "txtUserID" -AccessibleName "Enter your User ID" -X 250 -Y $yPos -Width 320
    $script:MainPanel.Controls.Add($txtUserID)
    $yPos += 45
    
    # Password
    $script:MainPanel.Controls.Add((New-Label -Text "Password:" -X 100 -Y $yPos -Width 140))
    $txtPassword = New-TextBox -Name "txtPassword" -AccessibleName "Enter your password" -X 250 -Y $yPos -Width 320 -IsPassword $true
    $script:MainPanel.Controls.Add($txtPassword)
    $yPos += 55
    
    # Login Button
    $btnLogin = New-Button -Text "Login" -AccessibleName "Submit Login" -X 200 -Y $yPos -Width 180 -Height 42 -Color "Green" -OnClick {
        $mobile = $script:MainPanel.Controls["txtMobile"].Text
        $userID = $script:MainPanel.Controls["txtUserID"].Text
        $password = $script:MainPanel.Controls["txtPassword"].Text
        
        # Validation
        if ([string]::IsNullOrWhiteSpace($mobile)) {
            Set-StatusMessage "Error: Please enter Mobile Number."
            return
        }
        
        if ([string]::IsNullOrWhiteSpace($userID)) {
            Set-StatusMessage "Error: Please enter User ID."
            return
        }
        
        if ([string]::IsNullOrWhiteSpace($password)) {
            Set-StatusMessage "Error: Please enter Password."
            return
        }
        
        $script:LoggedInUser = $userID
        $userData = Load-UserData
        
        if ($null -eq $userData) {
            Set-StatusMessage "Error: User ID not found."
            return
        }
        
        # Verify mobile number
        if ($userData.Mobile -ne $mobile) {
            Set-StatusMessage "Error: Mobile number does not match."
            return
        }
        
        # Verify password
        if ($userData.Password -ne $password) {
            Set-StatusMessage "Error: Invalid password."
            return
        }
        
        $script:CurrentUserData = $userData
        $script:SelectedCampus = $userData.Campus
        Set-StatusMessage "Login successful! Welcome, $($userData.Name)"
        Show-DashboardPage
    }
    $script:MainPanel.Controls.Add($btnLogin)
    
    # Back Button
    $btnBack = New-Button -Text "Back" -AccessibleName "Go Back to Main Menu" -X 420 -Y $yPos -Width 180 -Height 42 -Color "Gray" -OnClick { Show-MainMenuPage }
    $script:MainPanel.Controls.Add($btnBack)
    
    $txtMobile.Focus() | Out-Null
}

# ==================== PAGE: FORGOT USER ID ====================
function Show-ForgotUserIDPage {
    Clear-Page
    $script:CurrentPage = "ForgotUserID"
    Set-StatusMessage "Enter your email to recover User ID."
    
    $title = New-Label -Text "RECOVER USER ID" -X 250 -Y 50 -Width 300 -Height 35 -FontSize "16"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $script:MainPanel.Controls.Add((New-Label -Text "Email:" -X 120 -Y 130 -Width 100))
    $txtEmail = New-TextBox -Name "txtEmail" -AccessibleName "Enter your registered email" -X 230 -Y 130 -Width 350
    $script:MainPanel.Controls.Add($txtEmail)
    
    $btnRecover = New-Button -Text "Recover" -AccessibleName "Find User ID" -X 200 -Y 200 -Width 180 -Height 40 -Color "Orange" -OnClick {
        $email = $script:MainPanel.Controls["txtEmail"].Text
        $found = $false
        
        Get-ChildItem -Path $script:DataFolder -Filter "*.json" -ErrorAction SilentlyContinue | ForEach-Object {
            $data = Get-Content $_.FullName -Raw | ConvertFrom-Json
            if ($data.Email -eq $email) {
                Set-StatusMessage "User ID found: $($data.UserID)"
                [System.Windows.Forms.MessageBox]::Show("Your User ID is: $($data.UserID)", "Found", [System.Windows.Forms.MessageBoxButtons]::OK, [System.Windows.Forms.MessageBoxIcon]::Information)
                $found = $true
            }
        }
        
        if (-not $found) {
            Set-StatusMessage "Email not found in our records."
        }
    }
    $script:MainPanel.Controls.Add($btnRecover)
    
    $btnBack = New-Button -Text "Back" -AccessibleName "Go Back" -X 420 -Y 200 -Width 180 -Height 40 -Color "Gray" -OnClick { Show-MainMenuPage }
    $script:MainPanel.Controls.Add($btnBack)
    
    $txtEmail.Focus() | Out-Null
}

# ==================== PAGE: DASHBOARD ====================
function Show-DashboardPage {
    Clear-Page
    $script:CurrentPage = "Dashboard"
    Set-StatusMessage "Dashboard. Welcome, $($script:CurrentUserData.Name). Select an option."
    
    $title = New-Label -Text "DASHBOARD - $($script:SelectedCampus)" -X 250 -Y 40 -Width 300 -Height 30 -FontSize "14"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $welcome = New-Label -Text "Welcome, $($script:CurrentUserData.Name) [$($script:LoggedInUser)]" -X 200 -Y 70 -Width 400 -Height 25
    $welcome.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($welcome)
    
    $yPos = 110
    $btnWidth = 220
    $btnHeight = 42
    
    $btnAddData = New-Button -Text "Add Data" -AccessibleName "Add New Record" -X 160 -Y $yPos -Width $btnWidth -Height $btnHeight -OnClick { Show-AddDataPage }
    $script:MainPanel.Controls.Add($btnAddData)
    
    $btnViewData = New-Button -Text "View Data" -AccessibleName "View Your Records" -X 420 -Y $yPos -Width $btnWidth -Height $btnHeight -OnClick { Show-ViewDataPage }
    $script:MainPanel.Controls.Add($btnViewData)
    $yPos += 55
    
    $btnProfile = New-Button -Text "View Profile" -AccessibleName "View Your Profile" -X 160 -Y $yPos -Width $btnWidth -Height $btnHeight -OnClick { Show-ProfilePage }
    $script:MainPanel.Controls.Add($btnProfile)
    
    $btnExport = New-Button -Text "Export Report" -AccessibleName "Export Data to File" -X 420 -Y $yPos -Width $btnWidth -Height $btnHeight -Color "Orange" -OnClick { Show-ExportPage }
    $script:MainPanel.Controls.Add($btnExport)
    $yPos += 55
    
    $btnPassword = New-Button -Text "Change Password" -AccessibleName "Change Your Password" -X 290 -Y $yPos -Width $btnWidth -Height $btnHeight -OnClick { Show-ChangePasswordPage }
    $script:MainPanel.Controls.Add($btnPassword)
    $yPos += 60
    
    $btnLogout = New-Button -Text "Logout" -AccessibleName "Logout" -X 290 -Y $yPos -Width $btnWidth -Height $btnHeight -Color "Red" -OnClick {
        $script:LoggedInUser = ""
        $script:CurrentUserData = @{}
        Show-MainMenuPage
    }
    $script:MainPanel.Controls.Add($btnLogout)
    
    $btnAddData.Focus() | Out-Null
}

# ==================== PAGE: ADD DATA ====================
function Show-AddDataPage {
    Clear-Page
    $script:CurrentPage = "AddData"
    Set-StatusMessage "Add New Record for $($script:SelectedCampus)."
    
    $title = New-Label -Text "ADD DATA - $($script:SelectedCampus)" -X 250 -Y 40 -Width 300 -Height 30 -FontSize "14"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $yPos = 80
    
    switch ($script:SelectedCampus) {
        "School" {
            $script:MainPanel.Controls.Add((New-Label -Text "Subject:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtSubject" -AccessibleName "Enter subject name" -X 180 -Y $yPos -Width 300))
            $yPos += 40
            $script:MainPanel.Controls.Add((New-Label -Text "Marks Obtained:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtMarks" -AccessibleName "Enter marks obtained" -X 180 -Y $yPos -Width 150))
            $yPos += 40
            $script:MainPanel.Controls.Add((New-Label -Text "Total Marks:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtTotal" -AccessibleName "Enter total marks" -X 180 -Y $yPos -Width 150))
        }
        "College" {
            $script:MainPanel.Controls.Add((New-Label -Text "Course:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtCourse" -AccessibleName "Enter course name" -X 180 -Y $yPos -Width 300))
            $yPos += 40
            $script:MainPanel.Controls.Add((New-Label -Text "Credits:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtCredits" -AccessibleName "Enter credits" -X 180 -Y $yPos -Width 100))
            $yPos += 40
            $script:MainPanel.Controls.Add((New-Label -Text "Grade:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtGrade" -AccessibleName "Enter grade A/B/C/D/F" -X 180 -Y $yPos -Width 100))
        }
        "Hospital" {
            $script:MainPanel.Controls.Add((New-Label -Text "Patient Name:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtPatient" -AccessibleName "Enter patient name" -X 180 -Y $yPos -Width 300))
            $yPos += 40
            $script:MainPanel.Controls.Add((New-Label -Text "Diagnosis:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtDiagnosis" -AccessibleName "Enter diagnosis" -X 180 -Y $yPos -Width 300))
            $yPos += 40
            $script:MainPanel.Controls.Add((New-Label -Text "Treatment:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtTreatment" -AccessibleName "Enter treatment" -X 180 -Y $yPos -Width 300))
        }
        "Hostel" {
            $script:MainPanel.Controls.Add((New-Label -Text "Room Number:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtRoom" -AccessibleName "Enter room number" -X 180 -Y $yPos -Width 150))
            $yPos += 40
            $script:MainPanel.Controls.Add((New-Label -Text "Floor:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtFloor" -AccessibleName "Enter floor number" -X 180 -Y $yPos -Width 100))
            $yPos += 40
            $script:MainPanel.Controls.Add((New-Label -Text "Rent Amount:" -X 50 -Y $yPos -Width 120))
            $script:MainPanel.Controls.Add((New-TextBox -Name "txtRent" -AccessibleName "Enter rent amount" -X 180 -Y $yPos -Width 150))
        }
    }
    
    $yPos += 55
    
    $btnSave = New-Button -Text "Save" -AccessibleName "Save Record" -X 150 -Y $yPos -Width 180 -Height 40 -Color "Green" -OnClick {
        $record = @{ Date = (Get-Date).ToString("yyyy-MM-dd HH:mm") }
        
        switch ($script:SelectedCampus) {
            "School" {
                $record.Subject = $script:MainPanel.Controls["txtSubject"].Text
                $record.MarksObtained = $script:MainPanel.Controls["txtMarks"].Text
                $record.TotalMarks = $script:MainPanel.Controls["txtTotal"].Text
                if ($record.TotalMarks -gt 0) {
                    $record.Percentage = [math]::Round(([int]$record.MarksObtained / [int]$record.TotalMarks) * 100, 2)
                }
            }
            "College" {
                $record.Course = $script:MainPanel.Controls["txtCourse"].Text
                $record.Credits = $script:MainPanel.Controls["txtCredits"].Text
                $record.Grade = $script:MainPanel.Controls["txtGrade"].Text
            }
            "Hospital" {
                $record.Patient = $script:MainPanel.Controls["txtPatient"].Text
                $record.Diagnosis = $script:MainPanel.Controls["txtDiagnosis"].Text
                $record.Treatment = $script:MainPanel.Controls["txtTreatment"].Text
            }
            "Hostel" {
                $record.Room = $script:MainPanel.Controls["txtRoom"].Text
                $record.Floor = $script:MainPanel.Controls["txtFloor"].Text
                $record.Rent = $script:MainPanel.Controls["txtRent"].Text
            }
        }
        
        # Reload and update
        $userData = Load-UserData
        if ($null -eq $userData.Records) { $userData.Records = @() }
        $userData.Records += $record
        $script:CurrentUserData = $userData
        Save-UserData -Data ([hashtable]$userData)
        
        Set-StatusMessage "Record saved successfully!"
        [System.Windows.Forms.MessageBox]::Show("Record saved successfully!", "Success")
        Show-DashboardPage
    }
    $script:MainPanel.Controls.Add($btnSave)
    
    $btnBack = New-Button -Text "Back" -AccessibleName "Go Back" -X 450 -Y $yPos -Width 180 -Height 40 -Color "Gray" -OnClick { Show-DashboardPage }
    $script:MainPanel.Controls.Add($btnBack)
}

# ==================== PAGE: VIEW DATA ====================
function Show-ViewDataPage {
    Clear-Page
    $script:CurrentPage = "ViewData"
    
    $userData = Load-UserData
    $recordCount = if ($userData.Records) { $userData.Records.Count } else { 0 }
    Set-StatusMessage "Your Records ($recordCount items)."
    
    $title = New-Label -Text "YOUR RECORDS" -X 300 -Y 40 -Width 200 -Height 30 -FontSize "14"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    # ListView for data
    $listView = New-Object System.Windows.Forms.ListView
    $listView.Location = New-Object System.Drawing.Point(30, 80)
    $listView.Size = New-Object System.Drawing.Size(720, 250)
    $listView.View = [System.Windows.Forms.View]::Details
    $listView.FullRowSelect = $true
    $listView.GridLines = $true
    $listView.Font = New-Object System.Drawing.Font("Segoe UI", 10)
    $listView.AccessibleName = "Records List"
    
    # Add columns based on campus type
    switch ($script:SelectedCampus) {
        "School" {
            $listView.Columns.Add("Date", 120) | Out-Null
            $listView.Columns.Add("Subject", 200) | Out-Null
            $listView.Columns.Add("Marks", 100) | Out-Null
            $listView.Columns.Add("Total", 100) | Out-Null
            $listView.Columns.Add("Percentage", 100) | Out-Null
        }
        "College" {
            $listView.Columns.Add("Date", 120) | Out-Null
            $listView.Columns.Add("Course", 250) | Out-Null
            $listView.Columns.Add("Credits", 100) | Out-Null
            $listView.Columns.Add("Grade", 100) | Out-Null
        }
        "Hospital" {
            $listView.Columns.Add("Date", 120) | Out-Null
            $listView.Columns.Add("Patient", 180) | Out-Null
            $listView.Columns.Add("Diagnosis", 200) | Out-Null
            $listView.Columns.Add("Treatment", 200) | Out-Null
        }
        "Hostel" {
            $listView.Columns.Add("Date", 120) | Out-Null
            $listView.Columns.Add("Room", 150) | Out-Null
            $listView.Columns.Add("Floor", 100) | Out-Null
            $listView.Columns.Add("Rent", 150) | Out-Null
        }
    }
    
    # Add data
    if ($userData.Records) {
        foreach ($rec in $userData.Records) {
            $item = New-Object System.Windows.Forms.ListViewItem($rec.Date)
            switch ($script:SelectedCampus) {
                "School" {
                    $item.SubItems.Add($rec.Subject) | Out-Null
                    $item.SubItems.Add($rec.MarksObtained) | Out-Null
                    $item.SubItems.Add($rec.TotalMarks) | Out-Null
                    $item.SubItems.Add("$($rec.Percentage)%") | Out-Null
                }
                "College" {
                    $item.SubItems.Add($rec.Course) | Out-Null
                    $item.SubItems.Add($rec.Credits) | Out-Null
                    $item.SubItems.Add($rec.Grade) | Out-Null
                }
                "Hospital" {
                    $item.SubItems.Add($rec.Patient) | Out-Null
                    $item.SubItems.Add($rec.Diagnosis) | Out-Null
                    $item.SubItems.Add($rec.Treatment) | Out-Null
                }
                "Hostel" {
                    $item.SubItems.Add($rec.Room) | Out-Null
                    $item.SubItems.Add($rec.Floor) | Out-Null
                    $item.SubItems.Add($rec.Rent) | Out-Null
                }
            }
            $listView.Items.Add($item) | Out-Null
        }
    }
    
    $script:MainPanel.Controls.Add($listView)
    
    $btnBack = New-Button -Text "Back" -AccessibleName "Go Back to Dashboard" -X 300 -Y 350 -Width 200 -Height 40 -Color "Gray" -OnClick { Show-DashboardPage }
    $script:MainPanel.Controls.Add($btnBack)
    
    $listView.Focus() | Out-Null
}

# ==================== PAGE: VIEW PROFILE ====================
function Show-ProfilePage {
    Clear-Page
    $script:CurrentPage = "Profile"
    Set-StatusMessage "Your Profile Information."
    
    $userData = Load-UserData
    
    $title = New-Label -Text "YOUR PROFILE" -X 300 -Y 40 -Width 200 -Height 30 -FontSize "14"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $yPos = 90
    $fields = @(
        @("User ID:", $userData.UserID),
        @("Name:", $userData.Name),
        @("Campus Type:", $userData.Campus),
        @("Institute:", $userData.Institute),
        @("Department:", $userData.Department),
        @("Email:", $userData.Email),
        @("Mobile:", $userData.Mobile),
        @("Registered On:", $userData.CreatedAt),
        @("Total Records:", ($userData.Records.Count).ToString())
    )
    
    foreach ($field in $fields) {
        $lbl = New-Label -Text $field[0] -X 100 -Y $yPos -Width 150
        $lbl.Font = New-Object System.Drawing.Font("Segoe UI", 11, [System.Drawing.FontStyle]::Bold)
        $script:MainPanel.Controls.Add($lbl)
        
        $val = New-Label -Text $field[1] -X 260 -Y $yPos -Width 400
        $script:MainPanel.Controls.Add($val)
        $yPos += 30
    }
    
    $btnBack = New-Button -Text "Back" -AccessibleName "Go Back to Dashboard" -X 300 -Y ($yPos + 20) -Width 200 -Height 40 -Color "Gray" -OnClick { Show-DashboardPage }
    $script:MainPanel.Controls.Add($btnBack)
}

# ==================== PAGE: EXPORT ====================
function Show-ExportPage {
    Clear-Page
    $script:CurrentPage = "Export"
    Set-StatusMessage "Export your data to a file."
    
    $title = New-Label -Text "EXPORT REPORT" -X 280 -Y 50 -Width 250 -Height 30 -FontSize "14"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $info = New-Label -Text "Choose export format:" -X 200 -Y 110 -Width 400 -Height 25
    $info.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($info)
    
    $btnTXT = New-Button -Text "Export as TXT" -AccessibleName "Export as Text File" -X 150 -Y 160 -Width 200 -Height 45 -OnClick {
        $userData = Load-UserData
        $exportPath = "$script:DataFolder\$($script:LoggedInUser)_report.txt"
        
        $content = "===== CAMPUS MANAGEMENT SYSTEM REPORT =====`r`n"
        $content += "Generated: $(Get-Date)`r`n`r`n"
        $content += "USER PROFILE`r`n"
        $content += "User ID: $($userData.UserID)`r`n"
        $content += "Name: $($userData.Name)`r`n"
        $content += "Campus: $($userData.Campus)`r`n"
        $content += "Institute: $($userData.Institute)`r`n"
        $content += "Email: $($userData.Email)`r`n"
        $content += "Mobile: $($userData.Mobile)`r`n`r`n"
        $content += "RECORDS ($($userData.Records.Count) items)`r`n"
        $content += "=" * 50 + "`r`n"
        
        $i = 1
        foreach ($rec in $userData.Records) {
            $content += "`r`nRecord #$i - $($rec.Date)`r`n"
            $rec.PSObject.Properties | Where-Object { $_.Name -ne "Date" } | ForEach-Object {
                $content += "  $($_.Name): $($_.Value)`r`n"
            }
            $i++
        }
        
        try {
            $content | Out-File -FilePath $exportPath -Encoding UTF8 -Force
            Set-StatusMessage "Exported to: $exportPath"
            [System.Windows.Forms.MessageBox]::Show("Report exported to:`n$exportPath", "Export Complete")
        }
        catch {
            Set-StatusMessage "Export failed: $_"
            [System.Windows.Forms.MessageBox]::Show("Export failed: $_", "Error")
        }
    }
    $script:MainPanel.Controls.Add($btnTXT)
    
    $btnCSV = New-Button -Text "Export as CSV" -AccessibleName "Export as CSV File" -X 450 -Y 160 -Width 200 -Height 45 -Color "Green" -OnClick {
        $userData = Load-UserData
        $exportPath = "$script:DataFolder\$($script:LoggedInUser)_records.csv"
        
        if ($userData.Records -and $userData.Records.Count -gt 0) {
            try {
                $userData.Records | Export-Csv -Path $exportPath -NoTypeInformation -Encoding UTF8 -Force
                Set-StatusMessage "Exported to: $exportPath"
                [System.Windows.Forms.MessageBox]::Show("CSV exported to:`n$exportPath", "Export Complete")
            }
            catch {
                Set-StatusMessage "Export failed: $_"
                [System.Windows.Forms.MessageBox]::Show("Export failed: $_", "Error")
            }
        }
        else {
            Set-StatusMessage "No records to export."
        }
    }
    $script:MainPanel.Controls.Add($btnCSV)
    
    $btnBack = New-Button -Text "Back" -AccessibleName "Go Back to Dashboard" -X 300 -Y 250 -Width 200 -Height 40 -Color "Gray" -OnClick { Show-DashboardPage }
    $script:MainPanel.Controls.Add($btnBack)
}

# ==================== PAGE: CHANGE PASSWORD ====================
function Show-ChangePasswordPage {
    Clear-Page
    $script:CurrentPage = "ChangePassword"
    Set-StatusMessage "Change your password."
    
    $title = New-Label -Text "CHANGE PASSWORD" -X 270 -Y 50 -Width 260 -Height 30 -FontSize "14"
    $title.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
    $script:MainPanel.Controls.Add($title)
    
    $yPos = 110
    
    $script:MainPanel.Controls.Add((New-Label -Text "Current Password:" -X 100 -Y $yPos -Width 150))
    $txtCurrent = New-TextBox -Name "txtCurrent" -AccessibleName "Enter current password" -X 270 -Y $yPos -Width 300 -IsPassword $true
    $script:MainPanel.Controls.Add($txtCurrent)
    $yPos += 45
    
    $script:MainPanel.Controls.Add((New-Label -Text "New Password:" -X 100 -Y $yPos -Width 150))
    $txtNew = New-TextBox -Name "txtNew" -AccessibleName "Enter new password" -X 270 -Y $yPos -Width 300 -IsPassword $true
    $script:MainPanel.Controls.Add($txtNew)
    $yPos += 45
    
    $script:MainPanel.Controls.Add((New-Label -Text "Confirm Password:" -X 100 -Y $yPos -Width 150))
    $txtConfirm = New-TextBox -Name "txtConfirm" -AccessibleName "Confirm new password" -X 270 -Y $yPos -Width 300 -IsPassword $true
    $script:MainPanel.Controls.Add($txtConfirm)
    $yPos += 55
    
    $btnChange = New-Button -Text "Change" -AccessibleName "Change Password" -X 200 -Y $yPos -Width 180 -Height 40 -Color "Green" -OnClick {
        $current = $script:MainPanel.Controls["txtCurrent"].Text
        $new = $script:MainPanel.Controls["txtNew"].Text
        $confirm = $script:MainPanel.Controls["txtConfirm"].Text
        
        $userData = Load-UserData
        
        if ($userData.Password -ne $current) {
            Set-StatusMessage "Error: Current password is incorrect."
            return
        }
        
        if ($new -ne $confirm) {
            Set-StatusMessage "Error: New passwords do not match."
            return
        }
        
        if ($new.Length -lt 4) {
            Set-StatusMessage "Error: Password must be at least 4 characters."
            return
        }
        
        $userData.Password = $new
        Save-UserData -Data ([hashtable]$userData)
        
        Set-StatusMessage "Password changed successfully!"
        [System.Windows.Forms.MessageBox]::Show("Password changed successfully!", "Success")
        Show-DashboardPage
    }
    $script:MainPanel.Controls.Add($btnChange)
    
    $btnBack = New-Button -Text "Back" -AccessibleName "Go Back" -X 420 -Y $yPos -Width 180 -Height 40 -Color "Gray" -OnClick { Show-DashboardPage }
    $script:MainPanel.Controls.Add($btnBack)
    
    $txtCurrent.Focus() | Out-Null
}

# ==================== MAIN FORM ====================
function Initialize-MainForm {
    $script:MainForm = New-Object System.Windows.Forms.Form
    $script:MainForm.Text = "Campus Management System"
    $script:MainForm.Size = New-Object System.Drawing.Size(800, 480)
    $script:MainForm.StartPosition = [System.Windows.Forms.FormStartPosition]::CenterScreen
    $script:MainForm.BackColor = [System.Drawing.Color]::FromArgb(245, 245, 248)
    $script:MainForm.Font = New-Object System.Drawing.Font("Segoe UI", 10)
    $script:MainForm.FormBorderStyle = [System.Windows.Forms.FormBorderStyle]::FixedSingle
    $script:MainForm.MaximizeBox = $false
    $script:MainForm.AccessibleName = "Campus Management System"
    
    $script:MainPanel = New-Object System.Windows.Forms.Panel
    $script:MainPanel.Location = New-Object System.Drawing.Point(0, 0)
    $script:MainPanel.Size = New-Object System.Drawing.Size(784, 441)
    $script:MainPanel.BackColor = [System.Drawing.Color]::FromArgb(245, 245, 248)
    $script:MainForm.Controls.Add($script:MainPanel)
    
    Show-MainMenuPage
    
    $script:MainForm.KeyPreview = $true
    $script:MainForm.Add_KeyDown({
            if ($_.KeyCode -eq [System.Windows.Forms.Keys]::Escape) {
                if ($script:CurrentPage -eq "Dashboard") { return }
                if ($script:CurrentPage -ne "MainMenu") { Show-MainMenuPage }
            }
        })
    
    [System.Windows.Forms.Application]::Run($script:MainForm)
}

# ==================== RUN ====================
Initialize-MainForm
