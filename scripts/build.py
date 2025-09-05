#!/usr/bin/env python3
"""
Cross-platform build automation script for Campus Management System.
Handles CMake configuration, compilation, and packaging.
"""

import argparse
import os
import sys
import subprocess
import shutil
from pathlib import Path

def run_command(cmd, cwd=None):
    """Execute a command and return success status."""
    try:
        result = subprocess.run(cmd, shell=True, cwd=cwd, check=True, 
                              capture_output=True, text=True)
        print(result.stdout)
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        print(f"Output: {e.stdout}")
        print(f"Error: {e.stderr}")
        return False

def clean_build_dir(build_dir):
    """Clean the build directory."""
    if build_dir.exists():
        print(f"Cleaning build directory: {build_dir}")
        shutil.rmtree(build_dir)
    build_dir.mkdir(parents=True, exist_ok=True)

def configure_cmake(source_dir, build_dir, build_type="Release"):
    """Configure CMake build."""
    print(f"Configuring CMake for {build_type} build...")
    
    cmake_args = [
        "cmake",
        f"-DCMAKE_BUILD_TYPE={build_type}",
        f"-S{source_dir}",
        f"-B{build_dir}"
    ]
    
    cmd = " ".join(cmake_args)
    return run_command(cmd)

def build_project(build_dir, parallel_jobs=4):
    """Build the project."""
    print(f"Building project with {parallel_jobs} parallel jobs...")
    
    cmd = f"cmake --build {build_dir} --parallel {parallel_jobs}"
    return run_command(cmd)

def copy_dependencies(build_dir):
    """Copy required DLL files to build output."""
    lib_dir = Path("lib")
    if not lib_dir.exists():
        print("Warning: lib directory not found")
        return True
    
    # Find output directory
    output_dirs = [
        build_dir / "Debug",
        build_dir / "Release", 
        build_dir / "build" / "Debug",
        build_dir / "build" / "Release"
    ]
    
    for output_dir in output_dirs:
        if output_dir.exists():
            print(f"Copying dependencies to {output_dir}")
            for dll_file in lib_dir.glob("*.dll"):
                shutil.copy2(dll_file, output_dir)
                print(f"Copied {dll_file.name}")
            return True
    
    print("Warning: Could not find output directory for dependencies")
    return True

def create_directories():
    """Create required runtime directories."""
    dirs = ["data", "credentials", "logs"]
    for dir_name in dirs:
        Path(dir_name).mkdir(exist_ok=True)
        print(f"Created directory: {dir_name}")

def main():
    parser = argparse.ArgumentParser(description="Build Campus Management System")
    parser.add_argument("--build-type", choices=["Debug", "Release"], 
                       default="Release", help="Build type")
    parser.add_argument("--clean", action="store_true", 
                       help="Clean build directory first")
    parser.add_argument("--jobs", type=int, default=4, 
                       help="Number of parallel build jobs")
    parser.add_argument("--build-dir", default="build", 
                       help="Build directory name")
    
    args = parser.parse_args()
    
    # Get project root directory
    project_root = Path(__file__).parent.parent
    build_dir = project_root / args.build_dir
    
    print(f"Campus Management System Build Script")
    print(f"Project root: {project_root}")
    print(f"Build directory: {build_dir}")
    print(f"Build type: {args.build_type}")
    
    # Change to project root
    os.chdir(project_root)
    
    # Clean build directory if requested
    if args.clean:
        clean_build_dir(build_dir)
    else:
        build_dir.mkdir(exist_ok=True)
    
    # Configure CMake
    if not configure_cmake(project_root, build_dir, args.build_type):
        print("CMake configuration failed!")
        return 1
    
    # Build project
    if not build_project(build_dir, args.jobs):
        print("Build failed!")
        return 1
    
    # Copy dependencies
    if not copy_dependencies(build_dir):
        print("Warning: Failed to copy some dependencies")
    
    # Create runtime directories
    create_directories()
    
    print("\n✅ Build completed successfully!")
    print(f"Executable location: {build_dir}")
    
    # Show next steps
    print("\n📋 Next steps:")
    print("1. Run the application:")
    if os.name == 'nt':  # Windows
        print(f"   {build_dir}\\build\\{args.build_type}\\campus.exe")
    else:  # Linux/macOS
        print(f"   {build_dir}/campus")
    
    print("2. Run tests:")
    print("   python scripts/test_runner.py")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())