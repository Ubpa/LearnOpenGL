# 1. 简介

## 1.1 LearnOpenGL

>https://learnopengl-cn.github.io/
>
>欢迎来到OpenGL的世界。这个工程只是我([Joey de Vries](http://joeydevries.com/))的一次小小的尝试，希望能够建立起一个完善的OpenGL教学平台。无论你学习OpenGL是为了学业，找工作，或仅仅是因为兴趣，这个网站都将能够教会你**现代**(Core-profile) OpenGL从基础，中级，到高级的知识。LearnOpenGL的目标是使用易于理解的形式，使用清晰的例子，展现现代OpenGL的所有知识点，并与此同时为你以后的学习提供有用的参考。

## 1.2 项目

这是我对 LearnOpenGL 所做的练习

内容基于教程，代码为自行编写，主要目标如下

- 熟悉OpenGL
- 面向对象编程
- 锻炼CMake搭工程能力
- 完善泛用的 Utility 代码

## 1.3 工程结构

```
/LearnOpenGL
  /01_Introduction
    -01_CreateWindow
    -02_Triangle
    -03_Shader
  /LIBs
    /Utility
      -ArgManager
      -File
      -Operation
      -Image
    -glad
    -Glfw
    -Shader
```

# 2. 使用方法

### 2.1 下载数据文件

```
链接：https://pan.baidu.com/s/1TQOqNW7gNLE8Uxhl34hLWg 
提取码：ahmx
```

放在根目录下

### 2.2 建立工程

```bash
mkdir build
cd build
cmake ..
```

打开 `/build/LearnOpenGL.sln`，找到 `INSTALL` 项目，右键点击“生成”。

- 生成的`exe`会在`/bin`中

- 生成的`lib`会在 `/lib/Gen `中
- `*.fs`、`*.vs`会从`/src`中拷贝到`/data`中