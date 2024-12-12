#include "../include/RuneParser.hpp"
#include <iostream>

int main() {
    RuneLang::RuneParser parser;

    // Example 1: Custom type definition
    std::string runeCode1 = "ᛚ MyType";
    std::cout << "Example 1 - Custom Type Definition:\n";
    std::cout << "Rune code: " << runeCode1 << "\n";
    std::cout << "C++ code: " << parser.compileToCpp(runeCode1) << "\n\n";

    // Example 2: Nested function
    std::string runeCode2 = "ᛤ myFunction ᛒ ᚠ \"Hello from myFunction!\\n\" ᛏ 0 ᛘ";
    std::cout << "Example 2 - Nested Function Definition:\n";
    std::cout << "Rune code: " << runeCode2 << "\n";
    std::cout << "C++ code: " << parser.compileToCpp(runeCode2) << "\n\n";

    // Example 3: For loop
    std::string runeCode3 = "ᚱ i 0 i < 5 ᚦ ᚢ 1 ᚠ i";
    std::cout << "Example 3 - For Loop:\n";
    std::cout << "Rune code: " << runeCode3 << "\n";
    std::cout << "C++ code: " << parser.compileToCpp(runeCode3) << "\n\n";

    // Example 4: Simple print statement
    std::string runeCode4 = "ᚠ \"Hello, World!\\n\"";
    std::cout << "Example 4 - Print statement:\n";
    std::cout << "Rune code: " << runeCode4 << "\n";
    std::cout << "C++ code: " << parser.parseRuneCode(runeCode4) << "\n\n";

    // Example 5: Function definition
    std::string runeCode5 = "ᛤ main ᛒ ᚠ \"Hello from function!\\n\" ᛏ 0 ᛘ";
    std::cout << "Example 5 - Function definition:\n";
    std::cout << "Rune code: " << runeCode5 << "\n";
    std::cout << "C++ code: " << parser.parseRuneCode(runeCode5) << "\n\n";

    return 0;
}
