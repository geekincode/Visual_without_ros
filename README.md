# Foxglove SLAM Visualization

一个使用Foxglove SDK进行SLAM（同时定位与地图构建）数据可视化的C++项目。该项目无需ROS/ROS2，可以直接将SLAM数据流传输到Foxglove Studio进行实时可视化。

## 功能特性

- 实时SLAM数据可视化（位姿、点云、坐标变换）
- 使用Foxglove SDK进行WebSocket通信
- 模拟SLAM处理器用于演示
- 与Foxglove Studio完全兼容

## 依赖项

- C++17 或更高版本
- CMake 3.20 或更高版本
- Boost (system, thread)
- Protobuf
- Foxglove SDK (自动下载)

## 构建

```bash
mkdir build
cd build
cmake ..
make
```

或者使用提供的构建脚本：

```bash
./build.sh
```

## 运行

```bash
./build/slam_foxglove
```

程序启动后，将在端口8765上运行WebSocket服务器。打开Foxglove Studio，添加连接，地址为 `ws://localhost:8765`。

## 数据流

项目包含以下数据流：

- `/slam/pose`: 位姿信息 (foxglove.PoseInFrame)
- `/slam/pointcloud`: 点云数据 (foxglove.PointCloud)
- `/tf`: 坐标变换 (foxglove.FrameTransform)

## Foxglove SDK

本项目使用Foxglove SDK进行通信。SDK通过CMake的FetchContent模块自动下载。项目优先使用本地已下载的SDK文件，避免重复下载。

## 项目结构

- `include/slam_processor.h`: 模拟SLAM处理器头文件
- `src/slam_processor.cpp`: 模拟SLAM处理器实现
- `include/foxglove_bridge.h`: Foxglove桥接器头文件
- `src/foxglove_bridge.cpp`: Foxglove桥接器实现
- `src/main.cpp`: 主程序入口

## 使用场景

此项目适用于：

- SLAM算法开发与调试
- 机器人定位可视化
- 点云数据展示
- 传感器融合算法验证

## 许可证

MIT
