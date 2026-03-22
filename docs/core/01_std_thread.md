# std::thread 详解

## 基本用法

```cpp
#include <thread>

// 1. 函数指针
void func() { }
std::thread t1(func);

// 2. Lambda
std::thread t2([]{ });

// 3. 函数对象
struct Task {
    void operator()() { }
};
std::thread t3(Task());

// 4. 成员函数
class MyClass {
    void memberFunc() { }
};
MyClass obj;
std::thread t4(&MyClass::memberFunc, &obj);
```

## 成员函数

| 函数 | 说明 |
|------|------|
| `join()` | 等待线程结束 |
| `detach()` | 分离线程 |
| `joinable()` | 检查是否可 join |
| `get_id()` | 获取线程 ID |
| `hardware_concurrency()` | 硬件并发级别 |

## 注意事项

1. **资源所有权的转移**
```cpp
std::thread t1(work);
std::thread t2 = std::move(t1);  // t2 接管，t1 为空
```

2. **避免线程泄露**
```cpp
// 错误
void bad() {
    std::thread t(work);
    // 可能抛出异常
}

// 正确：使用 RAII
class ThreadGuard {
    std::thread& t;
public:
    ~ThreadGuard() { if(t.joinable()) t.join(); }
};
```
