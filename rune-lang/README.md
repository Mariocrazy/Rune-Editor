# RuneLang - Elder Futhark Programming Language

RuneLang is a programming language that uses Elder Futhark runes as syntax elements. It translates rune-based code into C++ code.

## Rune Mappings

Here are the 24 Elder Futhark runes and their corresponding C++ constructs:

- ᚠ (FEHU) - Output to console (std::cout)
- ᚱ (RAIDHO) - For loop
- ᚷ (GEBO) - If condition
- ᛏ (TIWAZ) - Return statement
- ᚢ (URUZ) - Addition operator
- ᚦ (THURISAZ) - Subtraction operator
- ᛉ (ALGIZ) - While loop
- ᚨ (ANSUZ) - Else condition
- ᛃ (JERA) - Assignment operator
- ᛇ (EIWAZ) - Not equal operator
- ᛋ (SOWILO) - Logical OR
- ᛏ (TIWAZ) - Logical AND
- ᛒ (BERKANAN) - Block start {
- ᛘ (MADR) - Block end }
- ᛚ (LAGUZ) - Integer type
- ᛦ (LONG-BRANCH) - Float type
- ᛙ (NAUDIZ) - Char type
- ᛠ (EOLH) - Double type
- ᛡ (WUNJO) - Boolean type
- ᛤ (FEHU) - Void type
- ᛥ (LAGUZ) - Class declaration
- ᛦ (LONG-BRANCH) - Namespace declaration
- ᛧ (WUNJO) - Struct declaration

## Building the Project

1. Create a build directory:
   ```bash
   mkdir build && cd build
   ```

2. Generate build files:
   ```bash
   cmake ..
   ```

3. Build the project:
   ```bash
   make
   ```

4. Run the example:
   ```bash
   ./rune_lang
   ```

## Examples

1. Hello World:
   ```
   ᚠ "Hello, World!\n"
   ```

2. For Loop:
   ```
   ᚱ i 0 i < 5 ᚦ ᚢ 1 ᚠ i
   ```

3. Function Definition:
   ```
   ᛤ main ᛒ ᚠ "Hello from function!\n" ᛏ 0 ᛘ
   ```

## Features

- Translation of rune-based code to C++
- Support for basic control structures (if, for, while)
- Function definitions
- Class declarations
- Variable declarations and assignments
- Basic error handling

## Contributing

Feel free to contribute to this project by submitting pull requests or reporting issues.
