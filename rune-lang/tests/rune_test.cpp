#include <cassert>
#include "RuneError.hpp"
#include "RuneMonitor.hpp"
#include "RuneLogger.hpp"
#include "GhostSystem.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace RuneLang;

void testSystemInitialization() {
    GhostSystem& system = GhostSystem::getInstance();
    assert(system.initialize() == true);
    assert(system.isInitialized() == true);
    system.shutdown();
    assert(system.isInitialized() == false);
}

void testProcessManagement() {
    GhostSystem& system = GhostSystem::getInstance();
    system.initialize();

    GhostProcess process;
    process.setPriority(10);
    assert(process.getPriority() == 10);

    process.setAffinity(1);
    assert(process.getAffinity() == 1);

    system.shutdown();
}

void testMemoryManagement() {
    GhostSystem& system = GhostSystem::getInstance();
    system.initialize();

    size_t allocated = system.allocateMemory(1024);
    assert(allocated == 1024);
    assert(system.getFreeMemory() >= 0);
    system.freeMemory(allocated);

    system.shutdown();
}

void testMonitoring() {
    RuneMonitor& monitor = RuneMonitor::getInstance();
    
    // Register a test metric
    monitor.registerMetric("test_metric", []() { return 42.0; });
    
    // Start monitoring
    monitor.startMonitoring(std::chrono::milliseconds(100));
    
    // Wait a bit for some metrics to be collected
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // Check the metric value
    assert(monitor.getMetric("test_metric") == 42.0);
    
    // Stop monitoring
    monitor.stopMonitoring();
    
    // Unregister the metric
    monitor.unregisterMetric("test_metric");
}

void testErrorHandling() {
    try {
        RUNE_THROW(RuneError::ErrorCode::SYSTEM_ERROR, "Test error");
        assert(false && "Should not reach here");
    } catch (const RuneError& e) {
        assert(e.getCode() == RuneError::ErrorCode::SYSTEM_ERROR);
    }
}

void testLogging() {
    LogLevelUtils::setLevel(LogLevel::DEBUG);
    LOG_DEBUG("Debug message");
    LOG_INFO("Info message");
    LOG_WARNING("Warning message");
    LOG_ERROR("Error message");
    LOG_CRITICAL("Critical message");
}

int main() {
    std::cout << "Running Rune tests..." << std::endl;

    // Initialize logging
    LogLevelUtils::setLevel(LogLevel::DEBUG);
    RuneLogger::getInstance().setLogFile("rune_test.log");

    try {
        testSystemInitialization();
        std::cout << "System initialization test passed" << std::endl;

        testProcessManagement();
        std::cout << "Process management test passed" << std::endl;

        testMemoryManagement();
        std::cout << "Memory management test passed" << std::endl;

        testMonitoring();
        std::cout << "Monitoring test passed" << std::endl;

        testErrorHandling();
        std::cout << "Error handling test passed" << std::endl;

        testLogging();
        std::cout << "Logging test passed" << std::endl;

        std::cout << "All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Test failed: ", e.what());
        return 1;
    }
}
