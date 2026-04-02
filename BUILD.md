# ⚒️ Building Moonlight Engine

This guide provides comprehensive instructions for building the engine from source on both Windows and Linux platforms.

## 📌 Table of Contents

- [Prerequisites](#prerequisites)
- [Quick Start](#quick-start)
- [Detailed Build Instructions](#detailed-build-instructions)
- [Project Structure](#project-structure)
- [Getting Help](#getting-help)

---

## 📋 Prerequisites

### Development Tools

#### Windows Requirements

| Tool                   | Version      | Download Link | Notes                                                                               |
|------------------------|--------------|---------------|-------------------------------------------------------------------------------------|
| **Visual Studio 2026** | Community+   | [Download](https://visualstudio.microsoft.com/downloads/) | Required: "Desktop development with C++" with MSVC v143+ and the Windows 10/11 SDK. |
| **Python**             | 3.8 or later | [Download](https://www.python.org/downloads/) | Must be added to PATH during installation                                           |

#### Linux Requirements

| Tool | Version | Installation Command | Notes |
|------|---------|---------------------|-------|
| **Clang** | 11.0 or later | `sudo apt install clang` (Ubuntu/Debian)<br>`sudo dnf install clang` (Fedora/CentOS)<br>`sudo pacman -S clang` (Arch) | C++ compiler |
| **Make** | Latest | `sudo apt install make` (Ubuntu/Debian)<br>Usually pre-installed | Build system |
| **Python** | 3.8 or later | `sudo apt install python3` (Ubuntu/Debian)<br>Usually pre-installed | Required for build scripts |

#### Cross-Platform Requirements

| Tool | Version | Download/Installation |
|------|---------|----------------------|
| **Git** | Latest | [Download](https://git-scm.com/downloads) (Windows)<br>`sudo apt install git` (Linux) | For cloning the repository |

---

## ⚡ Quick Start

### Windows (Visual Studio)
```batch
git clone https://github.com/Ayydxn/MoonlightEngine.git
cd "Moonlight Engine"
Setup.bat
# Open Moonlight Engine.sln in Visual Studio
# Build and run
```

### Linux (Terminal)
```bash
git clone https://github.com/Ayydxn/MoonlightEngine.git
cd MoonlightEngine
chmod +x Setup.sh && ./Setup.sh
make config=debug
./Binaries/Editor/Debug-Linux-x86_64/MoonlightEditor
```

---

## 🛠️ Detailed Build Instructions

### Step 1: Clone the Repository

```bash
git clone https://github.com/Ayydxn/MoonlightEngine.git
cd "MoonlightEngine"  # Windows
# or
cd MoonlightEngine     # Linux
```

### Step 2: Run Setup Script

The setup script handles dependency installation and project file generation.

#### Windows
```batch
# Double-click Setup.bat or run from command prompt/PowerShell
Setup.bat
```

#### Linux
```bash
# Make executable and run
chmod +x Setup.sh
./Setup.sh
```

**What the setup script does:**
- Checks Python installation
- Installs required Python packages
- Generates platform-specific project files:
  - **Windows**: Visual Studio solution (`.slnx`/`.vcxproj`)
  - **Linux**: Makefiles with Clang configuration

### Step 3: Build the Engine

#### Windows (Visual Studio IDE)

1. **Open the solution:**
   ```
   Moonlight Engine.slnx
   ```

2. **Configure build settings:**
   - **Configuration**: Debug / Release / Distribution
   - **Platform**: x64

3. **Build:**
   - Press `F7`
   - Or: **Build → Build Solution**
   - Or: Right-click solution → **Build**

#### Linux (Command Line)

```bash
# From project root directory

# Debug build (with symbols for development)
make config=debug

# Release build (optimized)
make config=release

# Distribution build (fully optimized)
make config=distribution
```

### Step 4: Run the Editor

#### Windows
1. In Visual Studio, right-click **Moonlight Editor** project
2. Select **Set as Startup Project**
3. Press `F5` to debug or `Ctrl+F5` to run

#### Linux
Execute the built binary directly:

```bash
# Debug build
./Binaries/Editor/Debug-Linux-x86_64/MoonlightEditor

# Release build
./Binaries/Editor/Release-Linux-x86_64/MoonlightEditor

# Distribution build
./Binaries/Editor/Distribution-Linux-x86_64/MoonlightEditor
```

---

## 📂 Project Structure

```
Moonlight Engine/
├── Engine/                 # Core engine library
├── Editor/                 # Editor application
├── ThirdParty/             # External dependencies
└── Scripts/                # Build and utility scripts
```

---

## ❗ Getting Help

### Issue Reporting
- Check existing [issues](https://github.com/Ayydxn/MoonlightEngine/issues) first
- Create new issue with:
  - Operating system and version
  - Compiler/toolchain used
  - Full error messages/log files (found in the `Editor/Logs` directory)
  - Steps to reproduce

---

**Note:** Remember that the engine is in active development. Some instability may occur as not all environments/hardware configurations/use cases are accounted for. Report any issues you encounter to help improve the engine!
