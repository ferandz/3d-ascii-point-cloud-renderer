# ASCII Object Renderer in C 💻

**ASCII Object Renderer** — It's a simple pseudo-3D drawing program written in pure C (C99) without any graphics APIs (OpenGL, DirectX, Vulkan, etc.).  
The project was started as an experiment inspired by the YouTube channel [Code Fiction](https://youtube.com), but it has received significant updates.

> **⚠️ WARNING:** This project was created purely as an experiment, so it may contain suboptimal ("bad") code and questionable design choices.

The program reads spatial coordinates for vertices and edges from `.obj` files and projects them onto a 2D console screen with real-time depth mapping.

## ## Features
- **`.obj` File Parsing:** The engine automatically counts vertices and edges, allocates RAM dynamically, and parses the coordinates.
- **Dynamic Pseudo-Depth Shading:** Render characters (`#`, `*`, `.`, `:`) are calculated automatically based on the 'z' coordinate after rotation, creating a visual depth effect.
- **Wireframe Rendering:** Supports drawing both vertices (points) and edges (lines connecting them).
- **Guaranteed Cross-Platform Compatibility:**
  - On **Windows**, ANSI escape sequence support is automatically enabled via WinAPI (`windows.h`).
  - On **POSIX-compliant OSs** (Linux, macOS), standard system timers via `usleep` are used.

## 🛠 Tech Stack
- **Programming Language:** C (C99 std)
- **Libraries:**
  - stdlibc: `stdio.h`, `math.h`, `string.h`, `stdint.h`
  - System: `unistd.h` (POSIX-compatibility OS) & `windows.h` (Windows)

## 🚀 Compile and Run
1. You need a C compiler (MSVC, Clang, GCC) and CMake installed.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running the application:
- On Windows (CMD/PowerShell):
  ```bash
  ASCIIPCR.exe
  ```
- On Linux/macOS:
  ```bash
  ./ASCIIPCR
  ```
