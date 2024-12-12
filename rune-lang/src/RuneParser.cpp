#include "../include/RuneParser.hpp"
#include <sstream>
#include <fstream>

namespace RuneLang {

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

std::string RuneParser::compileToCpp(const std::string& runeCode) {
    std::string cppCode = parseRuneCode(runeCode);
    std::ofstream outputFile("output.cpp");
    outputFile << "#include <iostream>\n\n" << cppCode << "\nint main() {\n\treturn 0;\n}";
    outputFile.close();
    return cppCode;
}

} // namespace RuneLang
