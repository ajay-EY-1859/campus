import os

INCLUDE_DIR = 'include/'
REQUIRED_INCLUDES = {
    'auth.h': ['config.h'],
    'fileio.h': ['auth.h', 'student.h', 'config.h'],
    'student.h': ['config.h'],
    'utils.h': [],
    'ui.h': ['config.h'],
}

def fix_includes(filepath, filename):
    with open(filepath, 'r') as f:
        lines = f.readlines()

    fixed = []
    inserted = []
    current_includes = [line.strip() for line in lines if line.strip().startswith('#include')]

    required = REQUIRED_INCLUDES.get(filename, [])
    for inc in required:
        inc_line = f'#include "{inc}"'
        if inc_line not in current_includes:
            inserted.append(inc_line)

    # Insert missing includes after first guard line
    idx = 0
    for i, line in enumerate(lines):
        if line.strip().startswith('#define'):
            idx = i + 1
            break

    fixed = lines[:idx] + [inc + '\n' for inc in inserted] + lines[idx:]

    if inserted:
        with open(filepath, 'w') as f:
            f.writelines(fixed)
        print(f"📄 {filename}: Inserted ➕ {', '.join(inserted)}")
    else:
        print(f"✅ {filename}: All includes present")

# 🔍 Run fix on all headers
for fname in os.listdir(INCLUDE_DIR):
    if fname.endswith('.h'):
        path = os.path.join(INCLUDE_DIR, fname)
        fix_includes(path, fname)
