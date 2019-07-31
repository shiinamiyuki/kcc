/*
 * lex.cc
 *
 *  Created on: 2018/7/6
 *      Author: xiaoc
 */


#include "lex.h"
#include "fmt/format.h"
using namespace kcc;
Token::Token(Type t, const std::string to, int l, int c) {
    type = t;
    line = l;
    col = c;
    tok = to;
    if (to.empty()) {
        throw std::runtime_error("token is empty");
    }
}

char Lexer::at(int idx) {
    if (idx >= source.size())
        return 0;
    else
        return source.at(idx);
}

void Lexer::consume() {
    //putchar(cur());
    if (cur() == '\n') {
        line++;
        col = 1;
    } else {
        if (cur() == '\t')
            col += 4;
        else
            col++;
    }
    pos++;
    if (pos > source.length() + 10)
        throw std::runtime_error("parser boom!");
}

char Lexer::cur() {
    return at(pos);
}

char Lexer::peek() {
    return at(pos + 1);
}

char Lexer::peek2() {
    return at(pos + 2);
}

bool isSpace(char c) {
    return c == ' ' || c == '\r' || c == '\t' || c == '\n';
}

void Lexer::skipspace() {
    skipcomment();
    while (isSpace(cur())) {
        consume();
        skipcomment();
    }
}

void Lexer::skipcomment() {
    int i = isComment();
    if (!i)
        return;
    if (i == 1 || i == 2) {
        while (cur() != '\n')
            consume();
    } else if (i == 3) {
        while (!(cur() == '*' && peek() == '/'))
            consume();
        consume();
        consume();
    }
}

int Lexer::isComment() {
    if (cur() == '#') // skip preprocessor instructions
        return 1;
    if (cur() == '/' && peek() == '/') {
        return 2;
    }
    if (cur() == '/' && peek() == '*') {
        return 3;
    }
    return 0;
}

Lexer::Lexer(const char * _filename,const std::string &s) {
    filename = _filename;
    pos = 0;
    source = s;
    line = 1;
    col = 1;

}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isIden(char c) {
    return c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static std::set<std::string> keywords =
        {"auto", "break", "case", "char",
         "const", "continue", "default", "do", "double", "else", "enum",
         "extern", "float", "for", "goto", "if", "inline", "int", "long",
         "register", "restrict", "return", "short", "signed", "short", "signed",
         "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
         "void", "volatile", "while", "_Alignas", "_Alignof", "_Atomic", "_Bool",
         "_Complex", "_Generic", "_Imaginary", "_Noreturn", "_Static_assert",
         "_Thread_local"};
static std::vector<std::set<std::string>> operators =
        {{"&&=", "||=", ">>=", "<<="},
         {"&&","||","++","--","+=",  "-=",  "*=",  "/=", "%=", "|=", "&=", "^=", ">=", "<=", "!=", "==", "->", ">>", "<<"},
         {"+",   "-",   "*",   "/",  "%",  "&",  "|",  "^",  "(",  ")",  "[",  "]",  "{",  "}",  ",", "=", "\\", "<", ">", ".", ":",
                 "?", "~", "!"}};
static std::set<char> opChar =
        {'+', '-', '*', '/', '%', '^', '>', '<', '!',
         '=', '(', ')', '[', ']', '{', '}', '.', ':', ',', '\\', '&', '|', '~',
         '?'};

Token Lexer::next() {
    if (cur() == ';') {
        consume();
        return makeToken(Token::Type::Terminator, ";", line, col);
    } else if (isdigit(cur())) { // numbers
        return number();
    } else if (isIden(cur())) {
        return identifier();
    } else if (opChar.find(cur()) != opChar.end()) {
        return punctuator();
    } else if (cur() == '\"' || cur() == '\'') {
        return string();
    }
    throw std::runtime_error(std::string("unable to parse ") + cur());
}

void Lexer::scan() {
    try {
        skipspace();
        while (pos < source.length()) {
            auto tok = next();
            if(tokenStream.size() > 0) {
                auto last = tokenStream.back();
                if (last.type == tok.type && tok.type == Token::Type::String) {
                    auto s = last.tok;
                    s.pop_back();
                    auto s2 = tok.tok;
                    for (auto iter = s2.begin() + 1; iter != s2.end(); iter++) {
                        s += *iter;
                    }
                    last.tok = s;
                    tokenStream.pop_back();
                    tokenStream.push_back(last);
                    skipspace();
                    continue;
                }
            }
             tokenStream.push_back(tok);
            skipspace();
        }
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
}

bool Lexer::isKeyWord(const std::string &s) {
    return keywords.find(s) != keywords.end();
}

Token Lexer::identifier() {
    std::string iden;
    while (isIden(cur()) || isdigit(cur())) {
        iden += cur();
        consume();
    }
    if (isKeyWord(iden))
        return makeToken(Token::Type::Keyword, iden, line, col);
    else
        return makeToken(Token::Type::Identifier, iden, line, col);
}

Token Lexer::number() {
    std::string number;
    Token::Type ty =  Token::Type::Int;
    if (cur() == '0' && peek() == 'x') {
        number += cur();
        number += peek();
        consume();
        consume();
        while (isdigit(cur()) || (cur() >= 'A' && cur() <= 'F')
               || (cur() >= 'a' && cur() <= 'f')) {
            number += cur();
            consume();
        }
    } else if (cur() == '0' && peek() != 'x' && peek() != '.') {
        while (cur() >= '0' && cur() <= '7') {
            number += cur();
            consume();
        }
    } else {
        while (isdigit(cur())) {
            number += cur();
            consume();
        }
        if (cur() == '.') {
            ty =  Token::Type::Float;
            number += cur();
            consume();
            while (isdigit(cur())) {
                number += cur();
                consume();
            }
        }
        if (cur() == 'e') {
            ty =  Token::Type::Float;
            number += cur();
            if (peek() == '-') {
                consume();
                number += cur();
            }
            consume();
            while (isdigit(cur())) {
                number += cur();
                consume();
            }
        }
        if(cur() == 'f'){
            ty =  Token::Type::Float;
            number += cur();
            consume();
        }
    }
    return makeToken(ty, number, line, col);
}

Token Lexer::punctuator() {
    char p2 = peek2();
    char p = peek();
    std::string s1, s2, s3;
    s1 = cur();
    s2 = s1 + p;
    s3 = s2 + p2;
    //std::cout << s1 << " " << s2 << " " << s3 << " " << std::endl;
    //system("pause");
    if (operators[0].find(s3) != operators[0].end()) {
        auto t = makeToken(Token::Type::Punctuator, s3, line, col);
        consume();
        consume();
        consume();
        return t;
    } else if (operators[1].find(s2) != operators[1].end()) {
        auto t = makeToken(Token::Type::Punctuator, s2, line, col);
        consume();
        consume();
        return t;
    } else if (operators[2].find(s1) != operators[2].end()) {
        auto t = makeToken(Token::Type::Punctuator, s1, line, col);
        consume();
        return t;
    } else {
        throw std::runtime_error(std::string("unable to parse ") + cur());
    }
}

Token Lexer::string() {
    std::string s;
  //  s += cur();
    char c = cur();
    consume();
    while (cur() != c) {
        if (cur() == '\\') {
            consume();
            if (cur() == '\\') {
                s += "\\\\";
            } else if (cur() == 'n') {
                s += "\\n";
            } else if (cur() == '"') {
                s += "\\\"";
            } else if (cur() == '\'') {
                s += "\\\'";
            }
        } else
            s += cur();
        consume();
    }
    consume();
   // s += c;
    if(s[0] == '\'' ){
        if(s.length()!=3)
            throw std::runtime_error(std::string("char literal to long"));
        return makeToken(Token::Type::Int,fmt::format("{}",(int)s[1]),line, col);
    }

    return makeToken(Token::Type::String, s, line, col);
}

std::vector<Token> &Lexer::getTokenStream() {
    return tokenStream;
}
