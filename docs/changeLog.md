
## CHANGELOG.md - AjCampus v1.0

```markdown
# CHANGELOG

All notable changes to this project will be documented in this file.

---

## [v1.0] – 2025-07-03

### Initial Release

- Added student signup with email and mobile validation
- Implemented secure signin with password hashing (`SHA256`)
- Dynamic student ID generation based on school initials + year + serial
- Profile storage using binary files
- Subject marks entry and percentage-based result summary
- Export result to PDF using Libharu (`hpdf.lib`)
- Modular file handling for profile and subjects
- Authentication audit logs stored in `login_audit.log`
- Dashboard navigation with seven options (view, edit, marks, export, logout etc.)
- `config.h` centralization for constants and file paths
- CMake build system with compiler warnings
- Included `.gitignore` and GitHub-ready `README.md`

---

## Repository & Support

- **GitHub:** https://github.com/ajay-EY-1859/campus
- **Email:** raj183576@gmail.com
