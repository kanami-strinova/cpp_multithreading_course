/**
 * @file 02_sync_solutions.cpp
 * @brief 第二章练习答案
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

using namespace std;

// 练习 2.1: 线程安全的计数器
class SafeCounter {
private:
    int count = 0;
    mutex mtx;

public:
    void increment() {
        lock_guard<mutex> lock(mtx);
        count++;
    }

    int get() {
        lock_guard<mutex> lock(mtx);
        return count;
    }
};

// 练习 2.2: 生产者-消费者队列
class PCQueue {
private:
    queue<int> q;
    mutex mtx;
    condition_variable cv;
    bool stop_ = false;

public:
    void push(int value) {
        {
            lock_guard<mutex> lock(mtx);
            q.push(value);
        }
        cv.notify_one();
    }

    int pop() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this] { return !q.empty() || stop_; });
        if (q.empty()) return -1;
        int value = q.front();
        q.pop();
        return value;
    }

    void stop() {
        {
            lock_guard<mutex> lock(mtx);
            stop_ = true;
        }
        cv.notify_all();
    }
};

int main() {
    // 测试 SafeCounter
    SafeCounter counter;
    vector<thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& t : threads) t.join();
    cout << "Counter: " << counter.get() << endl;

    // 测试 PCQueue
    PCQueue queue;
    bool running = true;

    thread producer([&queue, &running]() {
        for (int i = 0; i < 10; ++i) {
            queue.push(i);
            cout << "Produced: " << i << endl;
        }
        queue.stop();
    });

    thread consumer([&queue, &running]() {
        while (true) {
            int value = queue.pop();
            if (value == -1) break;
            cout << "Consumed: " << value << endl;
        }
    });

    producer.join();
    consumer.join();

    return 0;
}
