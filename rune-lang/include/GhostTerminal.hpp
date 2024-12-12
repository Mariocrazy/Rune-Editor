#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <map>
#include "RuneLogger.hpp"

namespace RuneLang {

class GhostTerminal {
public:
    static GhostTerminal& getInstance() {
        static GhostTerminal instance;
        return instance;
    }

    void initialize();
    void shutdown();
    void processCommand(const std::string& command);
    void registerCommand(const std::string& name, std::function<void(const std::vector<std::string>&)> handler);
    void clearScreen();
    void displayPrompt();
    void displayBanner();
    void setTerminalColor(const std::string& color);
    
    // Boot sequence
    void startBootSequence();
    void displayBootProgress(int percentage);
    
private:
    GhostTerminal() = default;
    ~GhostTerminal() = default;
    GhostTerminal(const GhostTerminal&) = delete;
    GhostTerminal& operator=(const GhostTerminal&) = delete;

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> commands;
    std::string currentDirectory;
    bool isRunning;

    void registerDefaultCommands();
    std::vector<std::string> parseCommand(const std::string& cmdLine);
    void displayLoadingBar(int percentage);
};

} // namespace RuneLang
