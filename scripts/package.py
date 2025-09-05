#!/usr/bin/env python3
"""
Release packaging script for Campus Management System.
Creates distribution packages with all necessary files.
"""

import argparse
import os
import sys
import shutil
import hashlib
import zipfile
import tarfile
from pathlib import Path
from datetime import datetime

def calculate_checksum(file_path):
    """Calculate SHA-256 checksum of a file."""
    sha256_hash = hashlib.sha256()
    with open(file_path, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            sha256_hash.update(chunk)
    return sha256_hash.hexdigest()

def create_directory_structure(package_dir):
    """Create the package directory structure."""
    dirs = [
        "bin",
        "lib", 
        "docs",
        "examples",
        "scripts"
    ]
    
    for dir_name in dirs:
        (package_dir / dir_name).mkdir(parents=True, exist_ok=True)

def copy_binaries(package_dir, build_dir):
    """Copy compiled binaries to package."""
    bin_dir = package_dir / "bin"
    
    # Windows binaries
    windows_exe = build_dir / "build" / "Release" / "campus.exe"
    if windows_exe.exists():
        shutil.copy2(windows_exe, bin_dir)
        print(f"Copied {windows_exe.name}")
    
    # Linux/macOS binaries
    unix_exe = build_dir / "campus"
    if unix_exe.exists():
        shutil.copy2(unix_exe, bin_dir)
        print(f"Copied {unix_exe.name}")

def copy_libraries(package_dir):
    """Copy required libraries to package."""
    lib_src = Path("lib")
    lib_dst = package_dir / "lib"
    
    if lib_src.exists():
        for lib_file in lib_src.iterdir():
            if lib_file.is_file():
                shutil.copy2(lib_file, lib_dst)
                print(f"Copied library: {lib_file.name}")

def copy_documentation(package_dir):
    """Copy documentation to package."""
    docs_dst = package_dir / "docs"
    
    # Main documentation files
    doc_files = [
        "README.md",
        "LICENSE.md", 
        "CONTRIBUTING.md",
        "ROADMAP.md",
        "CHANGELOG.md"
    ]
    
    for doc_file in doc_files:
        if Path(doc_file).exists():
            shutil.copy2(doc_file, docs_dst)
            print(f"Copied documentation: {doc_file}")
    
    # Copy docs directory
    docs_src = Path("docs")
    if docs_src.exists():
        for doc_file in docs_src.rglob("*.md"):
            rel_path = doc_file.relative_to(docs_src)
            dst_file = docs_dst / rel_path
            dst_file.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(doc_file, dst_file)
            print(f"Copied doc: {rel_path}")

def copy_scripts(package_dir):
    """Copy utility scripts to package."""
    scripts_src = Path("scripts")
    scripts_dst = package_dir / "scripts"
    
    if scripts_src.exists():
        # Copy Python scripts
        for script_file in scripts_src.glob("*.py"):
            shutil.copy2(script_file, scripts_dst)
            print(f"Copied script: {script_file.name}")
        
        # Copy requirements.txt
        req_file = scripts_src / "requirements.txt"
        if req_file.exists():
            shutil.copy2(req_file, scripts_dst)

def create_examples(package_dir):
    """Create example files and configurations."""
    examples_dir = package_dir / "examples"
    
    # Create sample configuration
    config_example = examples_dir / "config.ini.example"
    with open(config_example, 'w') as f:
        f.write("""[database]
path=data/campus.db
backup_interval=3600

[security]
session_timeout=1800
max_login_attempts=3
account_lock_duration=900

[logging]
level=INFO
audit_enabled=true
security_log_enabled=true
""")
    
    # Create startup script examples
    if os.name == 'nt':  # Windows
        startup_script = examples_dir / "start_campus.bat"
        with open(startup_script, 'w') as f:
            f.write("""@echo off
echo Starting Campus Management System...
cd /d "%~dp0.."
if not exist data mkdir data
if not exist credentials mkdir credentials  
if not exist logs mkdir logs
bin\\campus.exe
pause
""")
    else:  # Linux/macOS
        startup_script = examples_dir / "start_campus.sh"
        with open(startup_script, 'w') as f:
            f.write("""#!/bin/bash
echo "Starting Campus Management System..."
cd "$(dirname "$0")/.."
mkdir -p data credentials logs
./bin/campus
""")
        startup_script.chmod(0o755)

def create_install_script(package_dir):
    """Create installation script."""
    if os.name == 'nt':  # Windows
        install_script = package_dir / "install.bat"
        with open(install_script, 'w') as f:
            f.write("""@echo off
echo Installing Campus Management System...
mkdir data 2>nul
mkdir credentials 2>nul
mkdir logs 2>nul
echo Installation complete!
echo Run bin\\campus.exe to start the application.
pause
""")
    else:  # Linux/macOS
        install_script = package_dir / "install.sh"
        with open(install_script, 'w') as f:
            f.write("""#!/bin/bash
echo "Installing Campus Management System..."
mkdir -p data credentials logs
chmod +x bin/campus 2>/dev/null || true
echo "Installation complete!"
echo "Run ./bin/campus to start the application."
""")
        install_script.chmod(0o755)

def create_zip_package(package_dir, output_file):
    """Create ZIP package."""
    print(f"Creating ZIP package: {output_file}")
    
    with zipfile.ZipFile(output_file, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for file_path in package_dir.rglob('*'):
            if file_path.is_file():
                arc_name = file_path.relative_to(package_dir.parent)
                zipf.write(file_path, arc_name)
    
    return calculate_checksum(output_file)

def create_tar_package(package_dir, output_file):
    """Create TAR.GZ package."""
    print(f"Creating TAR.GZ package: {output_file}")
    
    with tarfile.open(output_file, 'w:gz') as tarf:
        tarf.add(package_dir, arcname=package_dir.name)
    
    return calculate_checksum(output_file)

def create_checksums_file(dist_dir, checksums):
    """Create checksums file."""
    checksums_file = dist_dir / "checksums.sha256"
    
    with open(checksums_file, 'w') as f:
        f.write("# SHA-256 Checksums for Campus Management System\n")
        f.write(f"# Generated on {datetime.now().isoformat()}\n\n")
        
        for filename, checksum in checksums.items():
            f.write(f"{checksum}  {filename}\n")
    
    print(f"Created checksums file: {checksums_file}")

def main():
    parser = argparse.ArgumentParser(description="Package Campus Management System")
    parser.add_argument("--version", required=True, help="Version number")
    parser.add_argument("--build-dir", default="build", help="Build directory")
    parser.add_argument("--output-dir", default="dist", help="Output directory")
    parser.add_argument("--platform", help="Platform name (auto-detected if not specified)")
    
    args = parser.parse_args()
    
    # Detect platform if not specified
    if not args.platform:
        if os.name == 'nt':
            args.platform = "windows"
        elif sys.platform == 'darwin':
            args.platform = "macos"
        else:
            args.platform = "linux"
    
    print(f"Campus Management System Packaging Script")
    print(f"Version: {args.version}")
    print(f"Platform: {args.platform}")
    
    # Setup directories
    project_root = Path(__file__).parent.parent
    build_dir = project_root / args.build_dir
    dist_dir = project_root / args.output_dir
    
    package_name = f"campus-{args.version}-{args.platform}"
    package_dir = dist_dir / package_name
    
    # Clean and create output directory
    if package_dir.exists():
        shutil.rmtree(package_dir)
    package_dir.mkdir(parents=True, exist_ok=True)
    
    # Create package structure
    print("Creating package structure...")
    create_directory_structure(package_dir)
    
    # Copy files
    print("Copying binaries...")
    copy_binaries(package_dir, build_dir)
    
    print("Copying libraries...")
    copy_libraries(package_dir)
    
    print("Copying documentation...")
    copy_documentation(package_dir)
    
    print("Copying scripts...")
    copy_scripts(package_dir)
    
    print("Creating examples...")
    create_examples(package_dir)
    
    print("Creating install script...")
    create_install_script(package_dir)
    
    # Create packages
    checksums = {}
    
    # ZIP package (universal)
    zip_file = dist_dir / f"{package_name}.zip"
    checksums[zip_file.name] = create_zip_package(package_dir, zip_file)
    
    # TAR.GZ package (Unix-like systems)
    if args.platform in ['linux', 'macos']:
        tar_file = dist_dir / f"{package_name}.tar.gz"
        checksums[tar_file.name] = create_tar_package(package_dir, tar_file)
    
    # Create checksums file
    create_checksums_file(dist_dir, checksums)
    
    # Cleanup temporary directory
    shutil.rmtree(package_dir)
    
    print(f"\n✅ Packaging completed successfully!")
    print(f"Output directory: {dist_dir}")
    print(f"Packages created:")
    for filename in checksums.keys():
        file_path = dist_dir / filename
        size_mb = file_path.stat().st_size / (1024 * 1024)
        print(f"  - {filename} ({size_mb:.1f} MB)")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())