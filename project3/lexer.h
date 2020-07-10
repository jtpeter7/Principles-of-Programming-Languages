/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>
#include <iostream>
#include <istream>
#include <cctype>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stack>

#include "inputbuf.h"
using namespace std;

 // ------- token types -------------------

typedef enum {
	END_OF_FILE = 0,
	IF, WHILE, DO, THEN, PRINT, PUBLIC, PRIVATE, SWITCH, INT, REAL, BOOL, TRUE, FALSE, CASE, NOT,
	PLUS, MINUS, DIV, MULT,
	EQUAL, COLON, COMMA, SEMICOLON,
	LBRAC, RBRAC, LPAREN, RPAREN,
	NOTEQUAL, GREATER, LESS, LTEQ, GTEQ,
	DOT, NUM, ID, ERROR, REALNUM, LBRACE, RBRACE,
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
	int Parse_Decl_Var_List();
	int Parse_Var_Decl();
	int Parse_Var_List();
	int Parse_Type_Name();
	int Parse_Body();
	int Parse_Stmt_List();
	int Parse_Stmt();
	int Parse_Assignment_Stmt();
	int Parse_Expression();
	int Parse_Unary_Operator();
	int Parse_Binary_Operator();
	int Parse_Primary();
	int Parse_If_Stmt();
	int Parse_While_Stmt();
	int Parse_Switch_Stmt();
	int Parse_Case_List();
	int Parse_Case();
	void syntax_error();
	void expect(TokenType);
	void Type_Mismatch(string ERROR);
	void check_solution();
	void check_equation();

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
