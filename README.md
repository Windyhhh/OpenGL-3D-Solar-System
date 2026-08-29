<div align="center">

# OpenGL 3D 太阳系 | OpenGL-3D-Solar-System

### Real-time 3D graphics in C with OpenGL (GLFW).

Sphere modeling, model loading and interactive rendering — extended with a realistic solar-system simulator.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C](https://img.shields.io/badge/C-11-A8B9CC?logo=c&logoColor=white)](https://en.cppreference.com/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3-5586A4?logo=opengl&logoColor=white)](https://www.opengl.org/)
[![GLFW](https://img.shields.io/badge/GLFW-3-1F1F1F)](https://www.glfw.org/)

</div>

---

**OpenGL-3D-Solar-System** is a real-time **3D graphics** application written in **C with OpenGL (GLFW)**. It builds sphere meshes from scratch, loads model files, and renders interactively — with textures, shadows and transforms, plus a full **solar-system simulator**.

> [!NOTE]
> 中文项目：基于 C + OpenGL (GLFW) 的 3D 球面建模与太阳系实时渲染——交互控制、纹理、阴影、几何变换。

---

## Features

- **Sphere mesh from scratch** — procedural 3D sphere modeling.
- **Model loading** — read model files for arbitrary meshes.
- **Real-time rendering** — multiple draw modes, texture mapping, shadows, transforms.
- **Interactive controls** — mouse / keyboard rotation, pan, zoom.
- **Solar-system simulator** — realistic multi-body scene with orbit rendering.

---

## Quickstart

```bash
git clone https://github.com/Windyhhh/OpenGL-3D-Solar-System.git
cd OpenGL-3D-Solar-System

# build with CMake / your GLFW + GLEW setup
cmake -B build && cmake --build build
./build/solar_system
```

---

## Project Structure

```
OpenGL-3D-Solar-System/
├── src/                    # C sources (sphere, model, renderer)
├── shaders/                # GLSL shaders
├── assets/                 # textures / models
├── CMakeLists.txt
└── docs/                   # usage, completion report, blog
```

---

## License

MIT — free to use, modify and distribute.
