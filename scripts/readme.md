# 🛠️ Scripts - Campus Management System Automation

This directory contains automation scripts and utilities for maintaining, building, and deploying the Campus Management System.

## 📜 Available Scripts

### 🔍 `lint_headers.py`
**Purpose**: Code quality analysis for header files
- Checks for missing include guards
- Validates function declarations
- Identifies missing/unused includes
- Ensures coding standards compliance

**Usage:**
```bash
python scripts/lint_headers.py
```

### 🔧 `fix_headers.py`
**Purpose**: Automatic header file corrections
- Fixes missing include statements
- Adds proper include guards
- Corrects header dependencies
- Maintains consistent formatting

**Usage:**
```bash
python scripts/fix_headers.py
```

### 🚀 `build.py` (New)
**Purpose**: Cross-platform build automation
- Automated CMake configuration
- Multi-platform compilation
- Dependency checking
- Build optimization

**Usage:**
```bash
python scripts/build.py [--release|--debug] [--clean]
```

### 📦 `package.py` (New)
**Purpose**: Release packaging automation
- Creates distribution packages
- Generates checksums
- Prepares release notes
- Archives documentation

**Usage:**
```bash
python scripts/package.py --version 2.0.0
```

### 🧪 `test_runner.py` (New)
**Purpose**: Automated testing framework
- Runs unit tests
- Integration testing
- Performance benchmarks
- Coverage reports

**Usage:**
```bash
python scripts/test_runner.py [--unit|--integration|--all]
```

### 📊 `generate_docs.py` (New)
**Purpose**: Documentation generation
- API documentation from headers
- Code metrics and statistics
- Dependency graphs
- README updates

**Usage:**
```bash
python scripts/generate_docs.py
```

## 🏗️ Project Structure

```
scripts/
├── lint_headers.py      # Code quality checks
├── fix_headers.py       # Automatic fixes
├── build.py            # Build automation
├── package.py          # Release packaging
├── test_runner.py      # Testing framework
├── generate_docs.py    # Documentation generator
├── utils/              # Shared utilities
│   ├── __init__.py
│   ├── file_utils.py   # File operations
│   ├── build_utils.py  # Build helpers
│   └── git_utils.py    # Git operations
└── README.md           # This file
```

## 🔧 Development Workflow

### Daily Development
```bash
# Check code quality
python scripts/lint_headers.py

# Fix common issues
python scripts/fix_headers.py

# Build and test
python scripts/build.py --debug
python scripts/test_runner.py --unit
```

### Release Preparation
```bash
# Full testing
python scripts/test_runner.py --all

# Generate documentation
python scripts/generate_docs.py

# Create release package
python scripts/package.py --version x.y.z
```

## 📋 Requirements

### Python Dependencies
```bash
pip install -r scripts/requirements.txt
```

**Required packages:**
- `pathlib` - Path operations
- `subprocess` - System commands
- `argparse` - Command line parsing
- `json` - Configuration handling
- `hashlib` - Checksum generation

### System Requirements
- Python 3.7+
- CMake 3.10+
- Git (for version control operations)
- Platform-specific build tools

## ⚙️ Configuration

### `config.json`
```json
{
  "project": {
    "name": "Campus Management System",
    "version": "2.0.0",
    "author": "Campus Contributors"
  },
  "build": {
    "cmake_args": ["-DCMAKE_BUILD_TYPE=Release"],
    "parallel_jobs": 4
  },
  "packaging": {
    "include_docs": true,
    "include_examples": true,
    "compression": "zip"
  }
}
```

## 🚀 CI/CD Integration

### GitHub Actions
These scripts integrate with GitHub Actions workflows:
- Automated building on push
- Testing on pull requests
- Release packaging on tags
- Documentation updates

### Local Hooks
```bash
# Install pre-commit hooks
python scripts/install_hooks.py
```

## 🧪 Testing Scripts

### Unit Testing
- Individual function testing
- Mock data generation
- Error condition testing
- Performance benchmarks

### Integration Testing
- End-to-end workflows
- Database operations
- File I/O testing
- Security validation

## 📈 Metrics & Analytics

### Code Quality Metrics
- Lines of code
- Cyclomatic complexity
- Test coverage
- Documentation coverage

### Build Metrics
- Build time tracking
- Binary size analysis
- Dependency analysis
- Performance profiling

## 🔒 Security Scripts

### Security Scanning
- Static code analysis
- Dependency vulnerability checks
- Security policy validation
- Audit log analysis

### Compliance Checking
- License compliance
- Code standard adherence
- Documentation requirements
- Security best practices

## 🌐 Platform Support

### Windows
- PowerShell integration
- MSVC build support
- Windows-specific packaging

### Linux
- Shell script compatibility
- GCC/Clang support
- Package manager integration

### macOS
- Xcode integration
- Homebrew compatibility
- App bundle creation

## 📚 Adding New Scripts

### Script Template
```python
#!/usr/bin/env python3
"""
Script description here.
"""

import argparse
import sys
from pathlib import Path

def main():
    parser = argparse.ArgumentParser(description="Script description")
    parser.add_argument("--option", help="Option description")
    args = parser.parse_args()
    
    # Script logic here
    
if __name__ == "__main__":
    main()
```

### Best Practices
- Use argparse for command line arguments
- Include proper error handling
- Add logging for debugging
- Follow PEP 8 style guidelines
- Include docstrings and comments

## 🤝 Contributing

### Script Contributions
1. Follow the existing code style
2. Add comprehensive error handling
3. Include usage documentation
4. Test on multiple platforms
5. Update this README

### Testing New Scripts
```bash
# Test script functionality
python scripts/your_script.py --help
python scripts/your_script.py --test-mode
```

## 📞 Support

### Script Issues
- Check script logs in `logs/scripts/`
- Verify Python dependencies
- Ensure proper permissions
- Check system requirements

### Getting Help
- GitHub Issues for bug reports
- Discussions for feature requests
- Documentation for usage help
- Community for general support

---

**These scripts help maintain code quality and automate development workflows for the Campus Management System project! 🚀**