# === Smart Cross-Platform Makefile ===

CC ?= gcc
SRC := $(wildcard src/main/*.c)
OUT := build/ajcampus

# Detect OS
OS := $(shell uname -s 2>/dev/null || echo Windows_NT)

ifeq ($(OS),Windows_NT)
    CC := cl
    CFLAGS := /I include
    OUT := build\\ajcampus.exe
    COMPILE = $(CC) $(CFLAGS) src\\main\\*.c /Fe$(OUT)
    MKDIR = if not exist build mkdir build
    RM = del /Q $(OUT) 2>nul || exit 0
else
    CFLAGS := -Wall -Wextra -Iinclude
    COMPILE = $(CC) $(CFLAGS) $(SRC) -o $(OUT)
    MKDIR = mkdir -p build
    RM = rm -f $(OUT)
endif

.PHONY: all run clean help

all:
    @echo 🔧 Building project using $(CC)
    @$(MKDIR)
    @$(COMPILE)

run: all
    @echo ▶️ Running...
    @$(OUT)

clean:
    @echo 🧹 Cleaning build directory...
    @$(RM)

help:
    @echo "Makefile targets:"
    @echo "  make         → build"
    @echo "  make run     → build and run"
    @echo "  make clean   → remove binary"
    @echo "  make help    → show this help"
