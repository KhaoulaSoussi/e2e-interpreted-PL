# e2e-interpreted-PL

## Description
This project is a group project for Languages and Compilers class. The following description is based on the project instructions given by the professor.

This course project consists in constructing, more or less from scratch (but with the potential help of some tools), an end-to-end interpreted programming language environment. We designed a simple “high-level” Programming Language, scanned it, parsed it, and interpreted it by translating to a simple virtual machine. 
We also built some of the aspects of the runtime environment, particularly those pertaining to subprogram call and return.
The main programming language is C, which we used for the VM and interpreter. We also used Java at the level of the Lexical analyzer (JLex).

## Work Plan
Each phase of the project corresponds to a delivery. We start backwards from the VM, following the order in which material was introduced in the course.
1. For the first delivery, you will design a simple machine language (ML), similar to the pseudo-code language we have examined in Unit 3, and implement its interpreter in C. In addition to the numeric ML will provide a symbolic version of it, as if it were an assembly language.
2. For the second delivery, you will design a simple, relatively high level, PL, describing its lexemes and its grammar using formal methods.
3. The third delivery targets the lexical analyzer for your high-level PL, based on Delivery 2. It will produce the token stream for the next phase.
4. For the fourth delivery, you will build a simple parser, construct a parse tree, and generating the assembly language code, which you will be able to run on your VM after converting to ML.
5. Finally, in the fifth delivery, you will implement runtime structures to execute subprogram call and return.

## Chellenges
When we reached the fourth deliverable, we realized that there were many issues that should be fixed in the previous deliverables (all that was changed is mentioned in a text file in parts 1 to 3). We had to go back to all the deliverables that have been already submitted and had to change many things that made it complicated to reproduce input files and to re-debug the lengthy codes that we developed earlier.

## Acronyms used throughout the project
PL: Programming Language
AL: Assembly Language
ML: Machine Language

N.B.: Please refer to the user manual for a detailed description of how to execute the program


