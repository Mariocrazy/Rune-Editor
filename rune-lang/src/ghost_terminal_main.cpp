#include "GhostTerminal.hpp"
#include <iostream>
#include <string>

int main() {
    auto& terminal = RuneLang::GhostTerminal::getInstance();
    
    // Start boot sequence (takes about 5 minutes)
    terminal.startBootSequence();
    
    // Initialize terminal
    terminal.initialize();
    
    std::string command;
    while (true) {
        terminal.displayPrompt();
        std::getline(std::cin, command);
        
        if (command == "exit") {
            break;
        }
        
        terminal.processCommand(command);
    }
    
    terminal.shutdown();
    return 0;
}
