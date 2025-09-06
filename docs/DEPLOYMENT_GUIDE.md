# 🚀 Campus Management System v2.0 - Deployment Guide

## 📋 **Quick Deployment Checklist**

### **✅ Pre-Deployment Requirements**
- [ ] Windows 10/11 or Linux system
- [ ] CMake 3.10+ installed
- [ ] C Compiler (GCC/MSVC) available
- [ ] 50MB disk space minimum
- [ ] Administrator privileges for installation

### **✅ System Requirements**
| Component | Minimum | Recommended |
|---|---|---|
| **OS** | Windows 10 | Windows 11 |
| **RAM** | 512 MB | 1 GB |
| **Storage** | 50 MB | 200 MB |
| **CPU** | Any x86/x64 | Modern processor |

---

## 🎯 **Deployment Methods**

### **Method 1: Quick Deploy (Recommended)**
```bash
# 1. Clone repository
git clone https://github.com/ajay-EY-1859/campus.git
cd campus

# 2. Build system
mkdir build && cd build
cmake ..
cmake --build . --config Release

# 3. Run application
cd build/Release
campus.exe
```

### **Method 2: Production Deploy**
```bash
# 1. Download release package
# 2. Extract to target directory
# 3. Run installer script
install.bat

# 4. Configure system
config.exe

# 5. Start service
campus.exe --service
```

---

## 📁 **Directory Structure**
```
campus/
├── bin/                    # Executable files
│   ├── campus.exe         # Main application
│   └── hpdf.dll          # PDF library
├── data/                  # User data storage
│   ├── profiles/         # User profiles
│   └── reports/          # Generated PDFs
├── logs/                  # System logs
│   ├── audit.log         # Activity logs
│   └── error.log         # Error logs
├── config/               # Configuration files
│   └── campus.conf       # System settings
└── backup/               # Data backups
    └── daily/            # Daily backups
```

---

## ⚙️ **Configuration Setup**

### **Basic Configuration**
```ini
# campus.conf
[System]
MaxUsers=1000
SessionTimeout=30
DataDirectory=./data
LogLevel=INFO

[Security]
PasswordMinLength=8
MaxLoginAttempts=3
LockoutDuration=15
OTPExpiry=5

[Campus]
DefaultType=SCHOOL
AllowedTypes=SCHOOL,COLLEGE,HOSPITAL,HOSTEL
```

### **Advanced Configuration**
```ini
[Performance]
CacheSize=100MB
MaxConcurrentUsers=500
DatabasePoolSize=10

[Backup]
AutoBackup=true
BackupInterval=24h
RetentionDays=30
```

---

## 🔒 **Security Setup**

### **File Permissions**
```bash
# Windows
icacls data /grant Users:(OI)(CI)F
icacls logs /grant Users:(OI)(CI)F

# Linux
chmod 755 data/
chmod 755 logs/
chmod 600 config/campus.conf
```

### **Firewall Configuration**
```bash
# Windows Firewall
netsh advfirewall firewall add rule name="Campus System" dir=in action=allow program="campus.exe"

# Linux iptables
iptables -A INPUT -p tcp --dport 8080 -j ACCEPT
```

---

## 📊 **Database Setup**

### **File-Based Database (Default)**
```bash
# Automatic setup - no configuration needed
# Data stored in: ./data/campus.db
```

### **External Database (Optional)**
```ini
[Database]
Type=SQLite
ConnectionString=./data/campus.sqlite
BackupEnabled=true
```

---

## 🎯 **Service Installation**

### **Windows Service**
```batch
# Install as Windows Service
sc create "Campus Management" binPath="C:\campus\campus.exe --service"
sc config "Campus Management" start=auto
sc start "Campus Management"
```

### **Linux Daemon**
```bash
# Create systemd service
sudo cp campus.service /etc/systemd/system/
sudo systemctl enable campus
sudo systemctl start campus
```

---

## 📋 **Post-Deployment Verification**

### **System Health Check**
```bash
# 1. Test application startup
campus.exe --test

# 2. Verify database connectivity
campus.exe --check-db

# 3. Test PDF generation
campus.exe --test-pdf

# 4. Validate security features
campus.exe --security-test
```

### **Performance Validation**
```bash
# Run performance benchmarks
cd performance/
run_performance_tests.bat

# Expected results:
# - Registration: < 2 seconds
# - Login: < 3 seconds  
# - Data entry: < 1 second
# - PDF generation: < 5 seconds
```

---

## 🔧 **Troubleshooting**

### **Common Issues**

**Issue: Application won't start**
```bash
# Solution 1: Check dependencies
campus.exe --check-deps

# Solution 2: Verify permissions
icacls campus.exe /grant Users:F

# Solution 3: Run as administrator
```

**Issue: PDF generation fails**
```bash
# Solution: Verify hpdf.dll location
copy hpdf.dll %SYSTEM32%
# Or place in same directory as campus.exe
```

**Issue: Database errors**
```bash
# Solution: Reset database
campus.exe --reset-db
# Backup will be created automatically
```

---

## 📈 **Monitoring & Maintenance**

### **Log Monitoring**
```bash
# Monitor system logs
tail -f logs/audit.log
tail -f logs/error.log

# Log rotation (weekly)
logrotate campus.conf
```

### **Performance Monitoring**
```bash
# Check system performance
campus.exe --status
campus.exe --performance-report

# Monitor resource usage
tasklist | findstr campus.exe
```

### **Backup Procedures**
```bash
# Manual backup
campus.exe --backup

# Automated backup (daily)
schtasks /create /tn "Campus Backup" /tr "campus.exe --backup" /sc daily
```

---

## 🚀 **Production Deployment Steps**

### **Step 1: Environment Preparation**
1. ✅ Verify system requirements
2. ✅ Install dependencies
3. ✅ Create directory structure
4. ✅ Set file permissions

### **Step 2: Application Installation**
1. ✅ Deploy application files
2. ✅ Configure system settings
3. ✅ Initialize database
4. ✅ Test basic functionality

### **Step 3: Security Configuration**
1. ✅ Configure firewall rules
2. ✅ Set up user permissions
3. ✅ Enable audit logging
4. ✅ Test security features

### **Step 4: Service Setup**
1. ✅ Install as system service
2. ✅ Configure auto-start
3. ✅ Set up monitoring
4. ✅ Create backup schedule

### **Step 5: Validation & Go-Live**
1. ✅ Run full system tests
2. ✅ Perform load testing
3. ✅ Validate all features
4. ✅ Deploy to production

---

## 📞 **Support & Maintenance**

### **Support Contacts**
- **Technical Support:** support@campus-system.com
- **Documentation:** docs@campus-system.com
- **Emergency:** emergency@campus-system.com

### **Maintenance Schedule**
- **Daily:** Automated backups
- **Weekly:** Log rotation
- **Monthly:** Performance review
- **Quarterly:** Security audit

---

## 🏆 **Deployment Success Criteria**

### **✅ Functional Validation**
- [ ] All campus types working (School/College/Hospital/Hostel)
- [ ] User registration and login functional
- [ ] Data entry and calculations accurate
- [ ] PDF generation working
- [ ] Security features active

### **✅ Performance Validation**
- [ ] Response times meet targets (< 2 sec registration)
- [ ] System handles expected load (100+ users)
- [ ] Memory usage within limits (< 10 MB)
- [ ] No performance bottlenecks

### **✅ Security Validation**
- [ ] Authentication working (2FA with OTP)
- [ ] Account lockout functional
- [ ] Input validation active
- [ ] Audit logging enabled

---

**🎉 Campus Management System v2.0 - Ready for Production Deployment! 🚀**