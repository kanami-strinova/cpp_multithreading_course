/**
 * @file 02_hello_thread.cpp
 * @brief 第一个多线程程序：创建和等待线程
 * 
 * @author C++学习者
 * @date 2024
 * 
 * @description
 * 这是最简单的多线程示例，演示：
 * 1. 如何创建线程
 * 2. 如何等待线程结束 (join)
 * 3. 线程执行顺序的不确定性
 */

#include <iostream>
#include <thread>      // C++ 线程库
#include <chrono>      // 时间相关工具
#include <vector>      // 容器

// 使用命名空间简化代码
using namespace std;
using namespace std::chrono;

// ============================================================
// 示例1: 最简单的多线程
// ============================================================

/**
 * @brief 线程函数：打印消息
 * 
 * 这个函数将在新线程中执行
 * 
 * @note 注意：
 *       - 线程函数的参数传递是复制的
 *       - 如果需要引用，使用 std::ref()
 */
void printMessage(const string& msg) {
    cout << "[线程 " << this_thread::get_id() << "] ";
    cout << msg << endl;
    
    // 模拟一些工作
    this_thread::sleep_for(milliseconds(100));
}

/**
 * @brief 带参数的任务函数
 * 
 * @param id     任务ID
 * @param seconds 睡眠秒数
 */
void taskWithParams(int id, int seconds) {
    cout << "任务 " << id << " 开始" << endl;
    
    // 睡眠指定秒数
    this_thread::sleep_for(seconds * 1s);
    
    cout << "任务 " << id << " 完成" << endl;
}

// ============================================================
// 示例2: Lambda 表达式创建线程
// ============================================================

/**
 * @brief 使用 Lambda 创建线程
 * 
 * Lambda 可以捕获外部变量，适用于简短的任务
 */
void lambdaThreadExample() {
    int counter = 0;
    
    // 创建线程，Lambda 捕获 counter
    thread t([&counter]() {
        for (int i = 0; i < 5; ++i) {
            counter++;
            cout << "线程中: counter = " << counter << endl;
        }
    });
    
    // 等待线程结束
    t.join();
    
    cout << "主线程结束: counter = " << counter << endl;
}

// ============================================================
// 示例3: 线程组管理
// ============================================================

/**
 * @brief 管理多个线程
 * 
 * 使用 vector 存储线程对象，便于统一管理
 */
void threadGroupExample() {
    const int NUM_THREADS = 4;
    vector<thread> threads;  // 线程容器
    
    cout << "创建 " << NUM_THREADS << " 个线程..." << endl;
    
    // 创建多个线程
    for (int i = 0; i < NUM_THREADS; ++i) {
        // 使用 move 避免复制
        threads.emplace_back([i]() {
            cout << "线程 " << i << " 运行中" << endl;
            this_thread::sleep_for(milliseconds(100));
        });
    }
    
    cout << "等待所有线程结束..." << endl;
    
    // 等待所有线程结束
    for (auto& t : threads) {
        if (t.joinable()) {  // 检查是否可以 join
            t.join();
        }
    }
    
    cout << "所有线程已完成" << endl;
}

// ============================================================
// 示例4: detach 分离线程
// ============================================================

/**
 * @brief 分离线程示例
 * 
 * @warning 注意：
 *       - detached 线程在后台运行
 *       - 主线程结束时可能导致未定义行为
 *       - 通常用于守护线程或日志线程
 */
void detachedThreadExample() {
    // 创建一个"后台"线程
    thread background([]() {
        for (int i = 0; i < 3; ++i) {
            cout << "[后台] 执行任务 " << i << endl;
            this_thread::sleep_for(milliseconds(200));
        }
        cout << "[后台] 任务完成" << endl;
    });
    
    // 分离线程（不再需要 join）
    background.detach();
    
    cout << "主线程继续执行..." << endl;
    this_thread::sleep_for(milliseconds(100));
    cout << "主线程结束" << endl;
}

// ============================================================
// 主函数
// ============================================================

int main() {
    cout << "========================================" << endl;
    cout << "    C++ 多线程编程 - 线程基础示例" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    // 显示当前硬件支持
    cout << "[信息] 硬件并发级别: " << thread::hardware_concurrency() << endl;
    cout << "[信息] 主线程 ID: " << this_thread::get_id() << endl;
    cout << endl;
    
    // ----- 示例1: 基本线程创建 -----
    cout << "----- 示例1: 基本线程创建 -----" << endl;
    {
        thread t1(printMessage, "来自线程的消息!");
        
        // 在 t1 启动后，主线程继续执行
        cout << "[主线程] 创建了线程 " << t1.get_id() << endl;
        
        // 等待线程结束
        t1.join();
        cout << "线程已结束" << endl;
    }
    cout << endl;
    
    // ----- 示例2: Lambda 线程 -----
    cout << "----- 示例2: Lambda 表达式线程 -----" << endl;
    lambdaThreadExample();
    cout << endl;
    
    // ----- 示例3: 线程组 -----
    cout << "----- 示例3: 线程组管理 -----" << endl;
    threadGroupExample();
    cout << endl;
    
    // ----- 示例4: 分离线程 -----
    cout << "----- 示例4: 分离线程 -----" << endl;
    detachedThreadExample();
    cout << endl;
    
    // 确保后台线程有机会执行
    this_thread::sleep_for(milliseconds(300));
    
    cout << "========================================" << endl;
    cout << "           程序结束" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
