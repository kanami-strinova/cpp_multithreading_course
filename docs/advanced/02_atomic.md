# std::atomic 原子操作

## 为什么用原子操作？

比锁更高效，适用于简单操作。

```cpp
// 使用锁
std::mutex mtx;
int counter = 0;
void increment() {
    mtx.lock();
    ++counter;
    mtx.unlock();
}

// 使用原子
std::atomic<int> counter{0};
void increment() {
    ++counter;  // 原子操作
}
```

## 常用操作

| 操作 | 说明 |
|------|------|
| `load()` | 读取值 |
| `store(val)` | 写入值 |
| `fetch_add(val)` | 原子加法 |
| `fetch_sub(val)` | 原子减法 |
| `exchange(val)` | 原子交换 |
| `compare_exchange_*` | CAS 操作 |

## 内存序

```cpp
std::atomic<int> counter{0};

// 默认：顺序一致
counter.fetch_add(1);

// 更宽松的顺序，可能更快
counter.fetch_add(1, std::memory_order_relaxed);
```

| 内存序 | 保证级别 |
|--------|----------|
| `memory_order_seq_cst` | 顺序一致（默认） |
| `memory_order_acquire` | 获取 |
| `memory_order_release` | 释放 |
| `memory_order_relaxed` | 最宽松 |

## 布尔原子

```cpp
std::atomic<bool> ready{false};

void writer() {
    ready.store(true, std::memory_order_release);
}

void reader() {
    while (!ready.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
    // 此时 writer 的操作已对 reader 可见
}
```
