#include "GhostTerminal.hpp"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iomanip>

namespace RuneLang {

void GhostTerminal::initialize() {
    registerDefaultCommands();
    isRunning = true;
    currentDirectory = "/";
    clearScreen();
}

void GhostTerminal::startBootSequence() {
    clearScreen();
    
    // Display initial boot message
    std::cout << "\033[32m" << "GhostC OS Bootloader v1.0" << "\033[0m" << std::endl;
    std::cout << "Starting boot sequence...\n" << std::endl;
    
    // Simulate various boot stages
    const std::vector<std::string> bootStages = {
        "Initializing kernel...",
        "Loading system modules...",
        "Mounting ghost partitions...",
        "Starting security services...",
        "Initializing Rune environment...",
        "Loading ghost protocols...",
        "Establishing secure connections...",
        "Starting system services...",
        "Performing security checks...",
        "Finalizing boot sequence..."
    };

    int totalSteps = bootStages.size();
    for (int i = 0; i < totalSteps; i++) {
        int percentage = (i * 100) / totalSteps;
        std::cout << "\033[32m[" << std::setw(3) << percentage << "%]\033[0m ";
        std::cout << bootStages[i] << std::endl;
        displayBootProgress(percentage);
        
        // Random delay between 25-35 seconds per stage
        int delay = 25 + (rand() % 11);
        std::this_thread::sleep_for(std::chrono::seconds(delay));
    }

    clearScreen();
    displayBanner();
    std::cout << "\n\033[32mBoot sequence completed. Welcome to GhostC OS.\033[0m\n" << std::endl;
}

void GhostTerminal::displayBanner() {
    std::cout << "\033[32m";
    std::cout << R"(
   ▄██████▄     ▄█    █▄     ▄██████▄     ▄████████     ███      ▄████████    ▄██████▄     ▄████████ 
  ███    ███   ███    ███   ███    ███   ███    ███ ▀█████████▄ ███    ███   ███    ███   ███    ███ 
  ███    █▀    ███    ███   ███    ███   ███    █▀     ▀███▀▀██ ███    █▀    ███    ███   ███    █▀  
 ▄███         ▄███▄▄▄▄███▄▄ ███    ███   ███            ███   ▀ ███         ▄███    ███   ███        
▀▀███ ████▄  ▀▀███▀▀▀▀███▀  ███    ███ ▀███████████     ███     ███        ▀▀███    ███ ▀███████████ 
  ███    ███   ███    ███   ███    ███          ███     ███     ███    █▄    ███    ███          ███ 
  ███    ███   ███    ███   ███    ███    ▄█    ███     ███     ███    ███   ███    ███    ▄█    ███ 
  ████████▀    ███    █▀     ▀██████▀   ▄████████▀     ▄████▀   ████████▀     ▀██████▀   ▄████████▀  
    )" << std::endl;
    
    std::cout << "\033[36m";
    std::cout << "                                  by: GHOST Sec" << std::endl;
    std::cout << "\033[0m";
}

void GhostTerminal::displayBootProgress(int percentage) {
    std::cout << "\033[32m[";
    int pos = 50 * percentage / 100;
    for (int i = 0; i < 50; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << percentage << "%\033[0m\r";
    std::cout.flush();
}

void GhostTerminal::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void GhostTerminal::displayPrompt() {
    std::cout << "\033[32mghost@ghostc\033[0m:\033[34m" << currentDirectory << "\033[0m$ ";
}

void GhostTerminal::processCommand(const std::string& cmdLine) {
    auto args = parseCommand(cmdLine);
    if (args.empty()) return;

    std::string cmd = args[0];
    args.erase(args.begin());

    auto it = commands.find(cmd);
    if (it != commands.end()) {
        it->second(args);
    } else {
        std::cout << "ghost: command not found: " << cmd << std::endl;
    }
}

std::vector<std::string> GhostTerminal::parseCommand(const std::string& cmdLine) {
    std::vector<std::string> args;
    std::stringstream ss(cmdLine);
    std::string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }
    return args;
}

void GhostTerminal::registerCommand(const std::string& name, 
    std::function<void(const std::vector<std::string>&)> handler) {
    commands[name] = handler;
}

void GhostTerminal::registerDefaultCommands() {
    // Clear screen
    registerCommand("clear", [this](const auto&) { clearScreen(); });

    // Exit
    registerCommand("exit", [this](const auto&) { 
        std::cout << "Shutting down GhostC OS..." << std::endl;
        isRunning = false; 
    });

    // Help
    registerCommand("help", [](const auto&) {
        std::cout << "Available commands:\n"
                  << "  clear    - Clear the screen\n"
                  << "  exit     - Exit GhostC OS\n"
                  << "  help     - Display this help message\n"
                  << "  ls       - List directory contents\n"
                  << "  cd       - Change directory\n"
                  << "  pwd      - Print working directory\n";
    });

    // PWD
    registerCommand("pwd", [this](const auto&) {
        std::cout << currentDirectory << std::endl;
    });

    // CD
    registerCommand("cd", [this](const auto& args) {
        if (args.empty()) {
            currentDirectory = "/";
        } else {
            // Simple path handling - could be enhanced
            if (args[0] == "..") {
                size_t pos = currentDirectory.find_last_of('/');
                if (pos != 0) {
                    currentDirectory = currentDirectory.substr(0, pos);
                }
            } else if (args[0][0] == '/') {
                currentDirectory = args[0];
            } else {
                if (currentDirectory == "/") {
                    currentDirectory += args[0];
                } else {
                    currentDirectory += "/" + args[0];
                }
            }
        }
    });

    // LS
    registerCommand("ls", [](const auto&) {
        std::cout << "Directory listing not implemented yet." << std::endl;
    });
}

void GhostTerminal::shutdown() {
    clearScreen();
    std::cout << "\033[32mShutting down GhostC OS...\033[0m" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    clearScreen();
}

} // namespace RuneLang
