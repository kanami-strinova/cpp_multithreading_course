# 📚 C++ 多线程编程完整教程

## 目录

### 第一部分：基础概念
1. [进程与线程](./basics/01_process_vs_thread.md)
2. [并发与并行](./basics/02_concurrency_parallelism.md)
3. [线程生命周期](./basics/03_thread_lifecycle.md)

### 第二部分：核心工具
4. [std::thread 详解](./core/01_std_thread.md)
5. [互斥锁 mutex](./core/02_mutex.md)
6. [锁管理 RAII](./core/03_lock_guard.md)

### 第三部分：高级主题
7. [条件变量](./advanced/01_condition_variable.md)
8. [原子操作 atomic](./advanced/02_atomic.md)
9. [内存模型](./advanced/03_memory_model.md)

### 第四部分：设计模式
10. [生产者-消费者](./patterns/01_producer_consumer.md)
11. [读写锁](./patterns/02_read_write_lock.md)
12. [线程池](./patterns/03_thread_pool.md)

### 第五部分：实战项目
- [日志分析器](../src/projects/01_log_analyzer.md)
- [并发服务器](../src/projects/02_server.md)
- [并行计算框架](../src/projects/03_parallel_compute.md)

---

## 学习路线

```
Week 1-2: 基础入门
    ↓
Week 3-4: 线程管理  
    ↓
Week 5-6: 同步原语
    ↓
Week 7-8: 高级主题
    ↓
Week 9-12: 实战项目
```

## 推荐资源

- [cppreference.com](https://en.cppreference.com/)
- 《C++ Concurrency in Action》
- 《Effective Modern C++》
