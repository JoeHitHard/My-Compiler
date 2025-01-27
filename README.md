
# My-Compiler 🛠️: A Custom Compiler & Interpreter Suite

**Dive into the heart of programming languages!** This project is a fully functional compiler and interpreter for a custom-designed programming language, built from scratch in C. It demonstrates core concepts of compiler design, including lexical analysis, intermediate code generation, and execution—perfect for enthusiasts and learners alike!

## 🚀 Features

- **Lexical Analysis**: Tokenizes source code into keywords, identifiers, and operators.
- **Intermediate Code Generation**: Translates source code into Intermediate Machine Language (IML).
- **Symbol & Constant Tables**: Efficiently manages variables and constants.
- **Control Flow**: Supports `if-else-endif` blocks, jumps, and labels.
- **Arithmetic & I/O Operations**: Handle `add`, `mul`, `read`, `write`, and more.
- **Memory Simulation**: Uses a virtual memory pool for registers, variables, and constants.
- **Extensible Architecture**: Easy to add new operations or optimize existing components.

## 💡 Why This Project?

- **Learn Compiler Design**: A hands-on implementation of lexical analysis, parsing, and code generation.
- **C Mastery**: Leverages low-level C features for memory management and data structures.
- **Real-World Skills**: Demonstrates symbol tables, intermediate code, and execution environments.
- **Perfect for Recruiters**: Shows deep understanding of systems programming and problem-solving.

---

## ⚙️ Technologies Used

- **C Programming**: Core implementation language for performance and control.
- **Dynamic Data Structures**: Custom arrays, stacks, and tables for symbol management.
- **Memory Management**: Manual allocation/deallocation for efficiency.
- **File I/O**: Serializes/deserializes tables and IML instructions.

---

## 🛠️ Getting Started

### Prerequisites
- GCC compiler
- Basic C knowledge (recommended)

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/JoeHitHard/My-Compiler.git
   cd My-Compiler
2. Compile the project:
   ```bash
   gcc -o mycompiler main.c
   ```

---

## 🎮 Usage

### Step 1: Write Your Program
Create a `.txt` file (e.g., `factorial.txt`) with your custom code:
```plaintext
data a
data i
data f
const c=1
mov i,c
mov f,c
read a
add a,a,c
x:
mul f,f,i
add i,i,c
if a gt i then
jump x
endif
write f
```

### Step 2: Compile & Execute
```bash
./mycompiler factorial.txt
```
The interpreter will:
1. Generate IML instructions.
2. Store symbol tables in `SymbolTable.st`.
3. Execute the code and print the result!

---

## 🌟 Sample Use Case: Factorial Calculation

**Objective**: Compute `f = a!` using a loop with `if` and `jump`.

1. **Input**: User provides `a` (e.g., `5`).
2. **Execution**:
   - Loop multiplies `f` by `i` (from 1 to `a`).
   - `if` checks if `i < a` and jumps back.
3. **Output**: `120` (if `a=5`).

**Why It’s Cool**: Demonstrates loops via conditional jumps and label handling—all without native loop constructs!

---

## 🧠 Behind the Scenes

### Key Components
1. **Symbol Table**: Tracks variables (`data a`) and their memory offsets.
2. **Constant Table**: Stores constants (`const c=1`).
3. **Block Address Table**: Maps labels (`x:`) to IML instruction numbers.
4. **Intermediate Code (IML)**: 
   - Example: `ADD a, a, c` becomes `OpCode 5` with parameter nodes.

### Execution Flow
1. **Lexer** → Tokenizes source code.
2. **Parser** → Generates IML instructions.
3. **Interpreter** → Executes IML using simulated memory (MP).

---

## 🚧 Roadmap

- **Planned Features**:
  - Error handling and diagnostics.
  - Support for floating-point arithmetic.
  - Optimizations in IML generation.
---

**Crafted with ❤️ by JoeHitHard**. Let’s connect on [LinkedIn]([https://linkedin.com/in/yourprofile](https://www.linkedin.com/in/joseph-meghanath-9880ba149/))!
