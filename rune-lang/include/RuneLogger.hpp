#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <mutex>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace RuneLang {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class RuneLogger {
public:
    static RuneLogger& getInstance() {
        static RuneLogger instance;
        return instance;
    }

    void setLogLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(mutex_);
        currentLevel_ = level;
    }

    void setLogFile(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (logFile_.is_open()) {
            logFile_.close();
        }
        logFile_.open(filename, std::ios::app);
    }

    template<typename... Args>
    void log(LogLevel level, const char* file, int line, Args&&... args) {
        if (level < currentLevel_) return;

        std::lock_guard<std::mutex> lock(mutex_);
        
        // Get current time
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto localTime = *std::localtime(&time);

        // Format message
        std::stringstream ss;
        ss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << " "
           << "[" << toString(level) << "] "
           << file << ":" << line << " - ";
        (ss << ... << std::forward<Args>(args)) << std::endl;

        // Write to console and file
        std::cerr << ss.str();
        if (logFile_.is_open()) {
            logFile_ << ss.str();
            logFile_.flush();
        }
    }

private:
    RuneLogger() : currentLevel_(LogLevel::INFO) {}
    ~RuneLogger() {
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }

    RuneLogger(const RuneLogger&) = delete;
    RuneLogger& operator=(const RuneLogger&) = delete;

    std::string toString(LogLevel level) const {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }

    LogLevel currentLevel_;
    std::mutex mutex_;
    std::ofstream logFile_;
};

namespace LogLevelUtils {
    inline void setLevel(LogLevel level) {
        RuneLogger::getInstance().setLogLevel(level);
    }

    inline std::string toString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }
}

} // namespace RuneLang

#define LOG_DEBUG(...)   RuneLogger::getInstance().log(LogLevel::DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)    RuneLogger::getInstance().log(LogLevel::INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) RuneLogger::getInstance().log(LogLevel::WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)   RuneLogger::getInstance().log(LogLevel::ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_CRITICAL(...) RuneLogger::getInstance().log(LogLevel::CRITICAL, __FILE__, __LINE__, __VA_ARGS__)
