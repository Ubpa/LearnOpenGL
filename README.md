# 1. 简介

## 1.1 LearnOpenGL

>https://learnopengl-cn.github.io/
>

## 1.2 项目

这是我对 LearnOpenGL 所做的练习

内容基于教程，代码大量重构与更改

主要目标如下

- 熟悉OpenGL
- 提高C++编程技巧
- 锻炼 CMake 搭工程能力
- 编写好用的 Utility 部分的代码

## 1.3 笔记

详见 `notes`

## 1.4 工程结构

```
/01_Introduction
  -01_01_CreateWindow
  -01_02_Triangle
  -01_03_Shader
  -01_04_Texture
  -01_05_Transform
  -01_06_Croodinate
  -01_07_Camera
/02_Lighting
  -02_01_Color
  -02_02_BasicLighting
  -02_03_Material
  -02_04_LightingMap
  -02_05_LightCaster
  -02_06_MultiLight
/03_LoadModel
  -03_01_LoadModel
/04_AdvanceOGL
  -04_01_DepthTest
  -04_02_StencilTest
  -04_03_Blend
  -04_04_FrameBuffer
  -04_05_CubeMap
  -04_06_UBO
  -04_07_GS
  -04_08_Instance
/05_AdvanceLighting
  -05_01_BlinnPhong
  -05_02_Gamma
  -05_03_ShadowMap
  -05_04_PointShadow
  -05_05_NormalMap
  -05_06_ParallaxMap
  -05_07_HDR
  -05_08_Bloom
  -05_09_DeferedShading
  -05_10_SSAO
/LIBs
  -glad
  /LOGL
    -Camera
    -FBO
    -Glfw
    -Mesh
    -Model
    -Shader
    -Texture
    -VAO
  /Utility
    -ArgManager
    -Config
    -EventManager
    -File
    -Header: Slogleton, Storage, Pool, Vec, Array2D 
    -Image
    -Operation: Operation, LambdaOp, InfoLambdaOp, OpQueue
    -Shape: Shape, Cube, Sphere
```

# 2. 使用方法

## 2.1 需求软件

- Visual Studio 2017（其他版本没有测试）
- Git
- CMake

## 2.2 下载代码

```bash
git clone https://github.com/Ubpa/LearnOpenGL
```

## 2.3 编译

1. 建立工程

```bash
# 在 cmd 中运行如下命令
mkdir build
cd build
cmake ..
```

2. 打开 `/build/LearnOpenGL.sln`

3. 在**解决方案中**的`CMakePredefinedTargets`中，右键`INSTALL`，点击生成

## 2.4 运行

双击根目录下的`run_all.bat`，即可运行所有`exe`

> 按`ESC`可退出`exe`