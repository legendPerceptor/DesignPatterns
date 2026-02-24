/*
The intent is to ensure a class has only one instance, and provide a global access point to it.
In modern C++:
- Use a private constructor (no one else can new it).
- Delete copy/move constructor to avoid copies.
- Provide static Logger& instance() that returns a reference to the one-and-only object.
- Use a function-local static for thread-safe lazy initialization.
*/
#ifndef A5_SINGLETON_H
#define A5_SINGLETON_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace Singleton {

class Logger {
    public:
    // Global access point
    static Logger& instance () {
        static Logger instance; // C++11+: thread-safe initialization
        return instance;
    }

    // Delete copy operations — no copying allowed
    Logger (const Logger&)            = delete;
    Logger& operator= (const Logger&) = delete;

    // Optional: also delete move operations
    Logger (Logger&&)            = delete;
    Logger& operator= (Logger&&) = delete;

    // Simple log API
    void log (const std::string& message) {
        std::lock_guard<std::mutex> lock (mutex_);

        // write to file if open, else fall back to std::cerr
        if (file_.is_open ()) {
            file_ << message << '\n';
            file_.flush ();
        } else {
            std::cerr << message << '\n';
        }
    }

    private:
    Logger () {
        // Try to open a log file
        file_.open ("app.log", std::ios::app);
        if (!file_) {
            std::cerr
            << "[Logger] Failed to open app.log, logging to stderr.\n";
        }
    }

    ~Logger () {
        if (file_.is_open ()) {
            file_ << "[Logger] Shutting down.\n";
            file_.close ();
        }
    }

    std::ofstream file_;
    std::mutex mutex_;
};
} // namespace Singleton

#endif