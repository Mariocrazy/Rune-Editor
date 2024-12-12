#pragma once

#include <string>
#include <map>
#include <mutex>
#include <chrono>
#include <thread>
#include <functional>
#include <atomic>
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
    using MetricCallback = std::function<double()>;

    static RuneMonitor& getInstance() {
        static RuneMonitor instance;
        return instance;
    }

    void registerMetric(const std::string& name, MetricCallback callback) {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        metricCallbacks_[name] = callback;
    }

    void unregisterMetric(const std::string& name) {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        metricCallbacks_.erase(name);
    }

    double getMetric(const std::string& name) const {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        auto it = metricCallbacks_.find(name);
        if (it != metricCallbacks_.end()) {
            try {
                return it->second();
            } catch (const std::exception& e) {
                LOG_ERROR("Error getting metric ", name, ": ", e.what());
                return 0.0;
            }
        }
        return 0.0;
    }

    void startMonitoring(std::chrono::milliseconds interval = std::chrono::seconds(1)) {
        bool expected = false;
        if (isRunning_.compare_exchange_strong(expected, true)) {
            monitorThread_ = std::thread([this, interval]() {
                while (isRunning_.load()) {
                    try {
                        updateMetrics();
                        checkThresholds();
                    } catch (const std::exception& e) {
                        LOG_ERROR("Error in monitoring thread: ", e.what());
                    }
                    std::this_thread::sleep_for(interval);
                }
            });
        }
    }

    void stopMonitoring() {
        bool expected = true;
        if (isRunning_.compare_exchange_strong(expected, false)) {
            if (monitorThread_.joinable()) {
                monitorThread_.join();
            }
        }
    }

    void setThreshold(const std::string& metric, double threshold, std::function<void(double)> callback) {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        thresholds_[metric] = {threshold, callback};
    }

    SystemMetrics getCurrentMetrics() const {
        std::lock_guard<std::mutex> lock(metricsMutex_);
        return currentMetrics_;
    }

private:
    RuneMonitor() : isRunning_(false) {}
    ~RuneMonitor() {
        stopMonitoring();
    }

    RuneMonitor(const RuneMonitor&) = delete;
    RuneMonitor& operator=(const RuneMonitor&) = delete;

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
                LOG_DEBUG("Metric ", metricName, " = ", currentMetrics_.customMetrics[metricName]);
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
                LOG_WARNING("Threshold exceeded for ", metric, ": ", value, " > ", threshold.first);
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
    std::map<std::string, MetricCallback> metricCallbacks_;
    std::map<std::string, std::pair<double, std::function<void(double)>>> thresholds_;
};

} // namespace RuneLang
