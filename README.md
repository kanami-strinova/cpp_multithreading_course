# 🎓 C++ 多线程编程完整学习项目

![C++](https://img.shields.io/badge/C++-17-blue)
![License](https://img.shields.io/badge/License-MIT-green)

面向计算机科学专业学生的 **C++ 多线程编程系统化学习资源**。

---

## 📚 内容概览

| 章节 | 内容 | 代码数 | 文档数 |
|------|------|--------|--------|
| 基础概念 | 进程、线程、并发 | 2 | 3 |
| 线程管理 | 创建、销毁、参数传递 | 3 | 4 |
| 同步原语 | mutex、lock_guard、原子 | 4 | 5 |
| 设计模式 | 生产者-消费者、线程池 | 3 | 3 |
| 实战项目 | 日志分析器、服务器等 | 4 | 4 |

---

## 🚀 快速开始

### 环境要求

- **C++ 编译器**: GCC 9+ / Clang 10+ / MSVC 2019+
- **C++ 标准**: C++17 或更高
- **构建工具**: CMake 3.15+

### 编译运行

```bash
# 克隆项目
git clone https://github.com/kanami-strinova/cpp_multithreading_course.git
cd cpp_multithreading_course

# 创建构建目录
mkdir build && cd build

# 配置 CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译所有示例
make -j$(nproc)

# 运行示例
./hello_threads
./thread_sync
./producer_consumer
```

---

## 📖 学习路线图

### 第一阶段：基础入门（Week 1-2）

```
📖 基础概念
├── 进程与线程的区别
├── 并发与并行的概念
└── 线程生命周期

💻 入门代码
├── hello_threads.cpp - 第一个线程程序
├── thread_args.cpp - 线程参数传递
└── thread_management.cpp - 线程管理
```

### 第二阶段：核心工具（Week 3-4）

```
📖 同步原语
├── mutex 互斥锁
├── lock_guard RAII 锁
├── unique_lock 灵活锁
└── condition_variable 条件变量

💻 实战代码
├── mutex_basic.cpp - mutex 基础
├── lock_guard_demo.cpp - RAII 锁
├── producer_consumer.cpp - 生产者消费者
└── thread_sync.cpp - 综合示例
```

### 第三阶段：高级主题（Week 5-6）

```
📖 高级概念
├── std::atomic 原子操作
├── 内存模型
├── 锁的进阶技巧
└── 死锁避免

💻 高级代码
├── atomic_demo.cpp - 原子操作
├── deadlock_avoid.cpp - 避免死锁
├── memory_order.cpp - 内存序
└── read_write_lock.cpp - 读写锁
```

### 第四阶段：设计模式（Week 7-8）

```
📖 并发模式
├── 生产者-消费者模式
├── 线程池模式
├── Future-Promise 模式
└── Active Object 模式

💻 模式实现
├── thread_pool.cpp - 线程池
├── producer_consumer.cpp - PC 模式
├── future_promise.cpp - 异步模式
└── active_object.cpp - 主动对象
```

### 第五阶段：实战项目（Week 9-12）

```
🚀 项目实战

1. 日志分析器
   ├── 多线程文件处理
   ├── 实时统计
   └── 性能监控

2. 并发服务器
   ├── 多客户端连接
   ├── 事件驱动
   └── 心跳检测

3. 任务调度系统
   ├── 生产者-消费者
   ├── 优先级队列
   └── 动态负载均衡

4. 并行计算框架
   ├── Fork-Join
   ├── Work Stealing
   └── SIMD 向量化
```

---

## 📁 项目结构

```
cpp_multithreading_course/
│
├── CMakeLists.txt              # 构建配置
├── README.md                   # 项目文档
│
├── docs/                       # 文档教程
│   ├── README.md               # 文档索引
│   ├── basics/                 # 基础概念
│   │   ├── 01_process_vs_thread.md
│   │   └── 02_concurrency_parallelism.md
│   ├── core/                  # 核心工具
│   │   ├── 01_std_thread.md
│   │   ├── 02_mutex.md
│   │   └── 03_lock_guard.md
│   ├── advanced/              # 高级主题
│   │   ├── 01_condition_variable.md
│   │   ├── 02_atomic.md
│   │   └── 03_memory_model.md
│   ├── patterns/             # 设计模式
│   │   ├── 01_producer_consumer.md
│   │   ├── 02_read_write_lock.md
│   │   └── 03_thread_pool.md
│   └── projects/              # 实战项目
│       ├── 01_log_analyzer.md
│       ├── 02_server.md
│       ├── 03_pc_system.md
│       └── 04_parallel_compute.md
│
├── src/                       # 源代码
│   ├── 01_basics/             # 基础示例
│   │   └── hello_threads.cpp
│   ├── 02_concurrency/       # 并发工具
│   │   └── thread_pool.cpp
│   ├── 03_sync/              # 同步原语
│   │   └── thread_sync.cpp
│   ├── 04_patterns/          # 设计模式
│   │   └── producer_consumer.cpp
│   └── 05_projects/          # 实战项目
│       └── log_analyzer_project.cpp
│
├── exercises/                 # 练习题
│   ├── 01_basics_exercises.md
│   ├── 02_sync_exercises.md
│   └── 03_patterns_exercises.md
│
├── solutions/                 # 参考答案
│   ├── 01_basics_solutions.cpp
│   └── 02_sync_solutions.cpp
│
└── resources/                 # 资源文件
    └── .gitkeep
```

---

## 🎯 核心知识点

### 线程管理

```cpp
// 创建线程
std::thread t(func, args...);

// 等待线程
t.join();

// 分离线程
t.detach();

// 检查是否可 join
if (t.joinable()) t.join();
```

### 同步原语

| 原语 | 用途 | 特点 |
|------|------|------|
| `std::mutex` | 互斥 | 手动管理 |
| `std::lock_guard` | RAII锁 | 自动释放 |
| `std::unique_lock` | 灵活锁 | 延迟/递归 |
| `std::condition_variable` | 条件同步 | 等待通知 |
| `std::atomic` | 原子操作 | 无锁编程 |

### 并发模式

- **生产者-消费者**: 任务队列 + 条件变量
- **线程池**: 预创建线程 + 任务队列
- **读写锁**: 读共享、写独占
- **Future-Promise**: 异步结果传递

---

## 🧪 测试

```bash
# 构建测试
make -j$(nproc)

# 运行单元测试
ctest -V

# 运行特定测试
./src/thread_sync
./src/producer_consumer
```

---

## 📝 练习指南

每章配有练习题，位于 `exercises/` 目录。

### 练习结构

```
exercises/
├── 01_basics_exercises.md      # 练习题
├── 02_sync_exercises.md       # 同步练习
└── 03_patterns_exercises.md   # 模式练习
```

### 参考答案

参考答案位于 `solutions/` 目录，**请先自己尝试！**

---

## 🤝 贡献

欢迎贡献代码和文档！

1. Fork 本项目
2. 创建特性分支
3. 提交 Pull Request

---

## 📚 推荐资源

### 书籍
- 《C++ Concurrency in Action》 - Anthony Williams
- 《Effective Modern C++》 - Scott Meyers
- 《The C++ Standard Library》 - Nicolai Josuttis

### 在线资源
- [cppreference.com](https://en.cppreference.com/w/cpp/thread)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [LearnCpp.com](https://www.learncpp.com/)

### 工具
- **编译器**: GCC / Clang / MSVC
- **调试器**: GDB / LLDB / WinDbg
- **性能分析**: perf / Valgrind / Intel VTune

---

## 📄 许可证

MIT License - 详见 [LICENSE](LICENSE) 文件

---

## 🙏 致谢

- C++ 标准委员会
- cppreference.com 社区
- 所有 Contributors

---

**开始学习吧！祝学业进步！** 🚀
