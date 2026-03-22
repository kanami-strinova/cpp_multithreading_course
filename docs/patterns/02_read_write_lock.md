# 读写锁模式

## 概念

- **读操作**：可以多个线程同时读
- **写操作**：需要独占访问

## 实现

```cpp
class ReadWriteLock {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int readers = 0;
    int writers_waiting = 0;
    int writers_active = 0;

public:
    class ReadLock {
        ReadWriteLock& lock_;
    public:
        ReadLock(ReadWriteLock& l) : lock_(l) { lock_.lockRead(); }
        ~ReadLock() { lock_.unlockRead(); }
    };

    class WriteLock {
        ReadWriteLock& lock_;
    public:
        WriteLock(ReadWriteLock& l) : lock_(l) { lock_.lockWrite(); }
        ~WriteLock() { lock_.unlockWrite(); }
    };

    void lockRead() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ 
            return writers_active == 0 && writers_waiting == 0; 
        });
        ++readers;
    }

    void unlockRead() {
        std::unique_lock<std::mutex> lock(mtx);
        --readers;
        cv.notify_all();
    }

    void lockWrite() {
        std::unique_lock<std::mutex> lock(mtx);
        ++writers_waiting;
        cv.wait(lock, [this]{ 
            return readers == 0 && writers_active == 0; 
        });
        --writers_waiting;
        ++writers_active;
    }

    void unlockWrite() {
        std::unique_lock<std::mutex> lock(mtx);
        --writers_active;
        cv.notify_all();
    }
};
```

## 使用

```cpp
ReadWriteLock rwlock;
int shared_data;

void read() {
    ReadWriteLock::ReadLock lock(rwlock);
    // 安全读取 shared_data
}

void write(int value) {
    ReadWriteLock::WriteLock lock(rwlock);
    // 安全写入 shared_data
}
```
