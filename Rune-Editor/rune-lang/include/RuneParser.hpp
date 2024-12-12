#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace RuneLang {

// Forward declarations
class RuneParser;
class Function;
class Class;

// Custom exception for rune parsing errors
class RuneParseError : public std::runtime_error {
public:
    explicit RuneParseError(const std::string& message) : std::runtime_error(message) {}
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
    void initializeRuneMap();
    std::string handleFunction(const std::string& code, size_t& pos);
    std::string handleClass(const std::string& code, size_t& pos);
    std::string handleVariableDeclaration(const std::string& code, size_t& pos);

    // Add new mappings for arrays and complex data types
    // Example: ᚨ for arrays, ᛚ for float arrays, ᛦ for double arrays
    // Update the runeToKeyword map accordingly.
    void addArrayMappings() {
        runeToKeyword["ᚨ"] = "std::vector<";  // Array declaration
        runeToKeyword["ᛚ"] = "float";  // Float array type
        runeToKeyword["ᛦ"] = "double";  // Double array type
    }

    // Add new mappings for switch statements
    // Example: ᚹ for switch, ᚲ for case, ᛞ for default
    void addSwitchMappings() {
        runeToKeyword["ᚹ"] = "switch";  // Switch statement
        runeToKeyword["ᚲ"] = "case";  // Case statement
        runeToKeyword["ᛞ"] = "default";  // Default statement
    }

    void initializeRuneMap() {
        runeToKeyword = {
            {"ᚠ", "std::cout << "}, // Output to console
            {"ᚱ", "for"},           // For-loop
            {"ᚷ", "if"},            // If-condition
            {"ᛏ", "return"},        // Return from function
            {"ᚢ", "+"},             // Addition operator
            {"ᚦ", "-"},             // Subtraction operator
            {"ᛉ", "while"},         // While-loop
            {"ᚨ", "std::vector<"},  // Array declaration
            {"ᛚ", "float"},         // Float array type
            {"ᛦ", "double"},        // Double array type
            {"ᚹ", "switch"},        // Switch statement
            {"ᚲ", "case"},          // Case statement
            {"ᛞ", "default"},       // Default case
            {"ᚨ", "else"},          // Else-condition
            {"ᛃ", "="},             // Assignment operator
            {"ᛇ", "!="},            // Not equal operator
            {"ᛋ", "||"},            // Logical OR operator
            {"ᛏ", "&&"},            // Logical AND operator
            {"ᛒ", "{"},             // Start of block
            {"ᛘ", "}"},             // End of block
            {"ᛚ", "int"},           // Integer type
            {"ᛦ", "float"},         // Float type
            {"ᛙ", "char"},          // Char type
            {"ᛠ", "double"},        // Double type
            {"ᛡ", "bool"},          // Boolean type
            {"ᛤ", "void"},          // Void return type
            {"ᛥ", "class"},         // Class declaration
            {"ᛦ", "namespace"},     // Namespace declaration
            {"ᛧ", "struct"}         // Struct declaration
        };
    }
};

} // namespace RuneLang
