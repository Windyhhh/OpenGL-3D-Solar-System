# 🌌 OpenGL 3D Solar System | 3D 球面建模与太阳系模拟系统

> **Real-time 3D solar system simulation built with OpenGL. Sphere modeling, planetary orbits, texture mapping, lighting, and interactive camera controls. From-scratch OpenGL graphics rendering project.**
>
> 基于 OpenGL 的实时 3D 太阳系模拟系统。球面建模、行星轨道、纹理映射、光照和交互式相机控制。从零实现的 OpenGL 图形渲染项目。

---

## 🌟 Features | 核心特性

- **OpenGL Rendering** — Real-time 3D graphics pipeline
- **Sphere Modeling** — Parametric sphere generation
- **Planetary Orbits** — Keplerian orbit simulation
- **Texture Mapping** — Planet surface textures
- **Lighting System** — Phong shading, sun as light source
- **Interactive Camera** — Mouse/keyboard camera controls
- **Solar System** — Sun + 8 planets with realistic scales

---

## 🚀 Quick Start | 快速开始

```bash
# Build (CMake)
mkdir build && cd build
cmake ..
make

# Run
./SolarSystem
```

### Controls | 操作

| Key | Action |
|-----|--------|
| **W/A/S/D** | Move camera |
| **Mouse** | Rotate view |
| **Scroll** | Zoom in/out |
| **Space** | Pause/Resume |
| **1-8** | Focus on planet |

---

## 🪐 Planets | 行星

| Planet | Orbit Period | Rotation | Moons |
|--------|-------------|----------|-------|
| **Mercury** | 88 days | 59 days | 0 |
| **Venus** | 225 days | 243 days | 0 |
| **Earth** | 365 days | 24h | 1 |
| **Mars** | 687 days | 24.6h | 2 |
| **Jupiter** | 12 years | 10h | 95 |
| **Saturn** | 29 years | 10.7h | 146 |
| **Uranus** | 84 years | 17h | 28 |
| **Neptune** | 165 years | 16h | 16 |

---

## 📄 License | 许可证

MIT License.

[GitHub](https://github.com/Windyhhh/OpenGL-3D-Solar-System)
