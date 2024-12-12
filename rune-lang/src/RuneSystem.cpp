#include "../include/RuneSystem.hpp"
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

namespace RuneLang {

// RuneProcess implementation
RuneProcess::RuneProcess() : pid_(0) {}

RuneProcess::~RuneProcess() {
    if (pid_ > 0) {
        stop();
    }
}

bool RuneProcess::start(const std::string& command) {
    pid_ = fork();
    if (pid_ < 0) {
        // Fork failed
        return false;
    } else if (pid_ == 0) {
        // Child process
        execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
        exit(1); // Only reached if execl fails
    }
    return true; // Parent process
}

void RuneProcess::stop() {
    if (pid_ > 0) {
        kill(pid_, SIGTERM);
        waitpid(pid_, nullptr, 0);
        pid_ = 0;
    }
}

bool RuneProcess::isRunning() const {
    return pid_ > 0;
}

// RuneThread implementation
RuneThread::RuneThread() : threadId_(0) {}

RuneThread::~RuneThread() {
    if (threadId_ != 0) {
        pthread_cancel(threadId_);
        pthread_join(threadId_, nullptr);
    }
}

bool RuneThread::start(std::function<void()> func) {
    threadFunc_ = std::move(func);
    return (pthread_create(&threadId_, nullptr, threadWrapper, this) == 0);
}

void RuneThread::join() {
    if (threadId_ != 0) {
        pthread_join(threadId_, nullptr);
        threadId_ = 0;
    }
}

void RuneThread::runThread() {
    threadFunc_();
}

bool RuneThread::isRunning() const {
    return threadId_ != 0;
}

pthread_t RuneThread::getThreadID() const {
    return threadId_;
}

void* threadWrapper(void* arg) {
    RuneThread* thread = static_cast<RuneThread*>(arg);
    thread->runThread();
    return nullptr;
}

// RuneFileSystem implementation
bool RuneFileSystem::createFile(const std::string& path, const std::string& content) {
    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return false;
    }
    
    ssize_t written = write(fd, content.c_str(), content.length());
    close(fd);
    return written == static_cast<ssize_t>(content.length());
}

bool RuneFileSystem::deleteFile(const std::string& path) {
    return unlink(path.c_str()) == 0;
}

bool RuneFileSystem::createDirectory(const std::string& path) {
    return mkdir(path.c_str(), 0755) == 0;
}

bool RuneFileSystem::deleteDirectory(const std::string& path) {
    return rmdir(path.c_str()) == 0;
}

bool RuneFileSystem::mount(const std::string& device, const std::string& mountPoint) {
    // Implementation depends on specific filesystem requirements
    // For now, return success
    return true;
}

bool RuneFileSystem::unmount(const std::string& mountPoint) {
    // Implementation depends on specific filesystem requirements
    // For now, return success
    return true;
}

// RuneMemory implementation
void* RuneMemory::allocate(size_t size) {
    return malloc(size);
}

void RuneMemory::deallocate(void* ptr) {
    free(ptr);
}

void* RuneMemory::reallocate(void* ptr, size_t newSize) {
    return realloc(ptr, newSize);
}

size_t RuneMemory::getUsage() {
    // This is a simplified implementation
    // For accurate memory usage, you would need to use platform-specific APIs
    return 0;
}

} // namespace RuneLang
