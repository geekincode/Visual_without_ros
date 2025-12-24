#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <string>

// Include Foxglove SDK headers first
#include <foxglove/server.hpp>
#include <foxglove/schemas.hpp>

#include "slam_processor.h"

class FoxgloveBridge {
public:
    FoxgloveBridge(std::shared_ptr<SLAMProcessor> slam_processor);
    ~FoxgloveBridge();

    // 启动WebSocket服务器
    void start(uint16_t port = 8765);
    
    // 停止WebSocket服务器
    void stop();

private:
    std::shared_ptr<SLAMProcessor> slam_processor_;
    std::unique_ptr<foxglove::WebSocketServer> server_;
    std::thread broadcast_thread_;
    std::atomic<bool> running_;
    
    // 通道
    std::shared_ptr<foxglove::schemas::PoseInFrameChannel> pose_channel_;
    std::shared_ptr<foxglove::schemas::PointCloudChannel> pointcloud_channel_;
    std::shared_ptr<foxglove::schemas::FrameTransformChannel> tf_channel_;
    
    // 初始化通道
    void initializeChannels();
    
    // 广播循环
    void broadcastLoop();
    
    // 发送各种数据
    void sendPose();
    void sendPointCloud();
    void sendTransform();
    
    // 序列化函数
    std::vector<uint8_t> serializePointCloud(const std::vector<Point3D>& points);
    std::vector<uint8_t> serializePose(const Pose& pose);
    std::vector<uint8_t> serializeTransform();
};