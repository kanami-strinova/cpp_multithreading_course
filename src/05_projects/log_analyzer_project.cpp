/**
 * @file log_analyzer_project.cpp
 * @brief 项目实战：并发日志分析器
 */

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct LogStats {
    atomic<int> total{0};
    atomic<int> errors{0};
    atomic<int> warnings{0};
    atomic<int> info{0};
};

class LogAnalyzer {
private:
    LogStats stats;

public:
    void analyze(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Cannot open file: " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stats.total++;
            
            if (line.find("ERROR") != string::npos) stats.errors++;
            else if (line.find("WARNING") != string::npos) stats.warnings++;
            else if (line.find("INFO") != string::npos) stats.info++;
        }
    }

    void analyze_chunk(const string& content) {
        istringstream stream(content);
        string line;
        
        while (getline(stream, line)) {
            stats.total++;
            if (line.find("ERROR") != string::npos) stats.errors++;
            else if (line.find("WARNING") != string::npos) stats.warnings++;
            else if (line.find("INFO") != string::npos) stats.info++;
        }
    }

    void print() {
        cout << "\n=== Log Analysis Results ===" << endl;
        cout << "Total lines: " << stats.total << endl;
        cout << "Errors: " << stats.errors << endl;
        cout << "Warnings: " << stats.warnings << endl;
        cout << "Info: " << stats.info << endl;
    }
};

int main() {
    cout << "=== Concurrent Log Analyzer ===" << endl;
    
    // 模拟日志内容
    vector<string> chunks = {
        "[INFO] System started\n[ERROR] Connection failed\n[WARNING] Low memory",
        "[INFO] User logged in\n[ERROR] File not found\n[INFO] Task completed",
        "[WARNING] High CPU usage\n[INFO] Backup started\n[ERROR] Timeout"
    };

    LogAnalyzer analyzer;
    vector<thread> threads;

    // 并发分析
    for (const auto& chunk : chunks) {
        threads.emplace_back(&LogAnalyzer::analyze_chunk, &analyzer, chunk);
    }

    for (auto& t : threads) t.join();

    analyzer.print();

    return 0;
}
