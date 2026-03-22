/**
 * @file thread_sync.cpp
 * @brief 线程同步示例
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

using namespace std;

class Counter {
private:
    int count = 0;
    mutex mtx;

public:
    void increment() {
        lock_guard<mutex> lock(mtx);
        ++count;
    }

    int get() {
        lock_guard<mutex> lock(mtx);
        return count;
    }
};

int main() {
    cout << "=== Thread Sync Demo ===" << endl;
    
    Counter counter;
    const int ITERATIONS = 10000;
    const int THREADS = 4;
    
    vector<thread> threads;
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back([&counter, ITERATIONS] {
            for (int j = 0; j < ITERATIONS; ++j) {
                counter.increment();
            }
        });
    }
    
    for (auto& t : threads) t.join();
    
    cout << "Final count: " << counter.get() << endl;
    cout << "Expected: " << THREADS * ITERATIONS << endl;
    
    return 0;
}
