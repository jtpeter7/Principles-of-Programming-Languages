/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

 // ------- token types -------------------

typedef enum {
	END_OF_FILE = 0,
	IF, WHILE, DO, THEN, PRINT, PUBLIC, PRIVATE, SWITCH,
	PLUS, MINUS, DIV, MULT,
	EQUAL, COLON, COMMA, SEMICOLON,
	LBRAC, RBRAC, LPAREN, RPAREN,
	NOTEQUAL, GREATER, LESS, LTEQ, GTEQ,
	DOT, NUM, ID, ERROR,
	//New Tokens
	REALNUM, //BASE08NUM, BASE16NUM,
	LBRACE, RBRACE,
	INT, REAL, BOOL, TRUE, FALSE, CASE, NOT
} TokenType;

class Token {
public:
	void Print();

	std::string lexeme;
	TokenType token_type;
	int line_no;
};

class LexicalAnalyzer {
public:
	Token GetToken();
	TokenType UngetToken(Token);
	LexicalAnalyzer();
	int Parse_Program();
	int Parse_Global_Vars();
	int Parse_Var_List();
	int Parse_Scope();
	int Parse_Public_Vars();
	int Parse_Private_Vars();
	int Parse_Stmt_List();
	int Parse_Stmt();

private:
	std::vector<Token> tokens;
	int line_no;
	Token tmp;
	InputBuffer input;

	bool SkipSpace();
	bool IsKeyword(std::string);
	TokenType FindKeywordIndex(std::string);
	Token ScanIdOrKeyword();
	Token ScanNumber();
	Token NumType(Token);
};

#endif  //__LEXER__H__
