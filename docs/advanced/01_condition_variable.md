# 条件变量

## 什么是条件变量？

用于线程间的**事件通知**和**条件等待**。

```
线程 A                    线程 B
   |                         |
   |    cv.wait(ready)       |
   |← ← ← ← ← ← ← ← ← ← ← ← |
   |                         |
   |                    ready = true
   |                    cv.notify()
   |                         |
   |    条件满足，继续       |
   |→ → → → → → → → → → → →|
```

## 基本用法

```cpp
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });  // 等待 ready == true
    // 继续执行
}

void notifier() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();  // 通知一个等待线程
}
```

## 生产者-消费者

```cpp
std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;

void producer() {
    for (int i = 0; i < 10; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(i);
        }
        cv.notify_one();
    }
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return !q.empty() || done; });
        if (q.empty()) break;
        int value = q.front();
        q.pop();
        lock.unlock();
        process(value);
    }
}
```

## 注意事项

1. **必须用 unique_lock**
2. **条件要放在锁内检查**
3. **虚假唤醒**：使用带条件的 wait
