# 生产者-消费者模式

## 模式描述

```
  生产者                    消费者
    |                         |
    ↓                         |
  ┌────────────────────────┐  |
  │    线程安全队列         │←─┘
  │    (有界缓冲区)         │
  └────────────────────────┘
         ↑                          
           (条件变量通知)
```

## 完整实现

```cpp
template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv;
    const size_t max_size;
    bool done = false;

public:
    ThreadSafeQueue(size_t max) : max_size(max) {}

    void push(T value) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]{ 
                return q.size() < max_size || done; 
            });
            if (done) return;
            q.push(std::move(value));
        }
        cv.notify_one();
    }

    bool pop(T& value) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !q.empty() || done; });
        if (q.empty()) return false;
        value = std::move(q.front());
        q.pop();
        lock.unlock();
        cv.notify_one();
        return true;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            done = true;
        }
        cv.notify_all();
    }
};
```

## 使用示例

```cpp
ThreadSafeQueue<int> queue(100);
std::atomic<bool> running{true};

// 生产者
std::thread producer([&] {
    for (int i = 0; i < 1000; ++i) {
        queue.push(i);
    }
    queue.stop();
});

// 消费者
std::thread consumer([&] {
    int value;
    while (queue.pop(value)) {
        process(value);
    }
});

producer.join();
consumer.join();
```
