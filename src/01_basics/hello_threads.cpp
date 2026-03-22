/**
 * @file hello_threads.cpp
 * @brief 第一个多线程程序
 */

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// 线程函数
void worker(int id) {
    cout << "Worker " << id << " started" << endl;
    this_thread::sleep_for(milliseconds(100));
    cout << "Worker " << id << " finished" << endl;
}

int main() {
    cout << "=== Hello Threads ===" << endl;
    
    const int NUM_THREADS = 4;
    vector<thread> threads;
    
    // 创建线程
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(worker, i);
    }
    
    // 等待所有线程
    for (auto& t : threads) {
        t.join();
    }
    
    cout << "All workers finished" << endl;
    return 0;
}
