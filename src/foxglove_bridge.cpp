#include "foxglove_bridge.h"
#include <foxglove/server.hpp>
#include <foxglove/schemas.hpp>

#include <iostream>
#include <thread>
#include <chrono>

FoxgloveBridge::FoxgloveBridge(std::shared_ptr<SLAMProcessor> slam_processor)
    : slam_processor_(slam_processor), server_(nullptr), running_(false) {
}

FoxgloveBridge::~FoxgloveBridge() {
    stop();
}

void FoxgloveBridge::start(uint16_t port) {
    if (running_) {
        std::cout << "Foxglove bridge is already running" << std::endl;
        return;
    }

    foxglove::WebSocketServerOptions server_options;
    server_options.port = port;
    server_options.host = "0.0.0.0";
    server_options.name = "SLAM Foxglove Bridge";
    
    auto result = foxglove::WebSocketServer::create(std::move(server_options));
    if (!result) {
        std::cerr << "Failed to create Foxglove server: " << static_cast<int>(result.error()) << std::endl;
        return;
    }
    
    server_ = std::make_unique<foxglove::WebSocketServer>(std::move(result.value()));

    // 初始化通道
    initializeChannels();

    // 启动广播线程
    running_ = true;
    broadcast_thread_ = std::thread(&FoxgloveBridge::broadcastLoop, this);

    std::cout << "Foxglove bridge started on port " << port << std::endl;
}

void FoxgloveBridge::stop() {
    if (running_) {
        running_ = false;
        
        if (broadcast_thread_.joinable()) {
            broadcast_thread_.join();
        }
        
        if (server_) {
            server_->stop();
        }
        
        std::cout << "Foxglove bridge stopped" << std::endl;
    }
}

void FoxgloveBridge::initializeChannels() {
    // 添加位姿通道
    auto pose_result = foxglove::schemas::PoseInFrameChannel::create("/slam/pose");
    if (pose_result) {
        pose_channel_ = std::make_shared<foxglove::schemas::PoseInFrameChannel>(std::move(*pose_result));
        std::cout << "Pose channel created" << std::endl;
    } else {
        std::cerr << "Failed to create pose channel: " << static_cast<int>(pose_result.error()) << std::endl;
    }

    // 添加点云通道
    auto pointcloud_result = foxglove::schemas::PointCloudChannel::create("/slam/pointcloud");
    if (pointcloud_result) {
        pointcloud_channel_ = std::make_shared<foxglove::schemas::PointCloudChannel>(std::move(*pointcloud_result));
        std::cout << "Point cloud channel created" << std::endl;
    } else {
        std::cerr << "Failed to create point cloud channel: " << static_cast<int>(pointcloud_result.error()) << std::endl;
    }

    // 添加坐标变换通道
    auto tf_result = foxglove::schemas::FrameTransformChannel::create("/tf");
    if (tf_result) {
        tf_channel_ = std::make_shared<foxglove::schemas::FrameTransformChannel>(std::move(*tf_result));
        std::cout << "Transform channel created" << std::endl;
    } else {
        std::cerr << "Failed to create transform channel: " << static_cast<int>(tf_result.error()) << std::endl;
    }
}

void FoxgloveBridge::broadcastLoop() {
    while (running_) {
        sendPose();
        sendPointCloud();
        sendTransform();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 每100ms发送一次
    }
}

void FoxgloveBridge::sendPose() {
    auto pose = slam_processor_->getCurrentPose();
    
    if (pose_channel_) {
        foxglove::schemas::PoseInFrame msg;
        msg.timestamp = foxglove::schemas::Timestamp{
            static_cast<uint32_t>(pose.timestamp / 1000000000),
            static_cast<uint32_t>(pose.timestamp % 1000000000)
        };
        msg.frame_id = "map";
        
        msg.pose = foxglove::schemas::Pose{};
        msg.pose->position = foxglove::schemas::Vector3{
            pose.x, pose.y, pose.z
        };
        msg.pose->orientation = foxglove::schemas::Quaternion{
            pose.qx, pose.qy, pose.qz, pose.qw
        };
        
        pose_channel_->log(msg);
    }
}

void FoxgloveBridge::sendPointCloud() {
    auto points = slam_processor_->getCurrentMapPoints();
    
    if (pointcloud_channel_) {
        foxglove::schemas::PointCloud msg;
        msg.timestamp = foxglove::schemas::Timestamp{0, 0};
        msg.frame_id = "map";
        msg.pose = foxglove::schemas::Pose{
            .position = foxglove::schemas::Vector3{0.0, 0.0, 0.0},
            .orientation = foxglove::schemas::Quaternion{0.0, 0.0, 0.0, 1.0}
        };
        msg.point_stride = 12;
        
        // 定义字段
        foxglove::schemas::PackedElementField x_field;
        x_field.name = "x";
        x_field.offset = 0;
        x_field.type = foxglove::schemas::PackedElementField::NumericType::FLOAT32;
        msg.fields.push_back(x_field);
        
        foxglove::schemas::PackedElementField y_field;
        y_field.name = "y";
        y_field.offset = 4;
        y_field.type = foxglove::schemas::PackedElementField::NumericType::FLOAT32;
        msg.fields.push_back(y_field);
        
        foxglove::schemas::PackedElementField z_field;
        z_field.name = "z";
        z_field.offset = 8;
        z_field.type = foxglove::schemas::PackedElementField::NumericType::FLOAT32;
        msg.fields.push_back(z_field);
        
        // 添加点数据
        msg.data.resize(points.size() * 3 * sizeof(float));  // x, y, z for each point
        for (size_t i = 0; i < points.size(); ++i) {
            float* data_ptr = reinterpret_cast<float*>(msg.data.data());
            data_ptr[i * 3 + 0] = static_cast<float>(points[i].x);
            data_ptr[i * 3 + 1] = static_cast<float>(points[i].y);
            data_ptr[i * 3 + 2] = static_cast<float>(points[i].z);
        }
        
        pointcloud_channel_->log(msg);
    }
}

void FoxgloveBridge::sendTransform() {
    if (tf_channel_) {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000000000;
        
        foxglove::schemas::FrameTransform msg;
        msg.timestamp = foxglove::schemas::Timestamp{
            static_cast<uint32_t>(seconds),
            static_cast<uint32_t>(nanoseconds)
        };
        msg.parent_frame_id = "map";
        msg.child_frame_id = "base_link";
        msg.translation = foxglove::schemas::Vector3{0.0, 0.0, 0.0};
        msg.rotation = foxglove::schemas::Quaternion{0.0, 0.0, 0.0, 1.0};
        
        tf_channel_->log(msg);
    }
}