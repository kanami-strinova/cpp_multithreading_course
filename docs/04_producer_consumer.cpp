/**
 * @file 04_producer_consumer.cpp
 * @brief 生产者-消费者模式完整实现
 * 
 * @description
 * 这是一个完整的多线程生产者-消费者示例，展示：
 * 1. 线程安全的任务队列
 * 2. 多生产者-多消费者
 * 3. 优雅的线程终止
 * 4. 性能监控
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>
#include <vector>
#include <memory>
#include <optional>

using namespace std;
using namespace std::chrono;

// ============================================================
// 类型定义
// ============================================================

/**
 * @brief 任务结构
 */
struct Task {
    int id;                 // 任务ID
    string name;           // 任务名称
    system_clock::time_point created;  // 创建时间
    
    Task(int i, const string& n) : id(i), name(n), created(system_clock::now()) {}
};

/**
 * @brief 统计信息
 */
struct Statistics {
    atomic<int> produced{0};
    atomic<int> consumed{0};
    atomic<int> processing{0};
};

// ============================================================
// 线程安全的任务队列
// ============================================================

/**
 * @brief 线程安全队列
 * 
 * 支持：
 * - 线程安全的 push/pop
 * - 条件变量通知
 * - 优雅终止
 */
class ThreadSafeQueue {
private:
    queue<Task> queue_;
    mutable mutex mtx_;
    condition_variable cv_;
    atomic<bool> stop_{false};

public:
    /**
     * @brief 入队
     */
    void push(Task task) {
        {
            lock_guard<mutex> lock(mtx_);
            queue_.push(std::move(task));
        }
        cv_.notify_one();
    }
    
    /**
     * @brief 出队（带超时）
     * 
     * @param timeout 超时时间
     * @return std::nullopt 如果超时或停止
     */
    optional<Task> pop(milliseconds timeout) {
        unique_lock<mutex> lock(mtx_);
        
        // 等待条件或超时
        bool success = cv_.wait_for(
            lock,
            timeout,
            [this]() { return !queue_.empty() || stop_.load(); }
        );
        
        if (!success || stop_.load()) {
            return nullopt;
        }
        
        Task task = std::move(queue_.front());
        queue_.pop();
        return task;
    }
    
    /**
     * @brief 停止队列
     */
    void stop() {
        {
            lock_guard<mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
    }
    
    /**
     * @brief 检查是否为空
     */
    bool empty() const {
        lock_guard<mutex> lock(mtx_);
        return queue_.empty();
    }
    
    /**
     * @brief 获取大小
     */
    size_t size() const {
        lock_guard<mutex> lock(mtx_);
        return queue_.size();
    }
};

// ============================================================
// 生产者
// ============================================================

/**
 * @brief 生产者线程
 */
class Producer {
private:
    int id_;
    ThreadSafeQueue& queue_;
    Statistics& stats_;
    atomic<bool>& running_;

public:
    Producer(int id, ThreadSafeQueue& q, Statistics& s, atomic<bool>& r)
        : id_(id), queue_(q), stats_(s), running_(r) {}
    
    /**
     * @brief 生产任务
     */
    void run() {
        int task_id = 0;
        while (running_.load()) {
            // 创建任务
            Task task(++task_id, "Task-" + to_string(id_) + "-" + to_string(task_id));
            
            // 入队
            queue_.push(std::move(task));
            stats_.produced++;
            
            // 随机延迟（模拟工作）
            this_thread::sleep_for(milliseconds(50 + rand() % 100));
        }
        
        cout << "[生产者 " << id_ << "] 停止，共生产 " << task_id << " 个任务" << endl;
    }
};

// ============================================================
// 消费者
// ============================================================

/**
 * @brief 消费者线程
 */
class Consumer {
private:
    int id_;
    ThreadSafeQueue& queue_;
    Statistics& stats_;
    atomic<bool>& running_;

public:
    Consumer(int id, ThreadSafeQueue& q, Statistics& s, atomic<bool>& r)
        : id_(id), queue_(q), stats_(s), running_(r) {}
    
    /**
     * @brief 消费任务
     */
    void run() {
        int processed = 0;
        while (running_.load() || !queue_.empty()) {
            // 尝试获取任务（带超时）
            auto task = queue_.pop(milliseconds(100));
            
            if (task) {
                stats_.processing++;
                
                // 处理任务
                cout << "[消费者 " << id_ << "] 处理: " << task->name << endl;
                
                // 模拟处理时间
                this_thread::sleep_for(milliseconds(30 + rand() % 70));
                
                processed++;
                stats_.consumed++;
                stats_.processing--;
            }
        }
        
        cout << "[消费者 " << id_ << "] 停止，共处理 " << processed << " 个任务" << endl;
    }
};

// ============================================================
// 主函数
// ============================================================

int main() {
    cout << "========================================" << endl;
    cout << "    生产者-消费者示例" << endl;
    cout << "========================================" << endl;
    
    // 配置
    const int NUM_PRODUCERS = 2;
    const int NUM_CONSUMERS = 3;
    const int RUNNING_SECONDS = 5;
    
    // 初始化
    ThreadSafeQueue queue;
    Statistics stats;
    atomic<bool> running{true};
    
    vector<unique_ptr<Producer>> producers;
    vector<unique_ptr<Consumer>> consumers;
    
    // 创建生产者
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        producers.push_back(make_unique<Producer>(i, queue, stats, running));
    }
    
    // 创建消费者
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumers.push_back(make_unique<Consumer>(i, queue, stats, running));
    }
    
    // 启动生产者线程
    vector<thread> producer_threads;
    for (auto& p : producers) {
        producer_threads.emplace_back([&p]() { p->run(); });
    }
    
    // 启动消费者线程
    vector<thread> consumer_threads;
    for (auto& c : consumers) {
        consumer_threads.emplace_back([&c]() { c->run(); });
    }
    
    // 监控线程
    thread monitor([&]() {
        while (running.load()) {
            this_thread::sleep_for(1s);
            cout << "[监控] 生产: " << stats.produced 
                 << " | 消费: " << stats.consumed 
                 << " | 处理中: " << stats.processing
                 << " | 队列: " << queue.size() << endl;
        }
    });
    
    // 运行一段时间
    cout << "系统运行 " << RUNNING_SECONDS << " 秒..." << endl;
    this_thread::sleep_for(seconds(RUNNING_SECONDS));
    
    // 停止
    cout << "停止系统..." << endl;
    running = false;
    queue.stop();
    
    // 等待所有线程结束
    monitor.join();
    for (auto& t : producer_threads) t.join();
    for (auto& t : consumer_threads) t.join();
    
    // 统计结果
    cout << "\n========================================" << endl;
    cout << "           统计结果" << endl;
    cout << "========================================" << endl;
    cout << "总生产: " << stats.produced << endl;
    cout << "总消费: " << stats.consumed << endl;
    cout << "丢失任务: " << (stats.produced - stats.consumed) << endl;
    
    return 0;
}
