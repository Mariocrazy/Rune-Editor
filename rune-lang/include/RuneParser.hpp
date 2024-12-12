#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "RuneSystem.hpp"

namespace RuneLang {

// Forward declarations
class RuneParser;
class Function;
class Class;

// Custom exception for rune parsing errors
class RuneParseError : public std::runtime_error {
public:
    explicit RuneParseError(const std::string& message, size_t line = 0, size_t column = 0) 
        : std::runtime_error(formatError(message, line, column)),
          line_(line),
          column_(column) {}

    size_t getLine() const { return line_; }
    size_t getColumn() const { return column_; }

private:
    size_t line_;
    size_t column_;
    
    static std::string formatError(const std::string& message, size_t line, size_t column) {
        if (line == 0) return message;
        return "Line " + std::to_string(line) + ", Column " + std::to_string(column) + ": " + message;
    }
};

// Structure to represent a function
class Function {
public:
    std::string returnType;
    std::string name;
    std::vector<std::string> parameters;
    std::string body;

    Function(const std::string& ret, const std::string& n) 
        : returnType(ret), name(n) {}
};

// Structure to represent a class
class Class {
public:
    std::string name;
    std::vector<Function> methods;
    std::vector<std::string> members;

    explicit Class(const std::string& n) : name(n) {}
};

// Main parser class
class RuneParser {
public:
    RuneParser();
    std::string parseRuneCode(const std::string& runeCode);
    std::string compileToCpp(const std::string& runeCode);
    std::string handleCustomType(const std::string& code, size_t& pos);
    
private:
    std::unordered_map<std::string, std::string> runeToKeyword;
    size_t currentLine;
    size_t currentColumn;
    
    void initializeRuneMap();
    std::string handleFunction(const std::string& code, size_t& pos);
    std::string handleClass(const std::string& code, size_t& pos);
    std::string handleSwitch(const std::string& code, size_t& pos);
    std::string handleString(const std::string& code, size_t& pos);
    std::string handleComment(const std::string& code, size_t& pos);
    std::string handleSystemCall(const std::string& code, size_t& pos);
    std::string handleProcessManagement(const std::string& code, size_t& pos);
    std::string handleFileOperation(const std::string& code, size_t& pos);
    void updatePosition(const std::string& code, size_t& pos);
    void skipWhitespace(const std::string& code, size_t& pos);
};

} // namespace RuneLang
