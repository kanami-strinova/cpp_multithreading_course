# 🌐 项目二：并发服务器

## 项目目标

构建一个支持多客户端连接的并发服务器：
- 处理多个并发连接
- 实现简单的协议
- 支持心跳检测

## 架构设计

```
                    ┌─────────────────┐
                    │   Main Thread   │
                    │  (监听端口)      │
                    └────────┬────────┘
                             │
              ┌──────────────┼──────────────┐
              ↓              ↓              ↓
         ┌────────┐    ┌────────┐    ┌────────┐
         │Worker 1│    │Worker 2│    │Worker N│
         │Thread  │    │Thread  │    │Thread  │
         └────────┘    └────────┘    └────────┘
              │              │              │
              └──────────────┼──────────────┘
                             ↓
                    ┌─────────────────┐
                    │  Session Map    │
                    │ (连接管理)      │
                    └─────────────────┘
```

## 协议设计

### 消息格式
```
+------------+----------+----------+
| Length(4) | Type(1) | Data(N) |
+------------+----------+----------+
```

### 消息类型
- `0x01`: 登录
- `0x02`: 心跳
- `0x03`: 消息
- `0x04`: 退出

## 核心代码

```cpp
class Server {
private:
    ThreadPool pool_;
    std::unordered_map<int, Session> sessions_;
    std::mutex session_mutex_;

public:
    void start(int port) {
        acceptor_.async_accept(
            [this](tcp::socket socket) {
                handle_accept(std::move(socket));
            }
        );
    }

private:
    void handle_accept(tcp::socket socket) {
        auto session = std::make_shared<Session>(
            std::move(socket),
            [this](int id) { remove_session(id); }
        );
        add_session(session);
        session->start();
    }
};
```

## 扩展挑战

1. 添加 SSL/TLS 加密
2. 实现消息广播
3. 添加连接限流
