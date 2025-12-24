#include "slam_processor.h"
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/random.h>
#include <iostream>
#include <random>
#include <cmath>

SLAMProcessor::SLAMProcessor()
    : running_(false), 
      current_pose_(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0) {  // 初始位姿为原点，无旋转
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
    // 更新位姿（模拟简单的运动）
    auto now = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    
    // 模拟一个圆形运动轨迹
    double time_s = timestamp / 1000000000.0;
    double radius = 2.0;
    double x = radius * cos(time_s * 0.5);  // 每4π秒一圈
    double y = radius * sin(time_s * 0.5);
    double z = 1.0 + 0.5 * sin(time_s * 1.5);  // Z轴方向有小幅振荡
    
    // 更新位姿
    {
        std::lock_guard<std::mutex> lock(data_mutex_);
        current_pose_ = Pose(x, y, z, 0.0, 0.0, 0.0, 1.0, timestamp);
        generatePCLSimulatedData();
    }
}

Pose SLAMProcessor::getCurrentPose() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    return current_pose_;
}

std::vector<Point3D> SLAMProcessor::getCurrentMapPoints() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    return map_points_;
}

void SLAMProcessor::generatePCLSimulatedData() {
    // 使用PCL生成密集的运动点云
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    
    // 生成一个"房间"的点云，包括地面、墙壁和一些物体
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    std::uniform_real_distribution<> dis_large(-5.0, 5.0);
    
    // 地面点 (z = 0)
    for (int i = 0; i < 500; ++i) {
        pcl::PointXYZ point;
        point.x = dis_large(gen);
        point.y = dis_large(gen);
        point.z = 0.0;
        cloud->points.push_back(point);
    }
    
    // 天花板点 (z = 3)
    for (int i = 0; i < 500; ++i) {
        pcl::PointXYZ point;
        point.x = dis_large(gen);
        point.y = dis_large(gen);
        point.z = 3.0;
        cloud->points.push_back(point);
    }
    
    // 墙壁点
    for (int i = 0; i < 300; ++i) {
        pcl::PointXYZ point;
        // X方向墙
        point.x = 5.0;
        point.y = dis_large(gen);
        point.z = dis_large(gen) * 1.5 + 1.5;
        cloud->points.push_back(point);
        
        // 另一面X方向墙
        point.x = -5.0;
        cloud->points.push_back(point);
        
        // Y方向墙
        point.x = dis_large(gen);
        point.y = 5.0;
        point.z = dis_large(gen) * 1.5 + 1.5;
        cloud->points.push_back(point);
        
        // 另一面Y方向墙
        point.y = -5.0;
        cloud->points.push_back(point);
    }
    
    // 添加一些"家具"，如桌子和椅子
    for (int i = 0; i < 200; ++i) {
        pcl::PointXYZ point;
        
        // 桌面 (在(2,2,0)附近)
        if (i % 3 == 0) {
            point.x = 2.0 + dis(gen);
            point.y = 2.0 + dis(gen);
            point.z = 1.0;
        } 
        // 椅子
        else if (i % 3 == 1) {
            point.x = 2.5 + dis(gen) * 0.3;
            point.y = 2.5 + dis(gen) * 0.3;
            point.z = 0.5 + dis(gen) * 0.5;
        }
        // 装饰品
        else {
            point.x = 1.0 + dis(gen) * 0.5;
            point.y = 1.0 + dis(gen) * 0.5;
            point.z = 0.5 + dis(gen) * 1.5;
        }
        
        cloud->points.push_back(point);
    }
    
    // 添加一个旋转的球体，模拟动态物体
    double time_s = current_pose_.timestamp / 1000000000.0;
    double sphere_x = 1.0 + 2.0 * cos(time_s * 2.0);  // 球体在x方向移动
    double sphere_y = 1.0 + 2.0 * sin(time_s * 2.0);  // 球体在y方向移动
    double sphere_z = 1.0;
    
    for (int i = 0; i < 300; ++i) {
        double theta = dis_large(gen) * M_PI;
        double phi = dis_large(gen) * M_PI;
        
        pcl::PointXYZ point;
        point.x = sphere_x + 0.5 * sin(theta) * cos(phi);
        point.y = sphere_y + 0.5 * sin(theta) * sin(phi);
        point.z = sphere_z + 0.5 * cos(theta);
        
        cloud->points.push_back(point);
    }
    
    // 转换为Point3D向量
    map_points_.clear();
    for (const auto& pcl_point : cloud->points) {
        map_points_.emplace_back(pcl_point.x, pcl_point.y, pcl_point.z);
    }
}