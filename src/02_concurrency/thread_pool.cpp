/**
 * @file thread_pool.cpp
 * @brief 线程池实现
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <atomic>

using namespace std;

class ThreadPool {
private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex queue_mutex;
    condition_variable cv;
    atomic<bool> stop_{false};

public:
    ThreadPool(size_t threads) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(queue_mutex);
                        cv.wait(lock, [this] { return stop_.load() || !tasks.empty(); });
                        if (stop_.load() && tasks.empty()) return;
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template<typename F>
    void enqueue(F&& f) {
        {
            lock_guard<mutex> lock(queue_mutex);
            tasks.emplace(forward<F>(f));
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        stop_ = true;
        cv.notify_all();
        for (auto& w : workers) w.join();
    }
};

int main() {
    cout << "=== Thread Pool Demo ===" << endl;
    
    ThreadPool pool(4);
    
    for (int i = 0; i < 8; ++i) {
        pool.enqueue([i] {
            cout << "Task " << i << " running" << endl;
            this_thread::sleep_for(100ms);
        });
    }
    
    this_thread::sleep_for(2s);
    cout << "All tasks completed" << endl;
    
    return 0;
}
