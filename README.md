# kcc
A small C Compiler (in progress) </br>
This aims to be a full C11 compilant optimizing compiler.

Stages: </br>
1.  Lexer (almost done, without preprocessor)
2.  Parser (supports basic syntax)
3.  Semantic Analysis (here we are)
4.  Three-Address form IR Generation
5.  SSA-based IR Generation
6.  Optimizations
7.  Graph-Coloring Register Allocation
8.  x64 Assembly Generation

# More info about this project:
What's the purpose of this project? </br>
In brief, to learn optimizing techniques as well as the C11 language.

Why using C++ instead of C? </br>
Because this is an optimizing compiler, which focus on algorithm rather that the ability of bootstrapping. Another reason is that C++ makes it easier to organize large amount of code, which is crucial, considering this is a one-man project. I don't have much time working on this project so i don't want to waste my time on segfaults.
