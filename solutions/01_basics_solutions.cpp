/**
 * @file 01_basics_solutions.cpp
 * @brief 第一章练习答案
 */

#include <iostream>
#include <thread>
#include <vector>
#include <string>

using namespace std;

// 练习 1.1: 创建多个线程
void exercise1_1() {
    auto worker = [](int id) {
        cout << "Thread " << this_thread::get_id() 
             << " (ID: " << id << ") started" << endl;
        this_thread::sleep_for(100ms);
        cout << "Thread " << id << " finished" << endl;
    };

    vector<thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }
}

// 练习 1.2: 参数传递
void exercise1_2() {
    // 注意：参数按值传递
    auto byValue = [](int value) {
        cout << "By value: " << value << endl;
    };

    // 使用 ref 传递引用
    auto byRef = [](int& value) {
        value++;
        cout << "By reference: " << value << endl;
    };

    int x = 10;
    thread t1(byValue, x);  // x 按值传递
    thread t2(ref(byRef), ref(x));  // x 按引用传递

    t1.join();
    t2.join();
    cout << "Final x: " << x << endl;  // x = 11
}

// 练习 1.3: 线程局部存储
void exercise1_3() {
    thread_local int counter = 0;

    auto increment = [&counter]() {
        counter++;
        cout << "Thread " << this_thread::get_id() 
             << " counter: " << counter << endl;
    };

    thread t1(increment);
    thread t2(increment);
    thread t3(increment);

    t1.join();
    t2.join();
    t3.join();

    // 每个线程的 counter 是独立的
    cout << "Main thread counter: " << counter << endl;
}

int main() {
    cout << "=== Exercise 1.1 ===" << endl;
    exercise1_1();

    cout << "\n=== Exercise 1.2 ===" << endl;
    exercise1_2();

    cout << "\n=== Exercise 1.3 ===" << endl;
    exercise1_3();

    return 0;
}
