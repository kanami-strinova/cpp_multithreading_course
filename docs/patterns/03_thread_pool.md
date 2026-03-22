# 线程池模式

## 概述

预先创建一组工作线程，从任务队列获取任务执行。

```
    ┌──────────────────────────────┐
    │        任务队列              │
    │   (线程安全)                 │
    └──────────────────────────────┘
           ↑        ↓
           │        │
    ┌──────┘        └──────┐
    ↓                      ↓
  ┌────┐                ┌────┐
  │线程│                │线程│
  └────┘                └────┘
```

## 实现

```cpp
class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable cv;
    std::atomic<bool> stop_{false};

public:
    explicit ThreadPool(size_t threads) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        cv.wait(lock, [this] { 
                            return stop_.load() || !tasks.empty(); 
                        });
                        if (stop_.load() && tasks.empty()) return;
                        task = std::move(tasks.front());
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
            std::lock_guard<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(f));
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        stop_ = true;
        cv.notify_all();
        for (auto& w : workers) w.join();
    }
};
```

## 使用

```cpp
ThreadPool pool(4);

// 提交任务
pool.enqueue([] { 
    std::cout << "Task 1\n"; 
});

pool.enqueue([] { 
    std::cout << "Task 2\n"; 
});

// Lambda 带参数和返回值
auto future = pool.enqueue([](int x, int y) {
    return x + y;
}, 1, 2);
```
