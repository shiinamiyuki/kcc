/*
 * parse.cc
 *
 *  Created on: 2018/7/6
 *      Author: xiaoc
 */


#include "parse.h"
using namespace kcc;
Parser::Parser(Lexer &lex) {
    pos = -1;
    tokenStream = lex.getTokenStream();
    int prec = 0;
    /*
     *   opPrec[","] = prec;
     * prec++;*/
    opPrec["="] = prec;
    opPrec["+="] = prec;
    opPrec["-="] = prec;
    opPrec["*="] = prec;
    opPrec["/="] = prec;
    opPrec[">>="] = prec;
    opPrec["<<="] = prec;
    opPrec["%="] = prec;
    opPrec["|="] = prec;
    opPrec["&="] = prec;
    opPrec["^="] = prec;
    opPrec["&&="] = prec;
    opPrec["||="] = prec;
    prec++;
    opPrec["?"] = ternaryPrec = prec;
    prec++;
    opPrec["||"] = prec;
    prec++;
    opPrec["&&"] = prec;
    prec++;
    opPrec["|"] = prec;
    prec++;
    opPrec["^"] = prec;
    opPrec["&"] = prec;
    prec++;
    opPrec["=="] = prec;
    opPrec["!="] = prec;
    prec++;
    opPrec[">="] = prec;
    opPrec["<="] = prec;
    opPrec[">"] = prec;
    opPrec["<"] = prec;

    prec++;
    opPrec[">>"] = prec;
    opPrec["<<"] = prec;
    prec++;
    opPrec["+"] = prec;
    opPrec["-"] = prec;
    prec++;
    opPrec["*"] = prec;
    opPrec["/"] = prec;
    opPrec["%"] = prec;
    prec++;
    opPrec["."] = prec;
    opPrec["->"] = prec;
    opAssoc = {
            {"+=",  0},
            {"-=",  0},
            {"*=",  0},
            {"/=",  0},
            {">>=", 0},
            {"<<=", 0},
            {"%=",  0},
            {"&=",  0},
            {"|=",  0},
            {":=",  0},
            {"::=", 0},
            {"=",   0},
            {".",   1},
            {"->",  1},
            {"+",   1},
            {"-",   1},
            {"*",   1},
            {"/",   1},
            {"!=",  1},
            {"==",  1},
            {">",   1},
            {">=",  1},
            {"<=",  1},
            {"<",   1},
            {"%",   1},
            {"&&",  1},
            {"&",   1},
            {"||",  1},
            {"|",   1}
    };
    types = {
            "int",
            "void",
            "float",
            "double",
            "char",
            "long",
    };
}

const Token &Parser::at(int idx) const {
    static Token nil = Token();
    if (idx >= this->tokenStream.size() || idx < 0) {
        return nil;
    } else {
        return tokenStream[idx];
    }
}

AST *Parser::parse() {
    auto root = new TopLevel();
    try {
        while (hasNext()) {
            root->add(parseGlobalDefs());
        }
    } catch (ParserException &e) {
        std::cerr << e.what() << std::endl;
    }
    return root;
}

AST *Parser::parseExpr(int lev) {
    AST *result = parseCastExpr();
    while (hasNext()) {
        auto next = peek();
        if (opPrec.find(next.tok) == opPrec.end())
            break;
        if (opPrec[next.tok] >= lev) {
            if (next.tok == "?") {
                result = parseTernary(result);
            } else {
                consume();
                AST *rhs = parseExpr(opAssoc[next.tok] + opPrec[next.tok]);
                BinaryExpression *op = makeNode<BinaryExpression>(next);
                op->add(result);
                op->add(rhs);
                result = hackExpr(op);
            }
        } else
            break;
    }
    return result;
}

AST *Parser::parseUnary() {
    if (has("+") || has("-") || has("++") || has("--")
        || has("*")
        || has("&")
        || has("!")) {
        consume();
        auto expr = makeNode<UnaryExpression>(cur());
        expr->add(parseCastExpr());
        return expr;
    } else if (has("sizeof")) {
        consume();
        auto expr = makeNode<UnaryExpression>(cur());
        expect("(");
        expr->add(parseTypeName());
        expect(")");
        return expr;
    }
    return parsePostfix();
}

AST *Parser::parsePostfix() {
    auto postfix = parsePrimary();
    static std::set<std::string> postfixOperator = {
            "++", "--", "(", "[",//".","->"
    };
    while (hasNext() && postfixOperator.find(peek().tok) != postfixOperator.end()) {
        if (has("[")) {
            consume();/*
            auto index = new IndexExpression();
            index->add(postfix);
            index->add(parseExpr(0));*/
            auto add = makeNode<BinaryExpression>(Token(Token::Type::Punctuator,
                                                        "+", -1, -1));
            add->add(postfix);
            add->add(parseExpr(0));
            auto index = makeNode<UnaryExpression>(Token(Token::Type::Punctuator,
                                                         "*", -1, -1));
            index->add(add);
            postfix = index;
            expect("]");
        } else if (has("(")) {
            auto arg = parseArgumentExpressionList();
            auto call = makeNode<CallExpression>();
            call->add(postfix);
            call->add(arg);
            postfix = call;
        } else {
            auto p = makeNode<PostfixExpr>(peek());
            p->add(postfix);
            postfix = p;
            consume();
        }
    }
    return postfix;
}

AST *Parser::parsePrimary() {
    auto &next = peek();
    if (next.type == Token::Type::Identifier) {
        consume();
        return makeNode<Identifier>(cur());
    } else if (next.type == Token::Type::Int || next.type == Token::Type::Float) {
        consume();
        return makeNode<Number>(cur());
    } else if (next.type == Token::Type::String) {
        consume();
        return makeNode<Literal>(cur());
    } else if (has("(")) {
        consume();
        auto expr = parseExpr(0);
        expect(")");
        return expr;
    } else {
        return nullptr;
    }
}

AST *Parser::parseTernary(AST *cond) {
    expect("?");
    AST *ternary = makeNode<TernaryExpression>();
    ternary->add(cond);
    ternary->add(parseExpr(0));
    expect(":");
    ternary->add(parseExpr(0));
    return ternary;
}

AST *Parser::parseCastExpr() {
    if (has("(") && types.find(at(pos + 2).tok) != types.end()) {  //( kind ) cast_expr
        consume();
        auto cast = makeNode<CastExpression>();
        cast->add(parseTypeName());
        while (!has(")"))consume();
        expect(")");
        cast->add(parseCastExpr());
        return cast;
    } else
        return parseUnary();
}

AST *Parser::parseArgumentExpressionList() {
    expect("(");
    auto arg = makeNode<ArgumentExepressionList>();
    while (hasNext() && !has(")")) {
        arg->add(parseExpr(0));
        if (has(")"))
            break;
        expect(",");
    }
    expect(")");
    return arg;
}

AST *Parser::parseBlock() {
    if (has("{")) {
        consume();
        auto block = makeNode<Block>();
        while (hasNext() && !has("}")) {
            block->add(parseStmt());
        }
        expect("}");
        return block;
    } else {
        return parseStmt();
    }
}

AST *Parser::parseIf() {
    auto stmt = makeNode<If>();
    expect("if");
    expect("(");
    stmt->add(parseExpr(0));
    expect(")");
    stmt->add(parseBlock());
    if (has("else")) {
        consume();
        stmt->add(parseBlock());
    }
    return stmt;
}

AST *Parser::parseWhile() {
    auto w = makeNode<While>();
    expect("while");
    expect("(");
    w->add(parseExpr(0));
    expect(")");
    w->add(parseBlock());
    return w;
}

#define IS_TYPE_SPECIFIER (types.find(peek().tok) != types.end())

AST *Parser::parseStmt() {
    if (has("if"))
        return parseIf();
    else if (has("while")) {
        return parseWhile();
    } else if (has("for")) {
        return parseFor();
    } else if (has("return")) {
        return parseReturn();
    } else if (IS_TYPE_SPECIFIER) {
        auto e = parseDecl();
        expect(";");
        return e;
    } else {
        auto a = parseExpr(0);
        expect(";");
        return a;
    }
}


void Parser::expect(const std::string &token) {
    if (peek().tok != token) {
        auto msg = format(
                "'{}' expected but found '{}'", token, peek().tok
        );
        if (config[quitIfError]) {
            throw ParserException(msg, cur().line, cur().col);
        } else {
            println("{}", msg);
        }
    } else {
        consume();
    }
}

bool Parser::has(const std::string &token) {
    return peek().tok == token;
}

AST *Parser::parseFuncDef() {
    auto ty = parseTypeSpecifier();
    auto func = makeNode<FuncDef>();
    func->add(ty);
    func->add(parsePrimary());
    func->add(parseFuncDefArg());
    func->add(parseBlock());
    return func;
}

AST *Parser::parseFuncDefArg() {
    expect("(");
    auto arg = makeNode<FuncDefArg>();
    while (hasNext() && !has(")")) {
        arg->add(parseParameterType()->first());
        if (has(")"))
            break;
        expect(",");
    }
    expect(")");
    return arg;
}

AST *Parser::parseReturn() {
    expect("return");
    if (has(";")) {
        consume();
        return makeNode<Return>();
    } else {
        auto ret = makeNode<Return>();
        ret->add(parseExpr(0));
        expect(";");
        return ret;
    }
}


/*  Simplifier declaration
 *  declaration: kind-specifier direct-declarator
 *  declarator: {pointer}  direct-declarator
 *  direct-declarator: direct-declarator' {   array-declarator | function-declarator }
 *  direct-declarator': identifier | '('  declarator ')'
 *  array-declarator: '[' int-literal ']'
 *  function-declarator: '(' function-decl-arg ')'
 * */
AST *Parser::parseTypeSpecifier() {
    if (IS_TYPE_SPECIFIER) {
        consume();
        return makeNode<PrimitiveType>(cur());
    } else {
        return nullptr;
    }
}

AST *Parser::parseGlobalDefs() {
    if (has("enum")) {
        return parseEnum();
    } else {
        auto result = parseDecl();
        if (result->kind() != FuncDef().kind()) {
            expect(";");
        }
        return result;
    }

}

AST *Parser::parseDecl() {
    auto type = parseTypeSpecifier();
    declStack.emplace_back(type);
    auto decl = makeNode<DeclarationList>();
    decl->add(parseDeclarationSpecifier());
    while (has(",")) {
        consume();
        decl->add(parseDeclarationSpecifier());
    }
    declStack.pop_back();
    if (has("{")) {
        if (decl->size() != 1) {
            error(format("{}", "unexpected '{'"));
        } else {
            auto func = convertFuncTypetoFuncDef(decl->first());
            func->add(parseBlock());
            return func;
        }
    }

    return decl;
}

AST *Parser::convertFuncTypetoFuncDef(AST *decl) {
    auto func = makeNode<FuncDef>();
    auto functype = decl->first();
    if (functype->kind() != FuncType().kind()) {
        error("function expected");
    }
    func->add(functype->first());
    func->add(decl->second());
    func->add(functype->second());
    return func;
}

AST *Parser::parseTypeName() {
    auto type = parseTypeSpecifier();
    declStack.emplace_back(type);
    if(has("*")){
        parseAbstractDeclarator();
        auto ty = declStack.back();
        declStack.pop_back();
        declStack.pop_back();
        return ty;
    }else {
        declStack.pop_back();
        return type;
    }
}

void Parser::parseAbstractDirectDeclarator() {

}

void Parser::parseAbstractDeclarator() {
    if (has("*")) {
        auto p = makeNode<PointerType>();
        consume();
        while (has("*")) {
            consume();
            auto p2 = makeNode<PointerType>();
            p2->add(p);
            p = p2;
        }
        auto t = declStack.back();
        auto ptr = p;
        while (true) {
            if (ptr->size() == 0) {
                ptr->add(t);
                break;
            } else {
                ptr = static_cast<PointerType *>(ptr->first());
            }
        }
        declStack.emplace_back(p);
    }
}


AST *Parser::parseDeclarationSpecifier() {
    parseDeclarator();
    auto t = declStack.back();
    declStack.pop_back();
    auto decl = extractIdentifier(t);
    if (has("=")) {
        consume();
        decl->add(parseExpr(0));
    }
    return decl;
}

void Parser::parseDirectDeclarator() {
    parseDirectDeclarator_();
    while (hasNext() && (has("(") || has("["))) {
        if (has("["))
            parseArrayDeclarator();
        else
            parseFunctionDeclarator();
    }
}

void Parser::parseDeclarator() {
    if (has("*")) {
        auto p = makeNode<PointerType>();
        consume();
        while (has("*")) {
            consume();
            auto p2 = makeNode<PointerType>();
            p2->add(p);
            p = p2;
        }
        parseDirectDeclarator();
        auto t = declStack.back();
        declStack.pop_back();
        auto ptr = p;
        while (true) {
            if (ptr->size() == 0) {
                ptr->add(t);
                break;
            } else {
                ptr = static_cast<PointerType *>(ptr->first());
            }
        }
        declStack.emplace_back(p);
    } else {
        parseDirectDeclarator();
    }

}

void Parser::parseArrayDeclarator() {
    expect("[");
    ArrayType *arr;
    if (!has("]")) {
        auto size = parsePrimary();
        if (size->kind() != "Number") {
            error("integer expected in array declaration");
        }
        int i;
        sscanf(size->getToken().tok.c_str(), "%d", &i);
        arr = makeNode<ArrayType>(i);
        if (i < 0) {
            error("none-negative integer expected in array declaration");
        }

    } else {
        arr = makeNode<ArrayType>();
    }
    expect("]");
    auto t = declStack.back();
    declStack.pop_back();
    arr->add(t);
    declStack.emplace_back(arr);

}

void Parser::parseFunctionDeclarator() {
    auto arg = parseFuncDefArg();
    auto func = makeNode<FuncType>();
    auto t = declStack.back();
    declStack.pop_back();
    func->add(t);
    func->add(arg);
    declStack.emplace_back(func);
}

void Parser::parseDirectDeclarator_() {
    if (has("(")) {
        consume();
        parseDeclarator();
        expect(")");
    } else if (peek().type == Token::Type::Identifier) {
        auto iden = parsePrimary();
        if (iden->kind() != "Identifier") {
            error("identifier expected in direct declarator");
        }
        declStack.emplace_back(iden);
    }
}

AST* Parser::error(const std::string &message) {
    if (config[quitIfError]) {
        throw ParserException(message);
    } else {
        println(message);
    }
    return nullptr;
}

AST *Parser::extractIdentifier(AST *ast) {
    AST *ty = declStack.back();
    if (ast->kind() == "Identifier") {
        auto decl = makeNode<Declaration>();
        decl->add(ty);
        decl->add(ast);
        return decl;
    } else {
        AST *walker = ast;
        while (walker->size() && (walker->first()->kind() != "Identifier")) {
            walker = walker->first();
        }
        auto iden = walker->first();
        walker->set(0, ty);
        auto decl = makeNode<Declaration>();
        decl->add(ast);
        decl->add(iden);
        return decl;
    }
}

AST *Parser::parseFor() {
    expect("for");
    expect("(");
    auto result = makeNode<For>();
    //init
    if (IS_TYPE_SPECIFIER) {
        result->add(parseDecl());
        expect(";");
    } else {
        if (has(";")) {
            consume();
            result->add(makeNode<Empty>());
        } else {
            result->add(parseExpr(0));
            expect(";");
        }
    }
    //cond
    if (has(";")) {
        result->add(makeNode<Empty>());
    } else {
        result->add(parseExpr(0));

    }
    expect(";");
    //step
    if (has(")")) {
        result->add(makeNode<Empty>());
    } else {

        result->add(parseExpr(0));
    }
    expect(")");
    result->add(parseBlock());
    return result;
}

AST *Parser::parseParameterType() {
    auto type = parseTypeSpecifier();
    declStack.emplace_back(type);
    auto decl = makeNode<DeclarationList>();
    decl->add(parseDeclarationSpecifier());
    declStack.pop_back();
    return decl;
}

BinaryExpression *Parser::hackExpr(BinaryExpression *e) {
    auto op = e->getToken().tok;
    if (op == "+=" || op == "-=" || op == "*=" || op == "/="
        || op == "%=" || op == "<<=" || op == ">>=") {
        auto t = e->getToken();
        op.pop_back();
        t.tok = op;
        Token t2 = t;
        t2.tok = "=";
        auto e2 = makeNode<BinaryExpression>(t2);
        auto e3 = makeNode<BinaryExpression>(t);
        e3->add(e->first());
        e3->add(e->second());
        e2->add(e->first());
        e2->add(e3);
        return e2;
    } else {
        return e;
    }

}

AST *Parser::parseEnum() {
    expect("enum");
    auto e = makeNode<Enum>();
    expect("{");
    while (hasNext() && !has("}")) {
        e->add(parseExpr(0));
        if (has("}"))
            break;
        else
            expect(",");
    }
    expect("}");
    expect(";");
    return e;
}

