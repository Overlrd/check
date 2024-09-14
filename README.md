# C Syntax Checker

## Description

This project implements a C program that checks for rudimentary syntax errors in C source code. It's based on Exercise 1-24 from the book "The C Programming Language" by Brian Kernighan and Dennis Ritchie (K&R).

The syntax checker looks for the following errors:

- Unmatched parentheses `()`
- Unmatched brackets `[]`
- Unmatched braces `{}`
- Unmatched quotes (both single `'` and double `"`)
- Unclosed comments

## Features

- Handles both single-line (`//`) and multi-line (`/* */`) comments
- Recognizes string literals and character constants
- Accounts for escape sequences within strings
- Provides line and column information for detected errors

## How to Use

1. Compile the program:
   ```
   make
   ```

2. Run the program with a C source file as an argument:
   ```
   ./check path/to/your/source_file.c
   ```

3. The program will output any syntax errors it finds, along with their locations in the file.

## Limitations

This is a rudimentary syntax checker and does not perform full C parsing or semantic analysis. It may not catch all possible syntax errors and could potentially report false positives in complex cases.
