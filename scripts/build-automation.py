#!/usr/bin/env python3
"""
Campus Management System - Build Automation
Cross-platform build script with advanced features
"""

import os
import sys
import subprocess
import shutil
from pathlib import Path

def build_project():
    """Main build function"""
    print("🚀 Campus Management System - Build Automation")
    
    # Create build directory
    build_dir = Path("build")
    if build_dir.exists():
        shutil.rmtree(build_dir)
    build_dir.mkdir()
    
    # Configure CMake
    print("⚙️ Configuring CMake...")
    result = subprocess.run([
        "cmake", "-B", "build", 
        "-DCMAKE_BUILD_TYPE=Release"
    ], capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"❌ CMake configuration failed: {result.stderr}")
        return False
    
    # Build project
    print("🔨 Building project...")
    result = subprocess.run([
        "cmake", "--build", "build", 
        "--config", "Release"
    ], capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"❌ Build failed: {result.stderr}")
        return False
    
    # Create runtime directories
    for dir_name in ["data", "credentials", "logs"]:
        Path(dir_name).mkdir(exist_ok=True)
    
    print("✅ Build completed successfully!")
    return True

if __name__ == "__main__":
    success = build_project()
    sys.exit(0 if success else 1)