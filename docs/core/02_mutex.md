# std::mutex 详解

## 互斥锁

### 基本互斥锁

```cpp
std::mutex mtx;

void safeFunction() {
    mtx.lock();
    try {
        // 临界区
        doSomething();
        mtx.unlock();
    } catch (...) {
        mtx.unlock();
        throw;
    }
}
```

### lock_guard（推荐）

```cpp
std::mutex mtx;

void safeFunction() {
    std::lock_guard<std::mutex> lock(mtx);
    // 自动管理锁
    doSomething();
}  // lock 在析构时自动解锁
```

### unique_lock（灵活）

```cpp
std::mutex mtx;
std::unique_lock<std::mutex> lock(mtx);

// 可以手动解锁
lock.unlock();
// ... 不需要锁的操作
lock.lock();

// 可以延迟加锁
std::unique_lock<std::mutex> lock2(mtx, std::defer_lock);
// ... 其他操作
lock2.lock();
```

## 递归锁

```cpp
std::recursive_mutex rmtx;

void outer() {
    std::lock_guard<std::mutex> lock(rmtx);
    inner();  // 可以再次获取锁
}

void inner() {
    std::lock_guard<std::mutex> lock(rmtx);
}
```
