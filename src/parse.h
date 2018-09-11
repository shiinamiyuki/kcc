/*
 * parse.h
 *
 *  Created on: 2018/7/6
 *      Author: xiaoc
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "format.h"
#include "lex.h"
#include "ast.h"
#include "config.h"
namespace kcc {
    class Parser {
        std::vector<Token> tokenStream;
        std::set<std::string> types;
        std::unordered_map<std::string, int> opPrec;
        std::unordered_map<std::string, int> opAssoc; //1 for left 0 for right
        std::set<std::string> typeSpecifiers;
        std::vector<AST *> declStack;
        std::unordered_map<std::string, int> enums;
        int pos;
        int ternaryPrec;
        ConfigState config;

        template<typename T>
        T *newNode() {
            return new T();
        }

        AST *parseExpr(int);

        BinaryExpression *hackExpr(BinaryExpression *);

        AST *parseUnary();

        AST *parsePostfix();

        AST *parsePrimary();

        AST *parseTernary(AST *cond);

        AST *parseCastExpr();

        AST *parseArgumentExpressionList();

        AST *parseBlock();

        AST *parseIf();

        AST *parseWhile();

        AST *parseStmt();

        AST *parseTypeSpecifier();

        AST *parseParameterType();

        AST *parseDecl();

        AST *parseEnum();

        AST *parseGlobalDefs();

        AST *parseDeclarationSpecifier();

        AST *parseTypeName();

        void parseAbstractDirectDeclarator();

        void parseAbstractDeclarator();

        AST *extractIdentifier(AST *);

        void parseDirectDeclarator();

        void parseDeclarator();

        AST *convertFuncTypetoFuncDef(AST *);

        void parseDirectDeclarator_();

        void parseArrayDeclarator();

        void parseFunctionDeclarator();

        AST *parseFuncDef();

        AST *parseFuncDefArg();

        AST *parseReturn();

        AST *parseFor();

        void expect(const std::string &token);

        bool has(const std::string &token);

        template<typename T, typename... Args>
        T *makeNode(Args... args) {
            auto t = new T(args...);
            t->pos = SourcePos(peek().filename,peek().line, peek().col);
            return t;
        }

    public:
        Parser(Lexer &);

        const Token &at(int idx) const;

        inline const Token &cur() const {
            return at(pos);
        }

        inline const Token &peek() const {
            return at(pos + 1);
        }

        inline void consume() { pos++; }

        inline bool hasNext() const {
            return pos + 1 < (int) tokenStream.size();
        }

        AST* error(const std::string &message);

        AST *parse();
    };

    class ParserException : public std::exception {
        std::string msg;
    public:
        ParserException(const std::string &message) {
            msg = message;
        }

        ParserException(const std::string &message, int line, int col) {
            msg = format("{}:{}:error: {}", line, col, message);
        }

        const char *what() {
            return msg.c_str();
        }
    };
}
#endif /* PARSE_H_ */
