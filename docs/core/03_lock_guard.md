# 锁管理：RAII 模式

## 为什么用 RAII？

```cpp
// 危险：手动管理
void dangerous() {
    mtx.lock();
    if (error) {
        mtx.unlock();  // 容易忘记
        return;
    }
    mtx.unlock();
}

// 安全：RAII 自动管理
void safe() {
    std::lock_guard<std::mutex> lock(mtx);
    if (error) {
        return;  // 锁自动释放
    }
}
```

## C++ 提供的锁包装器

| 类 | 特点 |
|----|------|
| `lock_guard` | 简单，构造即加锁 |
| `unique_lock` | 灵活，可延迟、可转让 |
| `shared_lock` | 共享锁 |
| `scoped_lock` | 多锁同时加锁 |

## scoped_lock（C++17）

```cpp
std::mutex mtx1, mtx2;

// 同时锁定多个锁，避免死锁
std::scoped_lock lock(mtx1, mtx2);
// 两个锁都会在作用域结束时释放
```

## 条件锁

```cpp
std::unique_lock<std::mutex> lock(mtx);
lock.lock();      // 加锁
lock.unlock();    // 解锁
lock.try_lock();  // 尝试加锁
```
