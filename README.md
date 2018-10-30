# 1. 简介

## 1.1 LearnOpenGL

>https://learnopengl-cn.github.io/
>
>欢迎来到OpenGL的世界。这个工程只是我([Joey de Vries](http://joeydevries.com/))的一次小小的尝试，希望能够建立起一个完善的OpenGL教学平台。无论你学习OpenGL是为了学业，找工作，或仅仅是因为兴趣，这个网站都将能够教会你**现代**(Core-profile) OpenGL从基础，中级，到高级的知识。LearnOpenGL的目标是使用易于理解的形式，使用清晰的例子，展现现代OpenGL的所有知识点，并与此同时为你以后的学习提供有用的参考。

## 1.2 项目

这是我对 LearnOpenGL 所做的练习

内容基于教程，代码为自行编写，主要目标如下

- 熟悉OpenGL
- 提高C++编程技巧
- 锻炼CMake搭工程能力
- 完善泛用的 Utility 代码

## 1.3 工程结构

```
/01_Introduction
  -01_CreateWindow
  -02_Triangle
  -03_Shader
  -04_Texture
  -05_Transform
  -06_Croodinate
  -07_Camera
/02_Lighting
  -01_Color
  -02_BasicLighting
  -03_Material
  -04_LightingMap
  -05_LightCaster
  -06_MultiLight
/03_LoadModel
  -01_LoadModel
/04_AdvanceOGL
  -01_DepthTest
  -02_StencilTest
  -03_Blend
  -04_FrameBuffer
  -05_CubeMap
  -06_UBO
  -07_GS
  -08_Instance
/05_AdvanceLighting
  -01_BlinnPhong
  -02_Gamma
  -03_ShadowMap
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

## 2.1 下载代码

```bash
git clone https://github.com/Ubpa/LearnOpenGL
```

## 2.2 建立工程

```bash
mkdir build
cd build
cmake ..
```

打开 `/build/LearnOpenGL.sln`即可

