# Foxglove SLAM Visualization

这是一个使用 Foxglove SDK 进行 SLAM（同时定位与地图构建）数据可视化的 C++ 项目。该项目无需 ROS/ROS2，可将 SLAM 数据流传输到 Foxglove Studio 进行实时可视化。

## 功能特性

- 实时 SLAM 数据可视化（位姿、点云、坐标变换）
- 使用 Foxglove SDK 通过 WebSocket 通信
- 使用 PCL 生成密集运动点云数据
- 提供模拟 SLAM 处理器用于演示
- 与 Foxglove Studio 兼容

## 依赖项

- C++17 或更高版本  
- CMake 3.20 或更高版本  
- Boost（system、thread）  
- Protobuf  
- PCL（Point Cloud Library）  
- Foxglove SDK（预构建归档，需手动准备）

## 关于 Foxglove SDK 的处理（重要）

- 本项目的 CMake 配置会在项目源目录下的 thirdparty/foxglove-src 中查找 Foxglove SDK（即 include/ 和 lib/ 等内容）。  
- 脚本 scripts/fetch_and_prepare_foxglove.sh 用于下载、校验并解压预构建 SDK 到 thirdparty/foxglove-src（默认目标）。  
- CMake 不会自动下载 SDK；如果 SDK 缺失，CMake 配置阶段会失败并提示手动运行该脚本。

## 快速操作步骤（在项目根目录）

1. 给予脚本执行权限（只需执行一次）：
   bash -c "chmod +x scripts/fetch_and_prepare_foxglove.sh"

2. 使用脚本准备 SDK（默认目标：项目根的 thirdparty/foxglove-src）：
   bash -c "scripts/fetch_and_prepare_foxglove.sh \
     https://github.com/foxglove/foxglove-sdk/releases/download/sdk%2Fv0.16.2/foxglove-v0.16.2-cpp-x86_64-unknown-linux-gnu.zip \
     12ccf93169a800496d7e0f4428127e92cb00e1862a3e609f141a2aaae0c8946f"

   或者显式指定目标目录（例如仍放到 thirdparty）：
   bash -c "scripts/fetch_and_prepare_foxglove.sh <URL> <SHA256> ${PWD}/thirdparty/foxglove-src"

   说明：脚本会下载 ZIP、校验 SHA256、解压并将包含 include/ 和 lib/ 的 SDK 根目录移动到目标目录。

3. 配置并构建：
   mkdir -p build && cd build
   cmake ..
   cmake --build .

如果配置阶段报错提示未找到 Foxglove SDK，请确认已按第 2 步运行脚本并将 SDK 放置在 project_root/thirdparty/foxglove-src（或你在 CMake 中指定的其他目录）。

## 运行

在构建目录运行生成的可执行文件：
./slam_foxglove

默认会在端口 8765 启动 WebSocket 服务。打开 Foxglove Studio，添加连接：ws://localhost:8765

## 数据流说明

- /slam/pose：位姿（foxglove.PoseInFrame）  
- /slam/pointcloud：点云（foxglove.PointCloud），使用 PCL 生成的密集环境点云  
- /tf：坐标变换（foxglove.FrameTransform）

## 项目结构（概览）

- include/slam_processor.h  
- src/slam_processor.cpp  
- include/foxglove_bridge.h  
- src/foxglove_bridge.cpp  
- src/main.cpp  
- scripts/fetch_and_prepare_foxglove.sh

## 许可证

MIT
