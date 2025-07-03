# 🎓 AjCampus – Secure Student CLI System

(https://github.com/ajay-EY-1859/ajcampus/actions)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Release](https://img.shields.io/badge/release-v1.0.0-success.svg)](https://github.com/ajay-EY-1859/ajcampus/releases)
[![PDF Export](https://img.shields.io/badge/pdf-enabled-green.svg)][![Authentication](https://img.shields.io/badge/auth-secure-orange.svg)](#)

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/ajay-EY-1859/ajcampus)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
(https://github.com/libharu/libharu)
[![Authentication](https://img.shields.io/badge/auth-secure-orange.svg)](#)

AjCampus is a modular C-based command-line app that allows students to securely register, manage profiles, enter subject marks, and export results as PDFs using Libharu.

---

## 📦 Features

- 🔒 Secure signup & login with hashed password storage
- 🧾 Dynamic student profile generation with subject mapping
- 📤 PDF export of result using [Libharu](https://github.com/libharu/libharu)
- 💾 Credential-based authentication & audit logging
- 🧱 Modular project structure with reusable code
- ⚙️ Cross-platform build via CMake

---

## ⚙️ Installation

```bash
git clone https://github.com/yourusername/ajcampus.git
cd ajcampus
mkdir build && cd build
cmake ..
cmake --build .
⚠️ Ensure hpdf.lib is in /lib, and hpdf.h, hpdf_config.h in /include
🚀 Run
./build/ajcampus.exe
📘 First-time users should select 1. Signup from the menu
📂 Project Structure
ajcampus/
├─ include/         # Header files
├─ lib/             # PDF engine (libharu)
├─ src/
│   └─ main/        # Core modules
├─ build/           # Compiled binaries
├─ CMakeLists.txt
├─ README.md
└─ CONTRIBUTING.md
📘 License
MIT License — free to use, modify, and distribute ❤️
🙌 Author
Created by Ajay, a logic-driven coder on a mission to build scalable C systems 💻 GitHub Profile →
