/**
 * @file producer_consumer.cpp
 * @brief 生产者-消费者模式
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

using namespace std;

class PCQueue {
private:
    queue<int> q;
    mutex mtx;
    condition_variable cv;
    atomic<bool> stop_{false};

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
        cv.wait(lock, [this] { return !q.empty() || stop_.load(); });
        if (q.empty()) return -1;
        int value = q.front();
        q.pop();
        return value;
    }

    void stop() { stop_ = true; cv.notify_all(); }
};

int main() {
    cout << "=== Producer-Consumer Demo ===" << endl;
    
    PCQueue queue;
    atomic<int> produced{0}, consumed{0};
    
    // 生产者
    thread producer([&queue, &produced] {
        for (int i = 0; i < 20; ++i) {
            queue.push(i);
            produced++;
            cout << "Produced: " << i << endl;
            this_thread::sleep_for(50ms);
        }
        queue.stop();
    });
    
    // 消费者
    thread consumer([&queue, &consumed] {
        while (true) {
            int value = queue.pop();
            if (value == -1) break;
            consumed++;
            cout << "Consumed: " << value << endl;
            this_thread::sleep_for(80ms);
        }
    });
    
    producer.join();
    consumer.join();
    
    cout << "Produced: " << produced << ", Consumed: " << consumed << endl;
    
    return 0;
}
