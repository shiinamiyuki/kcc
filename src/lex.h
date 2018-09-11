/*
 * lex.h
 *
 *  Created on: 2018/7/6
 *      Author: xiaoc
 */

#ifndef LEX_H_
#define LEX_H_
#include "kcc.h"
namespace  kcc {
	struct Token;

	struct Token {
		enum class Type {
			String, Int,Float, Identifier, Keyword, Punctuator, Terminator, Nil
		} type;
		std::string tok;
		char *filename;
		int line;
		int col;

		Token(Type t, const std::string to, int l, int c);

		Token() :
				tok(""), line(0), col(0), type(Type::Nil) {
		}
	};


	class Lexer {
		int pos;
		int line, col;
		std::string source;
		std::vector<Token> tokenStream;

		char at(int idx);

		Token next();

		void consume();

		char cur();

		char peek();

		char peek2();

		void skipspace();

		void skipcomment();

		int isComment();

		bool isKeyWord(const std::string &s);

		Token identifier();

		Token number();

		Token punctuator();

		Token string();

	public:
		Lexer(const std::string &s);

		void scan();

		std::vector<Token> &getTokenStream();
	};
}
#endif /* LEX_H_ */
