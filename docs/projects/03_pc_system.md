# 📬 项目三：生产者-消费者系统

## 项目目标

实现一个通用的任务调度系统：
- 支持多生产者、多消费者
- 任务优先级队列
- 动态负载均衡

## 系统架构

```
┌─────────────┐     ┌─────────────────┐     ┌─────────────┐
│  Producer A │────▶│                 │     │  Consumer 1 │
└─────────────┘     │   Task Queue    │────▶│  (Worker)   │
┌─────────────┐     │   (优先级)      │     └─────────────┘
│  Producer B │────▶│                 │     
└─────────────┘     │  - 线程安全     │     ┌─────────────┐
                    │  - 阻塞队列     │────▶│  Consumer 2 │
                    │  - 动态扩容     │     │  (Worker)   │
                    └─────────────────┘     └─────────────┘
```

## 任务结构

```cpp
struct Task {
    int id;                    // 任务ID
    int priority;              // 优先级 (0-100)
    std::function<void()> func;  // 任务函数
    system_clock::time_point created;  // 创建时间
    system_clock::time_point deadline; // 截止时间
};
```

## 核心实现

```cpp
class TaskScheduler {
private:
    struct TaskCompare {
        bool operator()(const Task* a, const Task* b) {
            // 优先级高的先执行
            if (a->priority != b->priority)
                return a->priority > b->priority;
            // 早创建的先执行
            return a->created < b->created;
        }
    };

    std::priority_queue<Task*, 
                       std::vector<Task*>, 
                       TaskCompare> tasks_;
    // ... 线程安全实现
};
```

## 监控功能

```cpp
struct Monitor {
    atomic<int> submitted{0};
    atomic<int> completed{0};
    atomic<int> failed{0};
    atomic<int> pending{0};
};
```

## 扩展挑战

1. 实现任务依赖
2. 支持任务取消
3. 添加任务重试机制
