# C++ 内存模型

## 什么是内存模型？

定义多线程对共享内存的访问规则。

## 同步顺序

```
所有线程看到相同的操作顺序
         ↓
    线程 A          线程 B
    x = 1           y = 1
    |               |
    ↓               ↓
  happens-before    happens-before
         ↓
    线程间同步点
```

## happens-before 关系

1. 同一个线程内，按代码顺序
2. 锁的 lock/unlock
3. 原子操作的同步
4. thread 的创建和结束

## 数据竞争

```cpp
// 未定义行为！
int counter = 0;
thread t1([&]{ ++counter; });
thread t2([&]{ ++counter; });

// 避免数据竞争：
// 1. 使用互斥锁
// 2. 使用原子操作
// 3. 使用线程局部存储
```

## 顺序一致视图

```cpp
std::atomic<int> x{0}, y{0};

void a() {
    x.store(1, std::memory_order_seq_cst);
    y.store(1, std::memory_order_seq_cst);
}

// 所有线程看到相同的 x,y 顺序
```
