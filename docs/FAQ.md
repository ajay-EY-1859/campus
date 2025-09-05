# Frequently Asked Questions (FAQ)

## 🔍 General Questions

### Q: What is Campus Management System?
**A:** Campus Management System is a secure, multi-platform C-based application designed to manage different types of educational and institutional campuses including Schools, Colleges, Hospitals, and Hostels.

### Q: Is this software free to use?
**A:** Yes, Campus Management System is open-source software released under the MIT License. It's free for both personal and commercial use.

### Q: What platforms are supported?
**A:** The system supports Windows 10/11, Linux (Ubuntu, CentOS, Fedora), and macOS. It's built using cross-platform technologies.

### Q: Do I need internet connection to use it?
**A:** No, the core functionality works offline. Internet is only simulated for OTP delivery in the current version.

## 🚀 Installation & Setup

### Q: What are the system requirements?
**A:** Minimum requirements:
- 512 MB RAM
- 100 MB storage space
- Modern x86/x64 processor
- Windows 10+, Ubuntu 18.04+, or macOS 10.14+

### Q: How do I install the software?
**A:** You can either:
1. Download pre-built binaries from the releases page
2. Build from source using CMake
See the [Installation Guide](INSTALLATION.md) for detailed instructions.

### Q: I'm getting "CMake not found" error. What should I do?
**A:** Install CMake from https://cmake.org/download/ and ensure it's added to your system PATH.

### Q: The build fails on Windows. What's wrong?
**A:** Ensure you have Visual Studio with C++ development tools installed. Use the Visual Studio Developer Command Prompt for building.

### Q: Can I install it on a server?
**A:** Yes, but it's currently a command-line application. A web interface is planned for future versions.

## 👤 User Account & Authentication

### Q: How do I create an account?
**A:** Run the application and select "1. Register New User". Follow the step-by-step registration process.

### Q: What makes a strong password?
**A:** Your password must have:
- At least 8 characters
- Uppercase and lowercase letters
- At least one number
- At least one special character (!@#$%^&*...)

### Q: I forgot my User ID. How can I recover it?
**A:** Currently, User IDs cannot be recovered automatically. Contact support with proof of identity, or check any saved reports that contain your ID.

### Q: My account is locked. What should I do?
**A:** Account locks automatically expire after 15 minutes. This happens after 3 failed login attempts for security.

### Q: I'm not receiving OTP codes. Why?
**A:** In the current version, OTP delivery is simulated (displayed on screen). Future versions will include real SMS/email integration.

### Q: Can I change my User ID?
**A:** No, User IDs are permanent and cannot be changed. You would need to create a new account.

## 📊 Data Management

### Q: What types of data can I store?
**A:** Depends on your campus type:
- **School**: Subject marks and grades
- **College**: Course marks and credits
- **Hospital**: Medical records and health data
- **Hostel**: Accommodation and resident information

### Q: How is my data stored?
**A:** Data is stored locally in encrypted files on your computer. No data is sent to external servers.

### Q: Can I backup my data?
**A:** Yes, you can backup the entire `data/` directory. PDF reports also serve as permanent records.

### Q: Can I import data from other systems?
**A:** Currently not supported, but it's planned for future versions. You'll need to enter data manually for now.

### Q: What happens if I lose my data files?
**A:** Without backups, data cannot be recovered. Always keep regular backups of your `data/` directory.

### Q: Can multiple users share the same computer?
**A:** Yes, each user has a separate account with their own data. However, all data is stored on the same computer.

## 📄 Reports & PDF Generation

### Q: How do I generate reports?
**A:** From your dashboard, select "6. Export Report". The system will generate a PDF based on your campus type and stored data.

### Q: Where are PDF reports saved?
**A:** Reports are saved in the `data/` directory with names like `{UserID}_{campus_type}_report.pdf`.

### Q: Can I customize report formats?
**A:** Currently, report formats are fixed. Customization options are planned for future versions.

### Q: The PDF generation fails. What's wrong?
**A:** Ensure:
- You have entered data first
- Sufficient disk space is available
- The `hpdf.dll` file is present (Windows)
- You have write permissions to the data directory

### Q: Can I print the reports?
**A:** Yes, the generated PDF files can be opened with any PDF viewer and printed normally.

## 🔒 Security & Privacy

### Q: How secure is my data?
**A:** Your data is:
- Stored locally (not on external servers)
- Protected by strong authentication
- Logged for audit purposes
- Encrypted using industry-standard methods

### Q: Can others access my data?
**A:** Only if they have:
- Physical access to your computer
- Your User ID, mobile number, and password
- Access to your OTP codes

### Q: What information is logged?
**A:** The system logs:
- Login attempts and times
- Data modifications
- Security events
- System errors

### Q: Can I delete my account?
**A:** Currently, there's no automated account deletion. You can manually delete your profile files from the `credentials/` directory.

### Q: Is my personal information shared?
**A:** No, all data remains on your local computer. No information is transmitted to external servers.

## 🛠️ Technical Issues

### Q: The application crashes on startup. What should I do?
**A:** Try:
1. Running as administrator (Windows) or with sudo (Linux)
2. Checking if all required files are present
3. Ensuring sufficient disk space
4. Checking the error logs in the `logs/` directory

### Q: I'm getting "Database initialization failed" error.
**A:** This usually means:
- Insufficient disk space
- No write permissions to the application directory
- Corrupted data files

Create the required directories manually: `data/`, `credentials/`, `logs/`

### Q: The application is very slow. How can I improve performance?
**A:** Try:
- Closing other applications to free memory
- Running from an SSD instead of HDD
- Ensuring your antivirus isn't scanning the application repeatedly
- Restarting your computer

### Q: Can I run multiple instances of the application?
**A:** It's not recommended as it may cause data corruption. Use one instance at a time.

### Q: The interface looks garbled. What's wrong?
**A:** This might be a console encoding issue. Try:
- Using a different terminal/command prompt
- Checking your system's locale settings
- Running in compatibility mode (Windows)

## 🔄 Updates & Versions

### Q: How do I update to a new version?
**A:** 
1. Backup your `data/`, `credentials/`, and `logs/` directories
2. Download the new version
3. Replace the application files
4. Keep your data directories

### Q: Will my data work with newer versions?
**A:** We maintain backward compatibility, but always backup your data before updating.

### Q: How often are updates released?
**A:** 
- Major releases: Every 6 months
- Minor releases: Every 2 months  
- Security patches: As needed

### Q: Where can I see what's new in each version?
**A:** Check the [CHANGELOG.md](../CHANGELOG.md) file or the releases page on GitHub.

## 🤝 Contributing & Support

### Q: How can I contribute to the project?
**A:** You can:
- Report bugs and suggest features
- Contribute code improvements
- Help with documentation
- Test new features
See [CONTRIBUTING.md](../CONTRIBUTING.md) for details.

### Q: I found a bug. How do I report it?
**A:** Create an issue on GitHub with:
- Steps to reproduce the bug
- Expected vs actual behavior
- Your system information
- Error messages or screenshots

### Q: Can I request new features?
**A:** Yes! Create a feature request on GitHub or participate in community discussions.

### Q: How do I get help with using the software?
**A:** 
1. Check this FAQ first
2. Read the [User Guide](USER_GUIDE.md)
3. Search existing GitHub issues
4. Create a new issue if needed
5. Join community discussions

### Q: Is commercial support available?
**A:** Currently, only community support is available. Commercial support options may be added in the future.

## 🎓 Educational Use

### Q: Can schools and colleges use this for free?
**A:** Yes, educational institutions can use this software completely free under the MIT License.

### Q: Can students use this for learning programming?
**A:** Absolutely! The source code is available for educational purposes. Students can study, modify, and learn from it.

### Q: Are there any restrictions for educational use?
**A:** No restrictions. Educational use is encouraged and supported.

### Q: Can I modify the software for my institution's needs?
**A:** Yes, you can modify the software under the MIT License terms. Consider contributing improvements back to the community.

## 🔮 Future Plans

### Q: What features are planned for future versions?
**A:** See our [ROADMAP.md](../ROADMAP.md) for detailed future plans including:
- Web interface
- Mobile apps
- Advanced analytics
- Cloud integration
- Multi-language support

### Q: When will the web version be available?
**A:** A web interface is planned for v3.2, estimated for Q1 2025.

### Q: Will there be a mobile app?
**A:** Yes, mobile apps are planned for v3.2 using React Native or Flutter.

### Q: Can I influence the development roadmap?
**A:** Yes! Community feedback heavily influences our roadmap. Participate in discussions and vote on feature priorities.

---
If your question isn't answered here:

1. **Search GitHub Issues**: https://github.com/ajay-EY-1859/campus/issues
2. **Create New Issue**: Provide detailed information about your question
3. **Join Discussions**: Participate in community discussions
4. **Email Support**: raj183576@gmail.com (for urgent issues)

**Remember to include:**
- Your operating system and version
- Application version you're using
- Steps you've already tried
- Exact error messages

---

*This FAQ is regularly updated. Last updated: December 2024*