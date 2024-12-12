#pragma once

#include <string>
#include <vector>
#include <functional>
#include <pthread.h>
#include <sys/types.h>

namespace RuneLang {

// Forward declaration
void* threadWrapper(void* arg);

// Process management class
class RuneProcess {
public:
    RuneProcess();
    ~RuneProcess();

    bool start(const std::string& command);
    void stop();
    bool isRunning() const;
    pid_t getPid() const { return pid_; }

protected:
    pid_t pid_;
};

// Thread management class
class RuneThread {
public:
    RuneThread();
    ~RuneThread();

    bool start(std::function<void()> func);
    void join();
    void runThread();
    bool isRunning() const;
    pthread_t getThreadID() const;

private:
    friend void* threadWrapper(void* arg);
    pthread_t threadId_;
    std::function<void()> threadFunc_;
};

// File system operations class
class RuneFileSystem {
public:
    static bool createFile(const std::string& path, const std::string& content);
    static bool deleteFile(const std::string& path);
    static bool createDirectory(const std::string& path);
    static bool deleteDirectory(const std::string& path);
    static bool mount(const std::string& device, const std::string& mountPoint);
    static bool unmount(const std::string& mountPoint);
};

// Memory management class
class RuneMemory {
public:
    static void* allocate(size_t size);
    static void deallocate(void* ptr);
    static void* reallocate(void* ptr, size_t newSize);
    static size_t getUsage();
};

} // namespace RuneLang
