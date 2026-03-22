/**
 * @file 03_thread_sync.cpp
 * @brief 线程同步原语详解
 * 
 * @description
 * 本文件演示 C++ 中的各种线程同步工具：
 * 1. std::mutex - 互斥锁
 * 2. std::lock_guard - RAII 风格锁
 * 3. std::unique_lock - 灵活锁管理
 * 4. std::condition_variable - 条件变量
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ============================================================
// 示例1: std::mutex 基础
// ============================================================

/**
 * @brief mutex 基础用法
 * 
 * mutex 提供最基本的互斥功能
 * 
 * @warning 注意：
 *       - 手动 lock/unlock 容易出错
 *       - 推荐使用 RAII 风格
 */
class CounterMutex {
private:
    int count = 0;
    mutex mtx;  // 保护 count 的互斥锁

public:
    /**
     * @brief 线程安全的计数增加
     * 
     * 使用 lock() 和 unlock() 手动管理
     */
    void increment() {
        mtx.lock();           // 加锁
        ++count;             // 临界区操作
        mtx.unlock();         // 解锁
    }
    
    /**
     * @brief 获取当前计数
     */
    int get() {
        lock_guard<mutex> lock(mtx);  // RAII 锁
        return count;
    }
};

// ============================================================
// 示例2: std::lock_guard
// ============================================================

/**
 * @brief lock_guard 用法
 * 
 * lock_guard 是 RAII 风格的锁
 * - 构造时自动加锁
 * - 析构时自动解锁
 * - 不能复制，不能移动
 */
class CounterLockGuard {
private:
    int count = 0;
    mutex mtx;

public:
    void increment() {
        // 构造时加锁，作用域结束时自动解锁
        lock_guard<mutex> lock(mtx);
        ++count;
        // ... 其他操作
        // lock 在这里析构，自动解锁
    }
    
    // 也可以在函数中间创建锁
    void safeIncrement() {
        // 前置操作（不需要锁）
        doSomething();
        
        {
            // 只在这个作用域内加锁
            lock_guard<mutex> lock(mtx);
            ++count;
        }  // 锁在这里释放
        
        // 后续操作（不需要锁）
        doOther();
    }
    
    int get() {
        lock_guard<mutex> lock(mtx);
        return count;
    }
    
private:
    void doSomething() { /* ... */ }
    void doOther() { /* ... */ }
};

// ============================================================
// 示例3: std::unique_lock
// ============================================================

/**
 * @brief unique_lock 灵活锁管理
 * 
 * unique_lock 比 lock_guard 更灵活：
 * - 可以随时加锁/解锁
 * - 支持延迟加锁
 * - 支持条件变量
 */
class ThreadSafeQueue {
private:
    queue<int> q;
    mutex mtx;
    condition_variable cv;

public:
    /**
     * @brief 入队（带通知）
     */
    void push(int value) {
        {
            unique_lock<mutex> lock(mtx);
            q.push(value);
        }
        // 锁已释放，通知等待的线程
        cv.notify_one();
    }
    
    /**
     * @brief 出队（带等待）
     * 
     * 使用条件变量等待，直到队列非空
     */
    int pop() {
        unique_lock<mutex> lock(mtx);
        
        // 等待条件为真
        // 第二个参数 lambda 返回 false 时会解锁并等待
        cv.wait(lock, [this]() { return !q.empty(); });
        
        int value = q.front();
        q.pop();
        return value;
    }
    
    /**
     * @brief 非阻塞获取大小
     */
    size_t size() {
        lock_guard<mutex> lock(mtx);
        return q.size();
    }
};

// ============================================================
// 示例4: 死锁演示与避免
// ============================================================

mutex mtx1, mtx2;

/**
 * @brief 死锁示例
 * 
 * @warning 这是错误的用法！
 * 两个线程可能互相等待对方释放锁，导致死锁
 */
void deadlockExample() {
    // 线程1: 先锁 mtx1，再锁 mtx2
    // 线程2: 先锁 mtx2，再锁 mtx1
    
    // 场景：
    // 线程1 获取 mtx1，等待 mtx2
    // 线程2 获取 mtx2，等待 mtx1
    // -> 死锁！
}

/**
 * @brief 避免死锁的方法
 * 
 * 方法1: 始终按相同顺序加锁
 * 方法2: 使用 std::lock() 同时锁定多个锁
 * 方法3: 使用 std::try_lock() 尝试锁定
 */

/**
 * @brief 方法2: std::lock() 示例
 */
class DeadlockSafe {
private:
    mutex mtx1, mtx2;

public:
    void safeOperation() {
        // std::lock 会自动处理，避免死锁
        // 它使用特定算法确保只有一个线程能获得所有锁
        lock(mtx1, mtx2);
        
        // 将 mutex 转换为 unique_lock（不重复加锁）
        unique_lock<mutex> lock1(mtx1, adopt_lock);
        unique_lock<mutex> lock2(mtx2, adopt_lock);
        
        // 执行临界区操作
        doSomething();
    }
    
private:
    void doSomething() { /* ... */ }
};

// ============================================================
// 示例5: std::atomic 原子操作
// ============================================================

/**
 * @brief 原子计数器
 * 
 * std::atomic 用于不需要锁的简单操作
 * 比 mutex 更高效
 */
class AtomicCounter {
private:
    atomic<int> count{0};  // 原子整数

public:
    /**
     * @brief 原子增加
     * 
     * operator++ 是原子操作
     */
    void increment() {
        ++count;
    }
    
    /**
     * @brief 原子获取值
     */
    int get() const {
        return count.load();
    }
    
    /**
     * @brief 原子减少
     */
    void decrement() {
        --count;
    }
};

// ============================================================
// 主函数：演示各种同步原语
// ============================================================

int main() {
    cout << "========================================" << endl;
    cout << "    C++ 线程同步原语示例" << endl;
    cout << "========================================" << endl;
    
    // ----- mutex 示例 -----
    cout << "\n----- mutex 示例 -----" << endl;
    {
        CounterMutex counter;
        
        vector<thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([&counter]() {
                for (int j = 0; j < 1000; ++j) {
                    counter.increment();
                }
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        cout << "最终计数: " << counter.get() << " (应为 10000)" << endl;
    }
    
    // ----- atomic 示例 -----
    cout << "\n----- atomic 示例 -----" << endl;
    {
        AtomicCounter counter;
        
        vector<thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([&counter]() {
                for (int j = 0; j < 1000; ++j) {
                    counter.increment();
                }
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
        
        cout << "原子计数: " << counter.get() << " (应为 10000)" << endl;
    }
    
    // ----- 线程安全队列示例 -----
    cout << "\n----- 线程安全队列示例 -----" << endl;
    {
        ThreadSafeQueue queue;
        
        // 生产者线程
        thread producer([&queue]() {
            for (int i = 0; i < 5; ++i) {
                queue.push(i);
                cout << "[生产者] 放入: " << i << endl;
                this_thread::sleep_for(100ms);
            }
        });
        
        // 消费者线程
        thread consumer([&queue]() {
            for (int i = 0; i < 5; ++i) {
                this_thread::sleep_for(150ms);
                int value = queue.pop();
                cout << "[消费者] 取出: " << value << endl;
            }
        });
        
        producer.join();
        consumer.join();
    }
    
    cout << "\n========================================" << endl;
    cout << "           程序结束" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
