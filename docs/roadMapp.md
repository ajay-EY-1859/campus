# 🗺️ AjCampus Project Roadmap

This roadmap outlines planned enhancements, future milestones, and long-term goals for the AjCampus CLI system.

---

## ✅ v1.0 – Stable Release (Completed)

- Secure signup/signin system with hashed password storage
- Student profile generation and validation
- Subject-wise mark entry and result percentage calculation
- PDF export of result using Libharu (`hpdf.lib`)
- Audit logging system (`login_audit.log`)
- Modular architecture with headers and organized source files
- CMake build support (MSVC compatible)
- Documentation (`README.md`, `CONTRIBUTING.md`, `LICENSE`, `CHANGELOG.md`)

---

## 🟡 v1.1 – User Experience & Testing

> Status: Planned

- Dashboard UX refinements and menu key shortcuts
- Password strength feedback (`weak`, `strong`) based on complexity
- CLI command flags (e.g. `--signup`, `--student AJ2401`)
- Profile backup/restore from within dashboard
- Logging enhancements (timestamps, session ID)
- Basic unit tests in `src/tests/`
- Auto-generated `demo.pdf` for Libharu sanity check

---

## 🔵 v2.0 – Admin Expansion

> Status: Upcoming (Goal-driven)

- Role-based login (admin/student)
- Admin tools for editing/deleting student profiles
- View login audit logs from dashboard
- Export all profiles to CSV for school archival
- Branded PDF results (logo, footer text, etc.)
- Lockout system after failed login attempts

---

## 🔴 v3.0 – Cross-Platform & APIs

> Status: Ambitious

- Linux/macOS builds with portable paths
- SQLite or JSON-based profile storage
- REST API-ready CLI wrapper for web-based transition
- GUI optional layer (Qt/GTK) prototype
- CLI email system for password reset
- PDF QR download option for mobile sync

---

## 🎯 Milestone Summary

| Version | Focus           | Status     |
|---------|------------------|------------|
| v1.0    | Core system      | ✅ Completed |
| v1.1    | UX + Testing     | 🟡 Planned |
| v2.0    | Admin features   | 🔵 Goal     |
| v3.0    | APIs & GUI       | 🔴 Visionary |

Stay tuned and keep watching this space! 😄
