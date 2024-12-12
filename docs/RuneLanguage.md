# Rune Programming Language Documentation

## Overview
Rune is a systems programming language designed specifically for GhostC OS, using Norse runes as syntax elements. It provides low-level system access while maintaining a clean and intuitive syntax.

## Basic Syntax

### Output and Input
- `ᚠ` - Output to console (std::cout)
- `ᛜ` - Input from console (std::cin)

### Control Flow
- `ᚱ` - For loop
- `ᚷ` - If condition
- `ᛉ` - While loop
- `ᚨ` - Else condition

### Operators
- `ᚢ` - Addition
- `ᚦ` - Subtraction
- `ᚹ` - Multiplication
- `ᚺ` - Division
- `ᚻ` - Modulo
- `ᛃ` - Assignment
- `ᛇ` - Not equal
- `ᛋ` - Logical OR
- `ᛏ` - Logical AND

### Data Types
- `ᛚ` - Integer
- `ᛦ` - Float
- `ᛙ` - Char
- `ᛠ` - Double
- `ᛡ` - Boolean
- `ᛤ` - Void
- `ᛝ` - String

### System Operations

#### Process Management
- `ᛨ` - System operations prefix
- `ᛩ` - Process operations prefix
- `ᛪ` - Thread operations prefix
- `ᛯ` - Create process
- `ᛰ` - Create thread
- `ᛱ` - Terminate process/thread

#### Memory Management
- `᛬` - Allocate memory
- `ᛮ` - Free memory

#### File System Operations
- `᛫` - File system operations prefix
- `ᛲ` - Create file
- `ᛳ` - Delete file
- `ᛴ` - Read file
- `ᛵ` - Write file

## GhostC OS Specific Features

### System Information
```rune
ᛨgetOSVersion()      // Get GhostC OS version
ᛨgetHostname()       // Get system hostname
ᛨgetCPUCount()       // Get number of CPU cores
ᛨgetTotalMemory()    // Get total system memory
ᛨgetAvailableMemory() // Get available memory
```

### Process Management
```rune
ᛯmyProcess           // Create new process
ᛰmyThread            // Create new thread
ᛱmyProcess           // Terminate process
```

### File System Operations
```rune
ᛲ"config.txt"        // Create file
ᛴ"config.txt"        // Read file
ᛵ"config.txt"        // Write to file
ᛳ"config.txt"        // Delete file
```

## Example Programs

### 1. System Information Display
```rune
ᚠ"GhostC OS System Information\n"
ᚠ"OS Version: " ᛨgetOSVersion() "\n"
ᚠ"Hostname: " ᛨgetHostname() "\n"
ᚠ"CPU Cores: " ᛨgetCPUCount() "\n"
ᚠ"Total Memory: " ᛨgetTotalMemory() "\n"
ᚠ"Available Memory: " ᛨgetAvailableMemory() "\n"
```

### 2. File Operations
```rune
ᛲ"test.txt"         // Create file
ᛵ"test.txt" "Hello, GhostC OS!"  // Write to file
ᛴ"test.txt"         // Read file
ᛳ"test.txt"         // Delete file
```

### 3. Process Management
```rune
ᛯmyProcess          // Create process
ᚷ(ᛩisRunning()) {   // If process is running
    ᚠ"Process is running\n"
    ᛱmyProcess      // Terminate process
}
```

## Error Handling
The Rune language provides detailed error messages with line and column information:
```rune
ᚷ(ᛨsystemCall()) {
    ᚠ"Success\n"
} ᚨ {
    ᚠ"Error at line " getCurrentLine() ", column " getCurrentColumn() "\n"
}
```

## Best Practices
1. Always check return values from system calls
2. Free allocated memory when no longer needed
3. Close files after use
4. Handle process and thread termination properly
5. Use comments to document code functionality
