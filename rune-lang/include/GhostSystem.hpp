#pragma once

#include <string>
#include <vector>
#include <memory>
#include "RuneSystem.hpp"
#include "RuneLogger.hpp"

namespace RuneLang {

// GhostC OS specific features
class GhostSystem {
public:
    // System Management
    static GhostSystem& getInstance() {
        static GhostSystem instance;
        return instance;
    }

    bool initialize() {
        try {
            initialized_ = true;
            LOG_INFO("GhostSystem initialized");
            return true;
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to initialize GhostSystem: ", e.what());
            return false;
        }
    }

    void shutdown() {
        if (initialized_) {
            initialized_ = false;
            LOG_INFO("GhostSystem shut down");
        }
    }

    bool isInitialized() const {
        return initialized_;
    }

    size_t allocateMemory(size_t size) {
        if (!initialized_) {
            LOG_ERROR("Cannot allocate memory: system not initialized");
            return 0;
        }
        // Simulate memory allocation
        totalAllocated_ += size;
        return size;
    }

    void freeMemory(size_t size) {
        if (!initialized_) {
            LOG_ERROR("Cannot free memory: system not initialized");
            return;
        }
        if (size <= totalAllocated_) {
            totalAllocated_ -= size;
        }
    }

    size_t getFreeMemory() const {
        return maxMemory_ - totalAllocated_;
    }

    // Security
    static bool setSecurityLevel(const std::string& level);
    static std::string getCurrentSecurityLevel();
    static bool authenticateUser(const std::string& username, const std::string& password);

    // Resource Management
    static size_t getSystemLoad();
    static size_t getNetworkUsage();
    static size_t getDiskUsage();
    static std::vector<std::string> getLoadedModules();

    // Ghost-specific features
    static bool loadGhostModule(const std::string& moduleName);
    static bool unloadGhostModule(const std::string& moduleName);
    static std::string getGhostVersion();
    static bool updateGhostSystem();

private:
    GhostSystem() : initialized_(false), totalAllocated_(0), maxMemory_(1024 * 1024 * 1024) {}
    ~GhostSystem() {
        shutdown();
    }

    GhostSystem(const GhostSystem&) = delete;
    GhostSystem& operator=(const GhostSystem&) = delete;

    bool initialized_;
    size_t totalAllocated_;
    const size_t maxMemory_;
};

// GhostC OS specific process class
class GhostProcess : public RuneProcess {
public:
    GhostProcess() : RuneProcess(), priority_(0), affinity_(0) {}

    // Process management
    void setPriority(int priority) {
        priority_ = priority;
        LOG_INFO("Process priority set to ", priority);
    }

    int getPriority() const {
        return priority_;
    }

    void setAffinity(int affinity) {
        affinity_ = affinity;
        LOG_INFO("Process affinity set to ", affinity);
    }

    int getAffinity() const {
        return affinity_;
    }

    bool setProcessSecurity(const std::string& securityLevel);
    std::string getProcessStatus() const;
    size_t getProcessMemoryUsage() const;

    // IPC methods
    bool sendMessage(GhostProcess* target, const std::string& message);
    std::string receiveMessage();
    bool createSharedMemory(const std::string& name, size_t size);
    void* mapSharedMemory(const std::string& name);

    // Resource limits
    bool setResourceLimit(const std::string& resource, size_t limit);
    size_t getResourceUsage(const std::string& resource) const;

private:
    int priority_;
    int affinity_;
};

// GhostC OS specific file system features
class GhostFileSystem : public RuneFileSystem {
public:
    // Ghost-specific filesystem operations
    static bool mountGhostFS(const std::string& device, const std::string& mountPoint);
    static bool createGhostPartition(const std::string& device, size_t size);
    static bool encryptFile(const std::string& path, const std::string& key);
    static bool decryptFile(const std::string& path, const std::string& key);
    static bool setFileCompression(const std::string& path, const std::string& algorithm);

    // Advanced file operations
    static bool createHardLink(const std::string& source, const std::string& target);
    static bool createSymLink(const std::string& source, const std::string& target);
    static bool setExtendedAttribute(const std::string& path, const std::string& name, const std::string& value);
    static std::string getExtendedAttribute(const std::string& path, const std::string& name);
};

} // namespace RuneLang
