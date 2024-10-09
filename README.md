# Assembler-project-C-
Final project for the System Programming course, involving the development of an assembler that translates assembly code to machine language. Implemented in C on a Linux (Ubuntu) environment.

**By Tamar Biton and Shira Kedmi**

## Overview

**Imaginary Programming Language Compiler**  
The assembler generates a machine code file based on an input file containing an assembly-language program, which will be defined below.

## Features

- Converts assembly language into machine-readable machine code.
- Handles various sentence types for comprehensive program structure.
- Easy to use with a clear input/output system.

## Usage

The assembler reads assembly files and converts them to machine code, producing an output file that can be executed.

## Types of Sentences

### 1. Empty Sentence
An empty sentence, only '/t', ' ', '\n' and spaces allowed, used for formatting purposes.

### 2. Comment Sentence
Comment sentences are used to annotate code and are ignored by the assembler.

Example:

```assembly
; This is a comment
```

### 3. Instruction Sentence
An instruction sentence consists of assembly commands that are converted into machine code.

**The 15 instruction:**

| Word | Instruction | Operands Required |
|------|-------------|-------------------|
| 0    | mov         | 2                 |
| 1    | cmp         | 2                 |
| 2    | add         | 2                 |
| 3    | sub         | 2                 |
| 4    | not         | 2                 |
| 5    | clr         | 2                 |
| 6    | lea         | 2                 |
| 7    | inc         | 1                 |
| 8    | dec         | 1                 |
| 9    | jmp         | 1                 |
| 10   | bne         | 1                 |
| 11   | red         | 1                 |
| 12   | prn         | 1                 |
| 13   | jsr         | 1                 |
| 14   | rts         | 0                 |
| 15   | stop        | 0                 |


### 4. Directive Sentence
Directive sentences guide the assembler to define data or provide instructions on how to process code.

**The directive sentence has the following structure:**

| Directive   | Description                                      | Example                      |
|-------------|--------------------------------------------------|------------------------------|
| .data       | Allocates space for variable storage              | `.data 1, -1, +1`        |
| .string     | Defines a string and reserves memory accordingly  | `STR: .string "string"`        |
| .entry      | Marks a label to be included in the output file  | `.entry LABEL`               |
| .extern     | Indicates a label defined outside this file       | `.extern LABEL`                 |


## Example
An example of a code in the given languagge:

```assembly
; Sample assembly code for demonstration
MAIN:	add r3, LIST        
LOOP:	prn #48            
	lea STR, r6        
	inc r6 
	mov *r6, K        
	sub r1, r4  
	cmp r3, #-6  
	bne END       
	dec K   
	jmp LOOP      
END:	stop      
STR:	.string "abcd"
LIST:	.data 6, -9     
	.data -100
K:	.data 31   


