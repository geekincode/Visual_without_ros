#include "slam_processor.h"
#include <iostream>
#include <cmath>
#include <random>

SLAMProcessor::SLAMProcessor() 
    : running_(false), 
      current_pose_(0, 0, 0, 0, 0, 0, 1, 0),
      map_points_() {
    // 初始化一些随机地图点
    generateSimulatedData();
}

SLAMProcessor::~SLAMProcessor() {
    if (running_) {
        stopProcessing();
    }
}

void SLAMProcessor::startProcessing() {
    if (!running_) {
        running_ = true;
        processing_thread_ = std::thread(&SLAMProcessor::processingLoop, this);
        std::cout << "SLAM processor started" << std::endl;
    }
}

void SLAMProcessor::stopProcessing() {
    if (running_) {
        running_ = false;
        if (processing_thread_.joinable()) {
            processing_thread_.join();
        }
        std::cout << "SLAM processor stopped" << std::endl;
    }
}

void SLAMProcessor::processingLoop() {
    while (running_) {
        processFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟30FPS
    }
}

void SLAMProcessor::processFrame() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    // 模拟SLAM位姿更新 - 简单的圆形轨迹
    static auto start_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start_time).count();
    
    double time_sec = elapsed / 1000.0;
    double x = 5 * cos(time_sec * 0.5);  // 圆形轨迹
    double y = 5 * sin(time_sec * 0.5);
    double z = 1 + sin(time_sec * 0.3);  // 上下波动
    
    // 更新当前位姿
    current_pose_ = Pose(x, y, z, 0, 0, sin(time_sec*0.2), cos(time_sec*0.2), 
                         static_cast<uint64_t>(elapsed * 1000000)); // 转换为纳秒
}

Pose SLAMProcessor::getCurrentPose() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    return current_pose_;
}

std::vector<Point3D> SLAMProcessor::getCurrentMapPoints() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    return map_points_;
}

void SLAMProcessor::generateSimulatedData() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    // 生成模拟地图点 - 在一个立方体区域内随机分布点
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);
    
    map_points_.clear();
    for (int i = 0; i < 500; ++i) {
        map_points_.emplace_back(dis(gen), dis(gen), dis(gen));
    }
}