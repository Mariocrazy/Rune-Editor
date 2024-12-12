#include "../include/RuneParser.hpp"
#include <sstream>
#include <fstream>

namespace RuneLang {

RuneParser::RuneParser() : currentLine(1), currentColumn(1) {
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
        {"ᛜ", "std::cin >> "},  // Input from console
        {"ᛝ", "std::string"},   // String type
        {"ᛞ", "#"},             // Comment marker
        {"ᛟ", "\""},            // String delimiter
        {"ᛗ", "->"},            // Arrow operator
        {"ᚹ", "*"},             // Multiplication
        {"ᚺ", "/"},             // Division
        {"ᚻ", "%"},             // Modulo
        {"ᚼ", "<<"},            // Left shift
        {"ᚽ", ">>"},            // Right shift
        {"ᚾ", "&"},             // Bitwise AND
        {"ᚿ", "|"},             // Bitwise OR
        {"ᛀ", "^"},             // Bitwise XOR
        {"ᛁ", "~"},             // Bitwise NOT
        {"ᛚ", "float"},         // Float array type
        {"ᛦ", "double"},        // Double array type
        {"ᛙ", "char"},          // Char type
        {"ᛠ", "double"},        // Double type
        {"ᛡ", "bool"},          // Boolean type
        {"ᛤ", "void"},          // Void return type
        {"ᛥ", "class"},         // Class declaration
        {"ᛦ", "namespace"},     // Namespace declaration
        {"ᛧ", "struct"}         // Struct declaration
    };
}

void RuneParser::updatePosition(const std::string& code, size_t& pos) {
    if (code[pos] == '\n') {
        currentLine++;
        currentColumn = 1;
    } else {
        currentColumn++;
    }
    pos++;
}

void RuneParser::skipWhitespace(const std::string& code, size_t& pos) {
    while (pos < code.length() && std::isspace(code[pos])) {
        updatePosition(code, pos);
    }
}

std::string RuneParser::handleString(const std::string& code, size_t& pos) {
    std::string result = "\"";
    pos++; // Skip opening quote rune
    
    while (pos < code.length()) {
        if (std::string(1, code[pos]) == "ᛟ") { // Check for closing quote
            pos++;
            break;
        }
        result += code[pos];
        updatePosition(code, pos);
    }
    
    return result + "\"";
}

std::string RuneParser::handleComment(const std::string& code, size_t& pos) {
    std::string result = "//";
    pos++; // Skip comment rune
    
    while (pos < code.length() && code[pos] != '\n') {
        result += code[pos];
        pos++;
    }
    
    return result + "\n";
}

std::string RuneParser::parseRuneCode(const std::string& runeCode) {
    std::string parsedCode;
    size_t pos = 0;
    
    try {
        while (pos < runeCode.length()) {
            skipWhitespace(runeCode, pos);
            if (pos >= runeCode.length()) break;
            
            std::string runeStr(1, runeCode[pos]);
            
            if (runeStr == "ᛟ") { // String
                parsedCode += handleString(runeCode, pos);
            } else if (runeStr == "ᛞ") { // Comment
                parsedCode += handleComment(runeCode, pos);
            } else if (runeToKeyword.find(runeStr) != runeToKeyword.end()) {
                parsedCode += runeToKeyword[runeStr];
                updatePosition(runeCode, pos);
            } else {
                throw RuneParseError("Unknown rune symbol: " + runeStr, currentLine, currentColumn);
            }
        }
    } catch (const std::exception& e) {
        throw RuneParseError(e.what(), currentLine, currentColumn);
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

std::string RuneParser::compileToCpp(const std::string& runeCode) {
    std::string cppCode = parseRuneCode(runeCode);
    std::ofstream outputFile("output.cpp");
    outputFile << "#include <iostream>\n\n" << cppCode << "\nint main() {\n\treturn 0;\n}";
    outputFile.close();
    return cppCode;
}

} // namespace RuneLang
