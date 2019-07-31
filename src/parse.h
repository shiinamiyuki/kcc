/*
 * parse.h
 *
 *  Created on: 2018/7/6
 *      Author: xiaoc
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "kcc.h"
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
        std::vector<AST::AST *> declStack;
        std::unordered_map<std::string, int> enums;
        int pos;
        int ternaryPrec;
        ConfigState config;

        template<typename T>
        T *newNode() {
            return new T();
        }

		AST::AST *parseExpr(int);

		AST::BinaryExpression *hackExpr(AST::BinaryExpression *);

		AST::AST *parseUnary();

		AST::AST *parsePostfix();

		AST::AST *parsePrimary();

		AST::AST *parseTernary(AST::AST *cond);

		AST::AST *parseCastExpr();

		AST::AST *parseArgumentExpressionList();

		AST::AST *parseBlock();

		AST::AST *parseIf();

		AST::AST *parseWhile();

		AST::AST *parseStmt();

		AST::AST *parseTypeSpecifier();

		AST::AST *parseParameterType();

		AST::AST *parseDecl();

		AST::AST *parseEnum();

		AST::AST *parseGlobalDefs();

		AST::AST *parseDeclarationSpecifier();

		AST::AST *parseTypeName();

        void parseAbstractDirectDeclarator();

        void parseAbstractDeclarator();

		AST::AST *extractIdentifier(AST::AST *);

        void parseDirectDeclarator();

        void parseDeclarator();

		AST::AST *convertFuncTypetoFuncDef(AST::AST *);

        void parseDirectDeclarator_();

        void parseArrayDeclarator();

        void parseFunctionDeclarator();

		AST::AST *parseFuncDef();

		AST::AST *parseFuncDefArg();

		AST::AST *parseReturn();

		AST::AST *parseFor();

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

		AST::AST* error(const std::string &message);

		AST::AST *parse();
    };

    class ParserException : public std::exception {
        std::string msg;
    public:
        ParserException(const std::string &message) {
            msg = message;
        }

        ParserException(const std::string &message, int line, int col) {
            msg = fmt::format("{}:{}:error: {}", line, col, message);
        }

        const char *what() {
            return msg.c_str();
        }
    };
}
#endif /* PARSE_H_ */
