# kcc
A small C compiler (in progress) 
This aims to be a full C11 compilant compiler.

## Stages: 
1.  Lexer (without preprocessor)
2.  Parser (supports basic syntax)
3.  Semantic Analysis
4.  Code Generation (System V ABI compatible)

## More info about this project:
What's the purpose of this project? </br>
~~In brief, to learn optimizing techniques as well as the C11 language.~~  Unfortunately, my time is limited. I might not have time to implement SSA-based optimizations. So now the first goal is to make it C11 compliant.


Why using C++ instead of C? </br>
Because this is an optimizing compiler, which focuses on algorithm rather that the ability of bootstrapping. Another reason is that C++ makes it easier to organize large amount of code, which is crucial, considering this is a one-man project. I don't have much time working on this project so i don't want to waste my time on segfaults.

Which plaform is it targeting? </br>
I am developing using ~~Windows~~WSL, but it's targeting linux.

