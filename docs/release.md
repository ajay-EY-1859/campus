🚀 Initial stable release of AjCampus CLI system!

AjCampus is a modular, secure student management application written in C, featuring:

✅ Secure signup/signin with hashed password handling  
✅ Student profile creation with subject marks and stream tracking  
✅ PDF result export using Libharu  
✅ Audit logging and credential validation  
✅ Modular project layout and configuration via CMake  

📦 Compatible with Windows/MSVC  
📤 Built-in support for `hpdf.lib` (Libharu static library)

To build:
```bash
mkdir build && cd build
cmake ..
cmake --build .
Tested on:
• 
🔧 MSVC 2022
• 
🖥️ Windows 11
