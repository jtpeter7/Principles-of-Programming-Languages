/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stack>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;
string reserved[] = { "END_OF_FILE",
	"IF", "WHILE", "DO", "THEN", "PRINT", "PUBLIC", "PRIVATE","SWITCH","INT","REAL","BOOL","TRUE","FALSE","CASE","NOT",
	"PLUS", "MINUS", "DIV", "MULT",
	"EQUAL", "COLON", "COMMA", "SEMICOLON",
	"LBRAC", "RBRAC", "LPAREN", "RPAREN",
	"NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
	"DOT", "NUM", "ID", "ERROR", "REALNUM", "LBRACE", "RBRACE"
};

#define KEYWORDS_COUNT 15
string keyword[] = { "if", "while", "do", "then", "print", "public", "private","switch","int","real","bool","true","false","case","!" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}
LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}
bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}
bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}
TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c, x,y,z,q,p;

    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0') {
            tmp.lexeme = "0";
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
        }
		
		tmp.token_type = NUM;
		input.GetChar(x);
		if (x == '.') {
			input.GetChar(y);
			if (int(y) >= 48 && int(y) <= 57) {
				tmp.token_type = REALNUM;
				tmp.lexeme += '.';
				while (int(y) >= 48 && int(y) <= 57) {
					tmp.lexeme += y;
					input.GetChar(y);
				}
				input.UngetChar(y);//unget the first non digit after
			}
			else {
				input.UngetChar(y);//unget after the .
				input.UngetChar(x);//unget the .
			}
		}
		else {
			input.UngetChar(x);
		}
        tmp.line_no = line_no;
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}
Token LexicalAnalyzer::ScanIdOrKeyword()
{
	char c;
	input.GetChar(c);

	if (isalpha(c)) {
		tmp.lexeme = "";
		while (!input.EndOfInput() && isalnum(c)) {
			tmp.lexeme += c;
			input.GetChar(c);
		}
		if (!input.EndOfInput()) {
			input.UngetChar(c);
		}
		tmp.line_no = line_no;
		if (IsKeyword(tmp.lexeme))
			tmp.token_type = FindKeywordIndex(tmp.lexeme);
		else
			tmp.token_type = ID;
	}
	else {
		if (!input.EndOfInput()) {
			input.UngetChar(c);
		}
		tmp.lexeme = "";
		tmp.token_type = ERROR;
	}
	return tmp;
}
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
	tokens.push_back(tok);;
	return tok.token_type;
}
Token LexicalAnalyzer::GetToken()
{
	char c, d, e;

	// if there are tokens that were previously
	// stored due to UngetToken(), pop a token and
	// return it without reading from input
	if (!tokens.empty()) {
		tmp = tokens.back();
		tokens.pop_back();
		return tmp;
	}

	SkipSpace();
	tmp.lexeme = "";
	tmp.line_no = line_no;
	input.GetChar(c);
	/////**/cout << "GETTOKEN: c:" << c << endl;
	bool helpme = true;
	while (helpme == true) {
		if (c == '/') {
			input.GetChar(d);//check for comments
			/////**/cout << "GETTOKEN: d:" << d << endl;
			if (d == '/') {
				input.GetChar(e);
				/////**/cout << "GETTOKEN: e:" << e;
				while (e != '\n') {
					input.GetChar(e);
					/////**/cout << e;
				}
				/////**/cout << endl;
				SkipSpace();
				input.GetChar(c);//get next input char
			}
			else {
				helpme = false;
				input.UngetChar(d);
			}
		}
		else {
			helpme = false;
		}
	}
	switch (c) {
	case '.':
		tmp.token_type = DOT;
		return tmp;
	case '+':
		tmp.token_type = PLUS;
		return tmp;
	case '-':
		tmp.token_type = MINUS;
		return tmp;
	case '/':
		tmp.token_type = DIV;
		return tmp;
	case '*':
		tmp.token_type = MULT;
		return tmp;
	case '=':
		tmp.token_type = EQUAL;
		return tmp;
	case ':':
		tmp.token_type = COLON;
		return tmp;
	case ',':
		tmp.token_type = COMMA;
		return tmp;
	case ';':
		tmp.token_type = SEMICOLON;
		return tmp;
	case '[':
		tmp.token_type = LBRAC;
		return tmp;
	case ']':
		tmp.token_type = RBRAC;
		return tmp;
	case '(':
		tmp.token_type = LPAREN;
		return tmp;
	case ')':
		tmp.token_type = RPAREN;
		return tmp;
	case '<':
		input.GetChar(c);
		if (c == '=') {
			tmp.token_type = LTEQ;
		}
		else if (c == '>') {
			tmp.token_type = NOTEQUAL;
		}
		else {
			if (!input.EndOfInput()) {
				input.UngetChar(c);
			}
			tmp.token_type = LESS;
		}
		return tmp;
	case '>':
		input.GetChar(c);
		if (c == '=') {
			tmp.token_type = GTEQ;
		}
		else {
			if (!input.EndOfInput()) {
				input.UngetChar(c);
			}
			tmp.token_type = GREATER;
		}
		return tmp;
	case '{':
		tmp.token_type = LBRACE;
		return tmp;
	case '}':
		tmp.token_type = RBRACE;
		return tmp;
	case '!':
		tmp.token_type = NOT;
		return tmp;
	default:
		if (isdigit(c)) {
			//Find what type of digit
			input.UngetChar(c);
			return ScanNumber();
		}
		else if (isalpha(c)) {
			input.UngetChar(c);
			return ScanIdOrKeyword();
		}
		else if (input.EndOfInput())
			tmp.token_type = END_OF_FILE;
		else
			tmp.token_type = ERROR;

		return tmp;
	}
}

struct Variable {
	string name;//name
	char* value;//value
	string type;//data type
	bool BOOL;
	bool SWITCH;
};
struct Equation {
	Variable LHS;
	vector<struct Variable> RHS;
};
vector<struct Variable> VariableList;
LexicalAnalyzer lexer;
Token token1, token2, token3, token4;
int VarNum = 0;
int VarLast = 0;
Variable LeftHandSide;
vector<struct Variable> RightHandSide;
Variable EXPRESSION;
string output = "";
vector<struct Equation> AllEquations;



void LexicalAnalyzer::syntax_error() {
	cout << "Syntax Error" << endl;
	exit(1);
}
void LexicalAnalyzer::expect(TokenType check) {
	Token temporary = lexer.GetToken();
	if (temporary.token_type == check) {
		
	}
	else {
		syntax_error();
	}
	return;
}
void LexicalAnalyzer::Type_Mismatch(string ERROR) {
	cout << "TYPE MISMATCH " << line_no<< " " << ERROR << endl;
	exit(1);
}
void LexicalAnalyzer::check_equation() {
	for (int i = 0; i < RightHandSide.size(); i++) {
		
		if ((RightHandSide[0].type == RightHandSide[i].type) || (RightHandSide[i].type == "undefined") || (RightHandSide[0].type == "undefined")) {
			if (RightHandSide[0].type != RightHandSide[i].type && RightHandSide[0].type == "undefined") {
				RightHandSide[0].type = RightHandSide[i].type;
			}
			if (RightHandSide[0].type != RightHandSide[i].type && RightHandSide[i].type == "undefined") {
				RightHandSide[i].type = RightHandSide[0].type;
			}
		}
		else {
			Type_Mismatch("C2");
		}
		
		if (EXPRESSION.SWITCH == true) {
			if (RightHandSide[i].type != "int") {
				Type_Mismatch("C5");
			}
		}
	}
	for (int i = 0; i < VariableList.size(); i++) {
		for (int j = 0; j < RightHandSide.size(); j++) {
			if (RightHandSide[j].name == VariableList[i].name) {
				VariableList[i].type = RightHandSide[j].type;
			}
		}
	}
	struct Equation newequation;
	newequation.LHS = LeftHandSide;
	newequation.RHS = RightHandSide;
	AllEquations.push_back(newequation);
	RightHandSide.clear();
}
void LexicalAnalyzer::check_solution() {
	for (int a = 0; a < AllEquations.size(); a++) {
		for (int i = 0; i < AllEquations[a].RHS.size(); i++) {
			if ((AllEquations[a].RHS[0].type == AllEquations[a].RHS[i].type) || (AllEquations[a].RHS[i].type == "undefined") || (AllEquations[a].RHS[0].type == "undefined")) {
				if (AllEquations[a].RHS[0].type != AllEquations[a].RHS[i].type && AllEquations[a].RHS[0].type == "undefined") {
					AllEquations[a].RHS[0].type = AllEquations[a].RHS[i].type;
				}
				if (AllEquations[a].RHS[0].type != AllEquations[a].RHS[i].type && AllEquations[a].RHS[i].type == "undefined") {
					AllEquations[a].RHS[i].type = AllEquations[a].RHS[0].type;
				}
			}
			else {
				Type_Mismatch("C2");
			}
		}
		for (int i = 0; i < VariableList.size(); i++) {
			for (int j = 0; j < AllEquations[a].RHS.size(); j++) {
				if (AllEquations[a].RHS[j].name == VariableList[i].name) {
					VariableList[i].type = AllEquations[a].RHS[j].type;
				}
			}
		}
	}
}

int LexicalAnalyzer::Parse_Program() {
	token1 = lexer.GetToken();
	//cout/**/ << "Parse_Program"<<endl;
	if (token1.token_type != END_OF_FILE) {
		lexer.UngetToken(token1);
		Parse_Global_Vars();
		Parse_Body();
	}
	//check_solution();
	return 0;
}
int LexicalAnalyzer::Parse_Global_Vars() {
	//cout/**/ << "Parse_Global_Vars" << endl;
	token1 = lexer.GetToken();
	if (token1.token_type == LBRACE) {
		lexer.UngetToken(token1);
	}
	else {
		lexer.UngetToken(token1);
		Parse_Decl_Var_List();
	}

	return 0;
}
int LexicalAnalyzer::Parse_Decl_Var_List() {
	//cout/**/ << "Parse_Decl_Var_List" << endl;
	Parse_Var_Decl();
	token1 = lexer.GetToken();
	if (token1.token_type == LBRACE) {
		lexer.UngetToken(token1);
	}
	else {
		lexer.UngetToken(token1);
		Parse_Decl_Var_List();
	}
	return 0;
}
int LexicalAnalyzer::Parse_Var_Decl() {
	//cout/**/ << "Parse_Var_Decl" << endl;
	VarLast = VarNum;//Set the first var for the list being instantiated in the same line to the current var num
	//LeftHandSide = VariableList[VarNum];
	///**/cout << "NUMBER BEFORE LINE::::::::::::::::" << VarLast << endl;
	Parse_Var_List();
	///**/cout << "NUMBER ON LINE::::::::::::::::" << VarNum-VarLast << endl;
	//Setting all vars instantiated on the line to the type
	
	expect(COLON);
	Parse_Type_Name();
	//cout/**/ << "Parse_Var_Decl: GOT TYPE" << endl;
	expect(SEMICOLON);
	//cout/**/ << "Parse_Var_Decl: GOT SEMICOLON" << endl;
}
int LexicalAnalyzer::Parse_Var_List() {
	//cout/**/ << "Parse_Var_List" << endl;
	token1 = lexer.GetToken();
	if (token1.token_type == ID) {
		struct Variable temp_node;
		//cout/**/ << "Parse_Var_List: GOT ID" << endl;
		temp_node.name = token1.lexeme;
		VariableList.push_back(temp_node);
		VarNum++;//Increase number of Vars
		///**/cout << "NUMBER OF VARS::::::::::::::::" << VarNum << endl;
		token1 = lexer.GetToken();
		if (token1.token_type == COLON) {
			lexer.UngetToken(token1);
		}
		else if (token1.token_type == COMMA) {
			Parse_Var_List();
		}
		else {
			syntax_error();
		}
	}
	else {
		syntax_error();
	}
}
int LexicalAnalyzer::Parse_Type_Name() {
	//cout/**/ << "Parse_Type_Name:START" << endl;
	token1 = lexer.GetToken();
	switch (token1.token_type) {
	case INT:
		//cout/**/ << "Parse_Type_Name: GOT INT" << endl;
		for (int i = VarLast; i < VarNum; i++) {
			VariableList[i].type = "int";
		}
		break;
	case REAL:
		break;
	case BOOL:
		break;
	default:
		syntax_error();
	}
	//cout/**/ << "Parse_Type_Name:END" << endl;
}
int LexicalAnalyzer::Parse_Body() {
	//cout/**/ << "Parse_Body" << endl;
	expect(LBRACE); //cout << ">>>>>>>>>>>>>>>>>>" << endl;
	Parse_Stmt_List();
	expect(RBRACE); //cout << "<<<<<<<<<<<<<<<<<<" << endl;
}
int LexicalAnalyzer::Parse_Stmt_List() {
	//cout/**/ << "Parse_Stmt_List: START" << endl;
	Parse_Stmt();
	token1 = lexer.GetToken();
	//cout/**/ << "Parse_Stmt_List: GOT token1 == "<<token1.lexeme << "::::"<<token1.token_type << endl;
	if (token1.token_type == END_OF_FILE) { cout << "AAAAAAAAAAAAA" << endl; }
	if (token1.token_type == RBRACE) {
		lexer.UngetToken(token1);
	}
	else if (token1.token_type == ID || token1.token_type == IF || token1.token_type == WHILE || token1.token_type == SWITCH){
		lexer.UngetToken(token1);
		Parse_Stmt_List();
	}
	//cout/**/ << "Parse_Stmt_List: END" << endl;
}
int LexicalAnalyzer::Parse_Stmt() {
	//cout/**/ << "Parse_Stmt: START" << endl;
	token1 = lexer.GetToken();
	switch (token1.token_type) {
	case ID: lexer.UngetToken(token1);
		//cout/**/ << "Parse_Stmt: GOT ID== " << token1.lexeme << endl; 
		Parse_Assignment_Stmt(); 
		break; 
	case IF: lexer.UngetToken(token1);
		//cout/**/ << "Parse_Stmt: GOT IF== " << token1.lexeme << endl;
		Parse_If_Stmt();
		break;
	case WHILE: lexer.UngetToken(token1);
		//cout/**/ << "Parse_Stmt: GOT WHILE== " << token1.lexeme << endl;
		Parse_While_Stmt();
		break;
	case SWITCH: lexer.UngetToken(token1);
		//cout/**/ << "Parse_Stmt: GOT SWITCH== " << token1.lexeme << endl; 
		Parse_Switch_Stmt();
		break;
	default: syntax_error();
	}
	//cout/**/ << "Parse_Stmt: END" << endl;
}
/**/int LexicalAnalyzer::Parse_Assignment_Stmt() {
	//cout/**/ << "Parse_Assignment_Stmt: START" << endl;
	token1 = lexer.GetToken();
	bool exists = false;
	for (int i = 0; i < VariableList.size(); i++) {
		if (VariableList[i].name == token1.lexeme) {
			exists = true;
			LeftHandSide = VariableList[i];
			token2 = lexer.GetToken();
			token3 = lexer.GetToken();
			if (token3.token_type == NOT) {
				if (VariableList[i].type != "bool" && VariableList[i].type != "undefined") {
					Type_Mismatch("C1");
				}
				VariableList[i].type = "bool";
			}
			lexer.UngetToken(token3);
			lexer.UngetToken(token2);
		}
	}
	if (!exists) {
		Variable temp_node;
		temp_node.name = token1.lexeme;
		temp_node.type = "undefined";
		///**/cout << "Parse_Assignment_Stmt: NUMBER OF VARS::::::::" << token1.lexeme << "::::::::" << VarNum<<"=="<<VariableList.size() << endl;

		token2 = lexer.GetToken();
		token3 = lexer.GetToken();
		if (token3.token_type == NOT) {
			temp_node.type = "bool";
		}
		lexer.UngetToken(token3);
		lexer.UngetToken(token2);

		VariableList.push_back(temp_node);
		LeftHandSide = VariableList[VariableList.size() - 1];
		VarLast = VarNum;
		VarNum++;//r as var 4
	}
	
	expect(EQUAL);
	Parse_Expression();
	check_equation();
	expect(SEMICOLON);
	//RightHandSide.clear();
	//cout/**/ << "Parse_Assignment_Stmt: DONE" << endl;
}
int LexicalAnalyzer::Parse_Expression() {
	//cout/**/ << "Parse_Expression" << endl;
	token1 = lexer.GetToken();
	string tempLeft;
	switch (token1.token_type) {
	case NOT: 
		//cout/**/ << "Parse_Expression: Got UNARY" << endl;
		lexer.UngetToken(token1);
		Parse_Unary_Operator();
		if (LeftHandSide.type == "bool" || LeftHandSide.type == "undefined") {
			LeftHandSide.type = "bool";
		}
		else {
			Type_Mismatch("C3");
		}
		Parse_Expression();
		break;
	case PLUS:case MINUS:case MULT:case DIV:
		//if (LeftHandSide.type == "int" || LeftHandSide.type == "real" || LeftHandSide.type == "undefined") {
			lexer.UngetToken(token1);
			Parse_Binary_Operator();
			tempLeft = LeftHandSide.type;
			LeftHandSide.type = "undefined";
			Parse_Expression();
			Parse_Expression();
			LeftHandSide.type = tempLeft;
		/*}
		else {
			cout << "SPANK" << endl;
			Type_Mismatch("C1");
		}*/
		break;
	case GREATER:case LESS:case GTEQ:case LTEQ:case EQUAL:case NOTEQUAL:
		//cout/**/ << "Parse_Expression: Got BINARY" << endl;
		if (LeftHandSide.type == "bool" || LeftHandSide.type == "undefined") {
			
			lexer.UngetToken(token1);
			Parse_Binary_Operator();
			tempLeft = LeftHandSide.type;
			LeftHandSide.type = "undefined";
			Parse_Expression();
			Parse_Expression();
			LeftHandSide.type = tempLeft;
		}
		else {
			//cout << "SPOINK" << endl;
			Type_Mismatch("C1");
		}
		break;
	case ID:case NUM:case REALNUM: case TRUE: case FALSE:
		//cout/**/ << "Parse_Expression: Got PRIMARY" << endl;
		lexer.UngetToken(token1);
		Parse_Primary();
		break;
	default:
		syntax_error();
	}
}
int LexicalAnalyzer::Parse_Unary_Operator() {
	//cout/**/ << "Parse_Unary_Operator" << endl;
	expect(NOT);
}
int LexicalAnalyzer::Parse_Binary_Operator() {
	//cout/**/ << "Parse_Binary_Operator" << endl;
	token1 = lexer.GetToken();
	switch (token1.token_type) {
	case PLUS:
		break;
	case MINUS:
		break;
	case MULT:
		break;
	case DIV:
		break;
	case GREATER:
		break;
	case LESS:
		break;
	case GTEQ:
		break;
	case LTEQ:
		break;
	case EQUAL:
		break;
	case NOTEQUAL:
		break;
	default: syntax_error();
	}
}
int LexicalAnalyzer::Parse_Primary() {
	//cout/**/ << "Parse_Primary" << endl;
	token1 = lexer.GetToken();
	bool exists = false;
	Variable temp_node;

	switch (token1.token_type) {
	case ID:
		//cout/**/ << "Parse_Primary: Got ID == " << token1.lexeme << endl;
		for (int i = 0; i < VariableList.size(); i++) {
			if (VariableList[i].name == token1.lexeme) {
				exists = true;
				if (VariableList[i].type == "undefined"){
					temp_node.type = "undefined";
					RightHandSide.push_back(temp_node);
				}
				else {
					temp_node.type = VariableList[i].type;
					RightHandSide.push_back(temp_node);
				}
				if (LeftHandSide.type == "undefined") {
					LeftHandSide.type = VariableList[i].type;
				}
			}
		}
		if (!exists) {
			temp_node.name = token1.lexeme;
			temp_node.type = "undefined";
			VariableList.push_back(temp_node);
			RightHandSide.push_back(temp_node);
			///**/cout << "Parse_Assignment_Stmt: NUMBER OF VARS::::::::" << token1.lexeme << "::::::::" << VarNum << "==" << VariableList.size() << endl;
			VarNum++;//r as var 4
		}
		break;
	case NUM:
		//cout/**/ << "Parse_Primary: Got NUM == " << token1.lexeme << endl;
		temp_node.type = "int";
		RightHandSide.push_back(temp_node);
		if (LeftHandSide.type == "undefined") {
			LeftHandSide.type = "int";
		}
		break;
	case REALNUM:
		//cout/**/ << "Parse_Primary: Got REALNUM == " << token1.lexeme << endl;
		temp_node.type = "real";
		RightHandSide.push_back(temp_node);
		if (LeftHandSide.type == "undefined") {
			LeftHandSide.type = "real";
		}
		break;
	case TRUE:
		//cout/**/ << "Parse_Primary: Got TRUE == " << token1.lexeme << endl;
		temp_node.type = "bool";
		RightHandSide.push_back(temp_node);
		if (LeftHandSide.type == "undefined") {
			LeftHandSide.type = "bool";
		}
		break;
	case FALSE:
		//cout/**/ << "Parse_Primary: Got FALSE == " << token1.lexeme << endl;
		temp_node.type = "bool";
		RightHandSide.push_back(temp_node);
		if (LeftHandSide.type == "undefined") {
			LeftHandSide.type = "bool";
		}
		break;
	}
	for (int i = 0; i < VariableList.size(); i++) {
		if (VariableList[i].name == LeftHandSide.name) {
			VariableList[i].type = LeftHandSide.type;
		}
	}
}
int LexicalAnalyzer::Parse_If_Stmt() {
	//cout/**/ << "Parse_If_Stmt" << endl;
	expect(IF);
	expect(LPAREN);
	token2 = lexer.GetToken();
	if (token2.token_type == ID) {
		for (int i = 0; i < VariableList.size(); i++) {
			if (VariableList[i].name == token2.lexeme) {
				if (VariableList[i].type != "bool" && VariableList[i].type != "undefined") {
					Type_Mismatch("C4");
				}
			}
		}
	}
	LeftHandSide.type = "bool";
	lexer.UngetToken(token2);
	Parse_Expression();
	check_equation();
	expect(RPAREN);
	Parse_Body();
}
int LexicalAnalyzer::Parse_While_Stmt () {
	//cout/**/ << "Parse_While_Stmt" << endl;
	expect(WHILE);
	expect(LPAREN);
	token2 = lexer.GetToken();
	if (token2.token_type == ID) {
		for (int i = 0; i < VariableList.size(); i++) {
			if (VariableList[i].name == token2.lexeme) {
				if (VariableList[i].type != "bool" && VariableList[i].type != "undefined") {
					Type_Mismatch("C4");
				}
			}
		}
	}
	LeftHandSide.type = "bool";
	lexer.UngetToken(token2);
	Parse_Expression();
	check_equation();
	expect(RPAREN);
	Parse_Body();
}
int LexicalAnalyzer::Parse_Switch_Stmt() {
	//cout/**/ << "Parse_Switch_Stmt: START" << endl;
	expect(SWITCH);
	expect(LPAREN);
	if (token2.token_type == PLUS || token2.token_type == MINUS || token2.token_type == MULT ||
		token2.token_type == DIV|| token2.token_type == ID) {
		lexer.UngetToken(token2);
	}
	else {
		Type_Mismatch("C5");
	}
	Parse_Expression();
	check_equation();
	//necessary_type = "undefined";
	expect(RPAREN);
	expect(LBRACE); //cout << ">>>>>>>>>>>>>>>>>>" << endl;
	Parse_Case_List();
	expect(RBRACE); //cout << "<<<<<<<<<<<<<<<<<<" << endl;
	//cout/**/ << "Parse_Switch_Stmt: END" << endl;
}
int LexicalAnalyzer::Parse_Case_List() {
	//cout/**/ << "Parse_Case_List: START" << endl;
	Parse_Case();
	token1 = lexer.GetToken();
	if (token1.token_type == CASE) {
		lexer.UngetToken(token1);
		Parse_Case_List();
	}
	else {
		lexer.UngetToken(token1);
	}
	//cout/**/ << "Parse_Case_List: DONE" << endl;
}
int LexicalAnalyzer::Parse_Case() {
	//cout/**/ << "Parse_Case: START" << endl;
	expect(CASE);
	expect(NUM);
	expect(COLON);
	Parse_Body();
	//cout/**/ << "Parse_Case: DONE" << endl;
}

void Print_Vars() {
	bool lastund = false;
	for (int i = 0; i < VariableList.size(); i++) {
		if (VariableList[i].type == "undefined") {
			if (! lastund) {
				cout << VariableList[i].name;
			}
			if(lastund){
				cout << ", " << VariableList[i].name;
			}
			lastund = true;
		}
		if (VariableList[i].type == "int") {
			if (lastund) {
				cout << ": ? #" << endl;
			}
			cout << VariableList[i].name;
			cout << ": int #" << endl;
			lastund = false;
		}
		if (VariableList[i].type == "real") {
			if (lastund) {
				cout << ": ? #" << endl;
			}
			cout << VariableList[i].name;
			cout << ": real #" << endl;
			lastund = false;
		}
		if (VariableList[i].type == "bool") {
			if (lastund) {
				cout << ": ? #" << endl;
			}
			cout << VariableList[i].name;
			cout << ": bool #" << endl;
			lastund = false;
		}
		
	}
	if (lastund) {
		cout << ": ? #" << endl;
	}
}

int main()
{
	int a = lexer.Parse_Program();
	//int a = check_solution();
	Print_Vars();
}
