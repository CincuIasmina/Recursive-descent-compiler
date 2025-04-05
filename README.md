### Lexical analysis (ALEX)
  - ALEX is a lexical analyzer that traverses the source code and transforms it into a list of lexical atoms (tokens), essential for the next stages of the compiler.
  - In terms of functionality, it deals with the elimination of spaces, comments and empty lines. It identifies atoms such as: identifiers (ID), numeric constants (INT, REAL), strings (STR), keywords (IF, WHILE, etc.). It correctly handles special characters and errors. It uses a Token structure to store relevant information (type, value, line).
  - Key functions: tokenize() –> iterates through the code and builds the list of tokens, addTk() –> adds a new token and copyn() –> extracts the text between two positions.

### Parsing
  - Implement a parser for the Quick language, which transforms a sequence of tokens into a valid syntactic structure according to the language's grammar.
  - **Method:** We use recursive functions for each syntactic rule (RS). Each rule is implemented in a function (FIRS) that:
      - **Returns true** if the rule was satisfied (all tokens in the rule are consumed).
      - **Returns false** if it was not satisfied and does not consume any tokens.
  - **Grammar Components**
      - **Lexical Atoms:** Consume via the consume() function, which checks whether the current token matches the expected one.
      - **Syntactic Rules:** FIRS functions for each syntactic rule, which are called to validate the structure.
      - **Sequences:** The components of a sequence are checked successively using nested ifs.
      - **Alternatives:** We use successive ifs to test alternative branches of a rule.
      - **Optional Repetition:** Implemented using loops, in which we test repeated conditions and exit when they are no longer met.
      - **Optionality and Mandatory Repetition:** Implemented using alternatives and sequences.
  - **Error Handling Syntax Error:** Issuing an error message when a rule cannot be satisfied.
  - **Error Messages:** The text is descriptive and easy for the programmer to understand, avoiding overly technical terms.
  - **Debug Help:** Using debug displays, such as displaying the current rule name or the status of an atom's consumption, to track the parsing steps.

### Domain analysis
  - **Domain analysis** is a technique used in compilers to manage declarations of symbols (variables, functions, arguments, data types) and organize them into a data structure called a Symbol Table (TS). The TS is implemented as a stack of domains, with each domain representing a section of the program with shared visibility, such as the global domain or local function domains.
  - In the Quick language, domain analysis occurs in parallel with syntactic analysis. The code for domain analysis is embedded in the syntactic analysis and is handled by functions and variables declared in the "ad.h" and "ad.c" files.
  - At the beginning of the program, the global scope is created, and as the functions are analyzed, local scopes are created. When the analysis of a function is finished, its scope is removed.
  - During compilation, appropriate messages must be displayed, and errors (for example, variables or arguments with the same name) must be handled and reported correctly.
  - Correct implementation involves integrating the code from the "ad.h" and "ad.c".

### Type analysis
  - **Type analysis (TA)** is a technique used in a compiler to check the correctness of the data types used in a program, ensuring that symbols, constants, and instructions are used according to the rules of the language. For example, a type error can occur when attempting to call a variable as a function.
  - **Type analysis** is performed in parallel with syntactic analysis, and code fragments that implement type analysis are inserted into certain syntactic rules.
  - The code sequences for type analysis use functions and data types that are declared in the "at.h" and "at.c" files.
