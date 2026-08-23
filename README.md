# 🌌 OpenGL 3D 太阳系模拟 | OpenGL 3D Solar System Simulation

> **纯 C 语言 + OpenGL 从零构建 3D 太阳系——球面建模、纹理映射、光照渲染，九大行星实时运转。**
>
> *Build a 3D solar system from scratch with pure C + OpenGL — sphere modeling, texture mapping, lighting, nine planets in real-time orbit.*

---

## ⭐ 核心卖点 | Why Star This

| 卖点 | Feature | 一句话 |
|------|---------|--------|
| 🎨 **从零构建** | From Scratch | 不依赖游戏引擎，纯 C + OpenGL 手写渲染管线 |
| 🌍 **球面建模** | Sphere Modeling | 参数化球面网格生成，UV 纹理坐标计算 |
| 🪐 **九大行星** | Nine Planets | 水星到冥王星，各自轨道、自转、纹理 |
| 💡 **光照系统** | Lighting System | 太阳作为点光源，漫反射 + 镜面反射 |
| 🎮 **交互控制** | Interactive | 鼠标旋转视角、滚轮缩放、键盘切换 |

---

## 🏆 技术栈 | Tech Stack

![C](https://img.shields.io/badge/C-99+-blue?logo=c)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-red?logo=opengl)
![GLFW](https://img.shields.io/badge/GLFW-3.3+-green?logo=glfw)
![CMake](https://img.shields.io/badge/CMake-3.10+-orange?logo=cmake)

---

## 📊 项目亮点 | Highlights

| 特性 | 实现 |
|------|------|
| 球面网格 | 参数化经纬度网格生成 |
| 纹理映射 | BMP 纹理加载 + UV 坐标 |
| 光照模型 | Phong 光照（环境光+漫反射+高光） |
| 轨道系统 | 开普勒轨道参数 + 实时计算 |
| 相机控制 | 欧拉角相机 + 鼠标交互 |
| 天空盒 | 星空背景渲染 |

---

## 🚀 快速开始 | Quick Start

```bash
git clone https://github.com/Windyhhh/OpenGL-3D-Solar-System.git
cd OpenGL-3D-Solar-System

# Windows (MinGW)
gcc -o solar_system src/*.c -Iinclude -Llib -lglfw3 -lopengl32 -lgdi32

# 运行
./solar_system
```

### 操作说明 | Controls

| 按键 | 功能 |
|------|------|
| 鼠标拖拽 | 旋转视角 |
| 滚轮 | 缩放 |
| W/A/S/D | 移动相机 |
| 空格 | 暂停/继续 |
| 1-9 | 聚焦对应行星 |

---

## 📂 项目结构 | Project Structure

```
OpenGL-3D-Solar-System/
├── src/
│   ├── main.c                 # 主程序入口
│   ├── graphics_engine.c      # 图形引擎
│   ├── celestial_system.c     # 天体系统
│   ├── sphere_geometry.c      # 球面几何
│   ├── mesh_loader.c          # 网格加载
│   ├── transform.c            # 变换矩阵
│   └── input.c                # 输入处理
├── include/                   # 头文件
├── lib/                       # 库文件
├── resources/                 # 纹理资源
│   ├── sun.bmp
│   ├── earth.bmp
│   ├── mars.bmp
│   └── ...
├── docs/                      # 文档
└── build/                     # 编译产物
```

---

## 🔬 核心技术 | Core Technology

### 球面参数化 | Sphere Parametrization

```c
// 经纬度球面网格生成
for (int lat = 0; lat <= LAT_SEGMENTS; lat++) {
    float theta = lat * PI / LAT_SEGMENTS;
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);
    for (int lon = 0; lon <= LON_SEGMENTS; lon++) {
        float phi = lon * 2 * PI / LON_SEGMENTS;
        float x = cos(phi) * sinTheta;
        float y = cosTheta;
        float z = sin(phi) * sinTheta;
        // UV 坐标
        float u = (float)lon / LON_SEGMENTS;
        float v = (float)lat / LAT_SEGMENTS;
    }
}
```

### Phong 光照模型 | Phong Lighting

```
环境光:  I_a = k_a * I_ambient
漫反射:  I_d = k_d * I_light * max(0, N·L)
镜面反射: I_s = k_s * I_light * max(0, R·V)^n
总光照:  I = I_a + I_d + I_s
```

---

## 🎯 应用场景 | Use Cases

- 🎓 **计算机图形学教学**：OpenGL 入门的完整实战项目
- 🌌 **天文科普**：太阳系运行的可视化演示
- 🎮 **游戏开发基础**：3D 渲染引擎的核心组件学习
- 🎨 **创意编程**：生成艺术和视觉创作的基础

---

## 📄 License

MIT License — 自由使用、修改和分发。

---

> 💡 **从零构建 3D 太阳系，Star ⭐ 支持开源图形学！**
