# Copilot Instructions for Campus Management System

## Project Overview
- Modular C-based CLI app for managing Schools, Colleges, Hospitals, and Hostels.
- Emphasizes security (2FA, session mgmt, audit logging), unified data handling, and extensibility.
- Key external dependency: [Libharu](https://github.com/libharu/libharu) for PDF generation (bundled for Windows, system package for Linux).

## Architecture & Patterns
- **Layered design:** UI (CLI) → Business Logic → Data Access → Infrastructure (security, logging).
- **Unified Data Model:** All campus types use a single struct (`UnifiedCampusData`) with type-driven logic (see `student.c`, `config.h`).
- **Configuration-driven:** Adding a campus type requires only updating config and logic files, not switch/case everywhere.
- **Error handling:** All major functions return `ErrorCode` enums for consistent error propagation.
- **File-based DB:** Data, logs, and reports are stored in `data/` (see `database.c`, `fileio.c`).

## Developer Workflows
- **Build:**
  - Windows: `cmake -B build` then `cmake --build build --config Release` (DLLs in `lib/` auto-copied)
  - Linux: `cmake -B build` then `cmake --build build` (requires `libhpdf-dev`)
- **Run:**
  - Windows: `build\Debug\campus.exe` or `build\Release\campus.exe`
  - Linux: `build/campus`
- **CI/CD:** GitHub Actions in `.github/workflows/` for Windows & Ubuntu. Artifacts uploaded on success. See `build.yml` for canonical steps.
- **Testing:** Manual, via app workflows. No automated test suite yet.
- **Scripts:** Utility scripts in `scripts/` (Python, batch) for build, packaging, and header checks.

## Project Conventions
- **C style:** 4 spaces, camelCase for functions/vars, UPPER_CASE for constants, max 100 chars/line.
- **Campus logic:** Extend by updating `CampusType` in `config.h`, add logic in `student.c`, update UI in `ui.c`.
- **Sensitive data:** Credentials, logs, and DB files are in `data/`, `credentials/`, `logs/` (auto-created if missing).
- **PDF export:** All report generation uses Libharu via `hpdf/` headers and `lib/` DLLs.
- **Security:** 2FA, account lockout, password policy, session timeout, and audit logging are enforced in code.

## Key Files & Directories
- `src/main/` — All core logic (auth, student, UI, DB, security)
- `include/` — All headers (including `hpdf/` for PDF)
- `lib/` — Libharu DLLs/libs (Windows)
- `data/`, `logs/`, `credentials/` — Runtime data, logs, and secrets
- `scripts/` — Build and utility scripts
- `.github/workflows/` — CI/CD definitions
- `docs/ARCHITECTURE.md` — Deep-dive on design patterns and data flows

## Examples

## Known Issues & Error Patterns

AI agents should proactively check for and address these known or likely issues (from project docs, roadmap, and user guides):

- **Build/Setup:**
  - CMake version < 3.10 or missing
  - Compiler incompatibility (MSVC/MinGW/GCC)
  - Missing or mislocated `hpdf.dll` (Windows) or `libhpdf-dev` (Linux)
  - Required directories (`data/`, `credentials/`, `logs/`) not present
  - File permission errors (especially on Linux)
  - Disk space issues

- **Runtime:**
  - Database initialization failure (permissions, disk, missing files)
  - Data not saving (invalid input, file locks, permissions)
  - PDF generation fails (no data, missing DLL, disk space)
  - Authentication lockouts (wait 15 min), OTP expiry (5 min), weak password rejection
  - Application crashes on startup (missing files, permissions, disk)
  - High memory usage with large datasets (optimization planned)
  - Performance slowdowns (especially with large data)

- **Code Quality:**
  - Memory leaks (see roadmap: detection/fixes planned)
  - Unhandled edge cases in data processing
  - Compiler warnings on some platforms
  - Static analysis (cppcheck) issues

- **Security:**
  - Input validation gaps (buffer overflow, injection)
  - Account lockout not enforced
  - Session timeout not enforced
  - Audit logging missing for sensitive actions

- **Testing:**
  - No automated test suite (manual testing only)
  - Incomplete code coverage
  - Lack of automated regression/performance tests

For more, see `README.md` (Troubleshooting), `docs/USER_MANUAL.md`, `docs/FAQ.md`, and `ROADMAP.md` (Known Issues).
For more, see `README.md`, `docs/ARCHITECTURE.md`, and `CONTRIBUTING.md`.
