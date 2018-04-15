# Tiny3D

基于GDI重新写了个软光栅，API设计表面类似D3D9 ，固定管线 。

## Environment

- C++ 11 support at least
- Windows 

## Features

- 固定pipeline
- d3d9-like functions , 左手系, 矩阵行占优，右乘列向量
- 变换：世界 ->视窗 -> 投影到CVV -> 归一化 -> 屏幕映射 ->光栅化
- 简单CVV 裁剪（一点出界整个三角形剔除）
- 背面剔除
- bersenham 扫描转换,已完成线框部分
- FPS 显示

## TODO：

- 纹理 
- 色彩
- Alpha 融合
- Z-buffer
- CVV中三角形的部分裁剪、分割
- ...


## Example

