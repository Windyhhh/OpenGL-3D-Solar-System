<div align="center">

# 🪐 OpenGL-3D-Solar-System

### Real-time 3D solar system in OpenGL.

Sphere modelling, planet orbits and real-time graphics — a compiled C / GLFW application.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C](https://img.shields.io/badge/C-11-A8B9CC?logo=c&logoColor=black)](https://en.wikipedia.org/wiki/C_(programming_language))
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3-5586A4?logo=opengl&logoColor=white)](https://www.opengl.org/)
[![GLFW](https://img.shields.io/badge/GLFW-3-000000)](https://www.glfw.org/)

</div>

---

**OpenGL-3D-Solar-System** renders a real-time **3D solar system** with OpenGL — sphere geometry, planet orbits and a graphics engine, delivered as a compiled C / GLFW application.

> [!NOTE]
> 中文项目：OpenGL 3D 太阳系——实时 3D 图形、球体建模、行星轨道。

---

## Quickstart

```bash
git clone https://github.com/Windyhhh/OpenGL-3D-Solar-System.git
cd OpenGL-3D-Solar-System

# Windows prebuilt
./build/solar_system.exe
```

Recompile with the object files under `build/` (main, graphics_engine, mesh_loader, transform, sphere_geometry, input, celestial_system).

---

## Features

- **Real-time 3D** — OpenGL rendering engine.
- **Sphere modelling** — geometry / mesh loader.
- **Planet orbits** — animated solar system.

---

## Project Structure

```
OpenGL-3D-Solar-System/
├── build/                  # .o files + solar_system.exe, glfw3.dll
├── docs/                   # usage guide, completion report
└── PROJECT_STRUCTURE.md
```

---

## License

MIT — free to use, modify and distribute.
