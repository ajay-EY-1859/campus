import os
import re

INCLUDE_DIR = 'include/'
REQUIRED_INCLUDES = {
    'auth.h': ['config.h'],
    'fileio.h': ['auth.h', 'student.h'],
    'student.h': ['config.h'],
    'utils.h': [],
    'ui.h': ['config.h'],
}

def lint_file(filepath, filename):
    issues = []
    with open(filepath, 'r') as f:
        lines = f.readlines()
    content = ''.join(lines)

    # 🔐 Include guard check
    if not re.search(r'#ifndef\s+[A-Z_]+\s*\n#define\s+[A-Z_]+', content):
        issues.append("❌ Missing or incorrect include guard")

    # ⚠️ Function declaration check
    for i, line in enumerate(lines):
        if '(' in line and ';' in line:
            if not re.match(r'^\s*(void|int|float|char|const char\*|[\w\s\*]+)\s+\**\w+\s*\(.*\)\s*;', line.strip()):
                issues.append(f"⚠️ Possibly malformed function at line {i+1}: {line.strip()}")

    # 🔎 Include detection
    includes = [l.strip() for l in lines if l.strip().startswith('#include')]
    for required in REQUIRED_INCLUDES.get(filename, []):
        if not any(required in inc for inc in includes):
            issues.append(f"⚠️ Missing required include: {required}")

    # 🧹 Unused include check (basic)
    for inc in includes:
        match = re.search(r'#include\s+"(\w+\.h)"', inc)
        if match:
            included_file = match.group(1).replace('.h', '')
            if included_file not in content:
                issues.append(f"🧹 Possibly unused include: {match.group(1)}")

    return issues

# 🚀 Run on all .h files
for fname in os.listdir(INCLUDE_DIR):
    if fname.endswith('.h'):
        path = os.path.join(INCLUDE_DIR, fname)
        results = lint_file(path, fname)

        print(f"\n📄 {fname}")
        if results:
            for r in results:
                print(r)
        else:
            print("✅ Clean")
