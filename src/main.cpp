#include <iostream>
#include <memory>
#include <signal.h>
#include <thread>

#include "slam_processor.h"
#include "foxglove_bridge.h"

// 用于优雅地关闭程序
std::atomic<bool> running(true);

// 信号处理函数
void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    running = false;
}

int main() {
    std::cout << "Starting Foxglove SLAM Visualization Demo" << std::endl;
    
    // 注册信号处理函数，用于优雅地关闭程序
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    try {
        // 创建SLAM处理器
        auto slam_processor = std::make_shared<SLAMProcessor>();
        
        // 创建Foxglove桥接
        FoxgloveBridge foxglove_bridge(slam_processor);
        
        // 启动SLAM处理器
        slam_processor->startProcessing();
        
        // 启动Foxglove服务器
        foxglove_bridge.start(8765);
        
        std::cout << "Press Ctrl+C to stop the program" << std::endl;
        
        // 主循环
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            // 可以在这里添加其他逻辑
            // 例如：处理用户输入、保存数据等
        }
        
        std::cout << "Stopping services..." << std::endl;
        
        // 停止Foxglove桥接
        foxglove_bridge.stop();
        
        // 停止SLAM处理器
        slam_processor->stopProcessing();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "Program terminated successfully" << std::endl;
    return 0;
}