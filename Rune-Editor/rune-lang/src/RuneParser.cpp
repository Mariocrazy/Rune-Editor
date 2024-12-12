#include "../include/RuneParser.hpp"
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <list>
#include <set>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace RuneLang {

struct Color {
    int r, g, b; // RGB values
};

Color keywordColor = {255, 0, 0}; // Red for keywords
Color stringColor = {0, 255, 0}; // Green for strings
Color commentColor = {128, 128, 128}; // Gray for comments

std::map<std::string, Color> syntaxColors = {
    {"keyword", keywordColor},
    {"string", stringColor},
    {"comment", commentColor}
};

std::vector<std::string> keywords = {
    "if",
    "else",
    "for",
    "while",
    "return",
    "int",
    "float",
    "double",
    "char",
    "void",
    "class",
    "namespace",
    "struct",
    // Add more keywords as needed
};

struct SyntaxError {
    std::string message;
    int lineNumber;
};

std::list<SyntaxError> syntaxErrors;

void checkForErrors(const std::string& code) {
    // Simple error checking logic
    if (code.find(";") == std::string::npos) {
        syntaxErrors.push_back({"Syntax Error: Missing semicolon.", 0});
    }
    if (code.find("{") != std::string::npos && code.find("}") == std::string::npos) {
        syntaxErrors.push_back({"Syntax Error: Unmatched opening brace.", 0});
    }
    // Add more error checks as needed
}

void renderErrors() {
    for (const auto& error : syntaxErrors) {
        std::cout << "Error on line " << error.lineNumber << ": " << error.message << std::endl;
    }
}

RuneParser::RuneParser() {
    initializeRuneMap();
}

void RuneParser::initializeRuneMap() {
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

std::string RuneParser::parseRuneCode(const std::string& runeCode) {
    std::string parsedCode;
    size_t pos = 0;

    try {
        while (pos < runeCode.length()) {
            std::string runeStr(1, runeCode[pos]);

            if (runeToKeyword.find(runeStr) != runeToKeyword.end()) {
                if (runeStr == "ᛥ") {
                    parsedCode += handleClass(runeCode, pos);
                } else if (runeStr == "ᛤ" || runeStr == "ᛚ" || runeStr == "ᛦ" || 
                         runeStr == "ᛙ" || runeStr == "ᛠ" || runeStr == "ᛡ") {
                    parsedCode += handleFunction(runeCode, pos);
                } else if (runeStr == "ᛚ") {
                    parsedCode += handleCustomType(runeCode, pos);
                    pos++; // increment pos after handling custom type
                } else if (runeStr == "ᚹ") {
                    parsedCode += handleSwitch(runeCode, pos);
                } else {
                    parsedCode += runeToKeyword[runeStr] + " ";
                    pos++;
                }
            } else if (std::isspace(runeCode[pos])) {
                parsedCode += runeCode[pos];
                pos++;
            } else {
                parsedCode += runeCode[pos];
                pos++;
            }
        }
    } catch (const RuneParseError& e) {
        std::cerr << "Parse Error: " << e.what() << std::endl;
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error while parsing: " << e.what() << std::endl;
        throw RuneParseError("Unexpected error while parsing: " + std::string(e.what()));
    }

    return parsedCode;
}

std::string RuneParser::handleFunction(const std::string& code, size_t& pos) {
    std::string returnType = runeToKeyword[std::string(1, code[pos])];
    pos++;

    // Skip whitespace
    while (pos < code.length() && std::isspace(code[pos])) pos++;

    // Get function name
    std::string functionName;
    while (pos < code.length() && !std::isspace(code[pos]) && code[pos] != '(') {
        functionName += code[pos++];
    }

    if (functionName.empty()) {
        throw RuneParseError("Expected function name");
    }

    return returnType + " " + functionName + "() {\n";
}

std::string RuneParser::handleClass(const std::string& code, size_t& pos) {
    pos++; // Skip class rune

    // Skip whitespace
    while (pos < code.length() && std::isspace(code[pos])) pos++;

    // Get class name
    std::string className;
    while (pos < code.length() && !std::isspace(code[pos]) && code[pos] != '{') {
        className += code[pos++];
    }

    if (className.empty()) {
        throw RuneParseError("Expected class name");
    }

    return "class " + className + " {\n";
}

std::string RuneParser::handleCustomType(const std::string& code, size_t& pos) {
    // Logic to handle custom type definitions
    // Example: ᛚ MyType
    std::string typeName;
    while (pos < code.length() && !std::isspace(code[pos]) && code[pos] != '{') {
        typeName += code[pos++];
    }
    return "typedef int " + typeName + ";";
}

std::string RuneParser::handleSwitch(const std::string& code, size_t& pos) {
    std::string switchCode;
    pos++; // Move past the switch rune

    // Skip whitespace
    while (pos < code.length() && std::isspace(code[pos])) pos++;

    // Get the switch variable
    std::string switchVariable;
    while (pos < code.length() && !std::isspace(code[pos]) && code[pos] != '{') {
        switchVariable += code[pos++];
    }

    switchCode += "switch (" + switchVariable + ") {\n";

    // Process cases
    while (pos < code.length()) {
        std::string caseRune(1, code[pos]);
        if (caseRune == "ᚲ") { // If it's a case rune
            pos++; // Move past the case rune
            std::string caseValue;
            while (pos < code.length() && !std::isspace(code[pos]) && code[pos] != '}') {
                caseValue += code[pos++];
            }
            switchCode += "case " + caseValue + ": \n";
        } else if (caseRune == "ᛞ") { // If it's a default case
            pos++; // Move past the default rune
            switchCode += "default: \n";
        } else if (caseRune == "}") { // End of switch
            switchCode += "}\n";
            break;
        } else {
            // Handle unexpected characters
            throw RuneParseError("Unexpected character in switch statement");
        }
    }

    return switchCode;
}

struct ColorSettings {
    std::string keywordColor;
    std::string stringColor;
    std::string commentColor;
};

ColorSettings loadColorSettings() {
    // Load color settings from a configuration file (e.g., colors.json)
    // For now, return default colors
    return {"#0000FF", "#008000", "#808080"}; // Blue, Green, Gray
}

void applySyntaxHighlighting(const std::string& code) {
    ColorSettings colors = loadColorSettings();
    std::istringstream codeStream(code);
    std::string line;

    while (std::getline(codeStream, line)) {
        // Example highlighting logic
        if (line.find("//") != std::string::npos) {
            // Highlight comments
            std::cout << "\033[38;5;245m" << line << "\033[0m" << std::endl; // Gray
        } else if (line.find("int") != std::string::npos) {
            // Highlight keywords
            std::cout << "\033[38;5;34m" << line << "\033[0m" << std::endl; // Green
        } else {
            // Regular line
            std::cout << line << std::endl;
        }
    }
}

void loadKeybindings() {
    std::ifstream file("keybindings.json");
    json keybindings;
    file >> keybindings;

    // Example of applying keybindings
    std::string setBreakpointKey = keybindings["setBreakpoint"].get<std::string>();
    // Implement logic to bind this key to the setBreakpoint function
    // ... (binding logic)
}

std::set<int> breakpoints;

void setBreakpoint(int lineNumber) {
    breakpoints.insert(lineNumber);
}

void removeBreakpoint(int lineNumber) {
    breakpoints.erase(lineNumber);
}

bool isBreakpoint(int lineNumber) {
    return breakpoints.find(lineNumber) != breakpoints.end();
}

void executeCode(const std::string& code) {
    std::istringstream codeStream(code);
    std::string line;
    int lineNumber = 0;

    while (std::getline(codeStream, line)) {
        lineNumber++;
        if (isBreakpoint(lineNumber)) {
            std::cout << "Breakpoint hit at line " << lineNumber << std::endl;
            // Implement logic to pause execution and allow variable inspection
            // For now, we will just wait for user input
            std::cin.get();
        }
        // Execute the line of code
        // ... (execution logic)
    }
}

void showSuggestions(const std::string& input) {
    std::vector<std::string> suggestions;
    for (const auto& keyword : keywords) {
        if (keyword.find(input) == 0) { // Match from the start
            suggestions.push_back(keyword);
        }
    }

    // Display suggestions
    if (!suggestions.empty()) {
        std::cout << "Suggestions:" << std::endl;
        for (const auto& suggestion : suggestions) {
            std::cout << "- " << suggestion << std::endl;
        }
    } else {
        std::cout << "No suggestions found." << std::endl;
    }
}

void processInput(const std::string& input) {
    // Call showSuggestions with the current input
    showSuggestions(input);
}

std::string RuneParser::compileToCpp(const std::string& runeCode) {
    checkForErrors(runeCode); // Check for errors before compiling
    std::string cppCode = parseRuneCode(runeCode);
    std::ofstream outputFile("output.cpp");
    outputFile << "#include <iostream>\n\n" << cppCode << "\nint main() {\n\treturn 0;\n}";
    outputFile.close();
    return cppCode;
}

} // namespace RuneLang
