# ⚡ 项目四：并行计算框架

## 项目目标

构建一个通用的数据并行框架：
- 透明的多线程化
- 负载均衡
- 结果聚合

## 设计理念

```cpp
// 用户代码
Parallel<int> result = parallel_map(
    data.begin(), data.end(),
    [](int x) { return x * x; }  // 并行处理
);

// 内部自动：
// 1. 分块
// 2. 分配到线程
// 3. 并行计算
// 4. 聚合结果
```

## 核心组件

### 1. Work Stealing Queue

```cpp
class WorkStealingQueue {
private:
    std::deque<std::function<void()>> queue_;
    std::mutex mtx_;

public:
    void push(std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push_front(task);
    }

    bool steal(std::function<void()>& task) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (!queue_.empty()) {
            task = queue_.back();
            queue_.pop_back();
            return true;
        }
        return false;
    }
};
```

### 2. Fork-Join 框架

```cpp
class ParallelForkJoin {
private:
    std::vector<std::thread> workers_;
    std::atomic<int> active_tasks_{0};

public:
    template<typename F>
    auto execute(F&& func) -> decltype(func()) {
        active_tasks_++;
        func();
        active_tasks_--;
        // 等待所有任务完成
        while (active_tasks_ > 0) {
            std::this_thread::yield();
        }
    }

    // 分叉
    template<typename F>
    void fork(F&& func) {
        workers_.emplace_back([func = std::forward<F>(func), this] {
            func();
            active_tasks_--;
        });
        active_tasks_++;
    }

    // 汇合
    void join() {
        for (auto& w : workers_) {
            if (w.joinable()) w.join();
        }
    }
};
```

## 性能优化

1. **False Sharing 避免**
```cpp
// 错误：相邻数据在同一个缓存行
struct Bad { atomic<int> a; atomic<int> b; };

// 正确：填充缓存行
struct Good { atomic<int> a; char pad[64]; atomic<int> b; };
```

2. **NUMA 感知**
```cpp
// 本地线程使用本地内存
void* local_alloc(size_t size) {
    int numa_node = sched_getcpu();
    return numa_alloc_onnode(size, numa_node);
}
```

## 扩展挑战

1. SIMD 向量化
2. GPU 卸载
3. 分布式扩展
