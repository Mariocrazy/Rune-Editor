#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include "RuneLogger.hpp"

namespace RuneLang {

struct SystemMetrics {
    double cpuUsage;
    size_t memoryUsage;
    size_t diskUsage;
    size_t networkUsage;
    size_t processCount;
    std::map<std::string, double> customMetrics;
};

class RuneMonitor {
public:
    static RuneMonitor& getInstance() {
        static RuneMonitor instance;
        return instance;
    }

    void start() {
        if (!isRunning_) {
            isRunning_ = true;
            monitorThread_ = std::thread(&RuneMonitor::monitorLoop, this);
        }
    }

    void stop() {
        if (isRunning_) {
            isRunning_ = false;
            if (monitorThread_.joinable()) {
                monitorThread_.join();
            }
        }
    }

    void addMetricCallback(const std::string& name, 
                          std::function<double()> callback) {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        metricCallbacks_[name] = callback;
    }

    void setThreshold(const std::string& metric, double threshold,
                     std::function<void(double)> callback) {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        thresholds_[metric] = {threshold, callback};
    }

    SystemMetrics getCurrentMetrics() const {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        return currentMetrics_;
    }

private:
    RuneMonitor() : isRunning_(false) {}
    ~RuneMonitor() { stop(); }

    void monitorLoop() {
        while (isRunning_) {
            try {
                updateMetrics();
                checkThresholds();
            } catch (const std::exception& e) {
                LOG_ERROR("Error in monitoring thread: ", e.what());
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void updateMetrics() {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        
        // Update system metrics
        currentMetrics_.cpuUsage = getCPUUsage();
        currentMetrics_.memoryUsage = getMemoryUsage();
        currentMetrics_.diskUsage = getDiskUsage();
        currentMetrics_.networkUsage = getNetworkUsage();
        currentMetrics_.processCount = getProcessCount();

        // Update custom metrics
        for (const auto& [metricName, callback] : metricCallbacks_) {
            try {
                currentMetrics_.customMetrics[metricName] = callback();
            } catch (const std::exception& e) {
                LOG_ERROR("Error updating metric ", metricName, ": ", e.what());
            }
        }
    }

    void checkThresholds() {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        
        for (const auto& [metric, threshold] : thresholds_) {
            double value = 0.0;
            
            if (metric == "cpu") value = currentMetrics_.cpuUsage;
            else if (metric == "memory") value = currentMetrics_.memoryUsage;
            else if (metric == "disk") value = currentMetrics_.diskUsage;
            else if (metric == "network") value = currentMetrics_.networkUsage;
            else {
                auto it = currentMetrics_.customMetrics.find(metric);
                if (it != currentMetrics_.customMetrics.end()) {
                    value = it->second;
                }
            }

            if (value > threshold.first) {
                LOG_WARNING("Threshold exceeded for ", metric, 
                           ": ", value, " > ", threshold.first);
                threshold.second(value);
            }
        }
    }

    double getCPUUsage() {
        // Implement real CPU usage monitoring
        return 0.0;
    }

    size_t getMemoryUsage() {
        // Implement real memory usage monitoring
        return 0;
    }

    size_t getDiskUsage() {
        // Implement real disk usage monitoring
        return 0;
    }

    size_t getNetworkUsage() {
        // Implement real network usage monitoring
        return 0;
    }

    size_t getProcessCount() {
        // Implement real process counting
        return 0;
    }

    std::atomic<bool> isRunning_;
    std::thread monitorThread_;
    mutable std::mutex metricsMutex_;
    SystemMetrics currentMetrics_;
    std::map<std::string, std::function<double()>> metricCallbacks_;
    std::map<std::string, std::pair<double, std::function<void(double)>>> thresholds_;
};

} // namespace RuneLang
