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
    "IF", "WHILE", "DO", "THEN", "PRINT", "PUBLIC", "PRIVATE",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR",
	//New Token Types
	//"REALNUM", "BASE08NUM", "BASE16NUM"
	// TODO: Add labels for new token types here (as string)
	"LBRACE", "RBRACE"
};

#define KEYWORDS_COUNT 7
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT", "public", "private" };

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
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
		/*Project 1, told to disable
	   for(int k = 0; k<20;k++){
		 input.GetChar(p);
		 if((int(p) >=48 && int(p) <=57) || (int(p) >=65 && int(p)  <=70)){
			tmp.lexeme += p;
		 }else{input.UngetChar(p); k=22;}
	   }
        input.GetChar(x);
        if(x == '.'){
            tmp.lexeme += x;
            for(int i = 0; i<10;i++){
                input.GetChar(q);
                if(int(q) >= 48 && int(q) <= 57){
                    tmp.lexeme += q;
                }else{
                    i=99;
                    input.UngetChar(q);
                }
            }
        }else if(x=='x'){
                tmp.lexeme += x;
                input.GetChar(y);
                input.GetChar(z);
                tmp.lexeme += y;
                tmp.lexeme += z;
        }else{
            input.UngetChar(x);
        }
		tmp.token_type = NumType(tmp).token_type;*/
		tmp.token_type = NUM;
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
/*Project 1, told to disable
Token LexicalAnalyzer::NumType(Token temporary) {
	Token temp;
	string Value = temporary.lexeme;
	bool real = true, b8 = true, b16 = true;
	char ValueAsChar[Value.length() + 1];
	strcpy(ValueAsChar, Value.c_str());
	if (int(ValueAsChar[0]) != 48) {//IF positive digit
		for (int i = 0; i < Value.length(); i++) {
			if (real) {
				if ((int(ValueAsChar[i]) >= 48 && int(ValueAsChar[i]) <= 57) || (int(ValueAsChar[i]) == 46)) {//0-9 & DOT

				}
				else {
					real = false;
				}
			}
			if (b8) {
				if ((int(ValueAsChar[i]) >= 48 && int(ValueAsChar[i]) <= 55)) {//0-8

				}
				else if ((int(ValueAsChar[i]) == 120) && (int(ValueAsChar[i + 1]) == 48) && (int(ValueAsChar[i+2]) == 56)) {
					//temp.token_type = BASE08NUM; return temp;
					b16 = false;
					real = false;
					i = Value.length();
				}
				else {
					b8 = false;
				}
			}
			if (b16) {
				if (((int(ValueAsChar[i]) >= 48 && int(ValueAsChar[i]) <= 57)) ||
					((int(ValueAsChar[i]) >= 65 && int(ValueAsChar[i]) <= 70))) {//0-9,A-F

				}
				else if ((int(ValueAsChar[i]) == 120) && (int(ValueAsChar[i + 1]) == 49) && (int(ValueAsChar[i+2]) == 54)) {
					//temp.token_type = BASE16NUM; return temp;
					b8 = false;
					real = false;
					i=Value.length();
				}
				else {
					b16 = false;
				}
			}
		}
	}
	else {
		temp.token_type = ERROR;
	}
	if (b16) {
		temp.token_type = BASE16NUM;
	}
	if (b8) {
		temp.token_type = BASE08NUM;
	}if (real) {
		temp.token_type = REALNUM;
	}
	if (!real && !b16 && !b8) {
		temp.token_type = ERROR;
	}
	return temp;
}*/

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
    } else {
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
    char c,d,e;

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
	///**/cout << "GETTOKEN: c:" << c << endl;
	bool helpme = true;
	while (helpme == true) {
		if (c == '/') {
			input.GetChar(d);//check for comments
			///**/cout << "GETTOKEN: d:" << d << endl;
			if (d == '/') {
				input.GetChar(e);
				///**/cout << "GETTOKEN: e:" << e;
				while (e != '\n') {
					input.GetChar(e);
					///**/cout << e;
				}
				///**/cout << endl;
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


//PROJECT 2 and on://cout/**/
struct sTableItem {//bottom layer, this is the var name and the scope_name present in along the permission level of the item in the scope
	char* name;
	char* scope;
	int permission;
};
struct sTable {//Top Layer of a scope. All elements in a scope will be made of this and point to the lines besides them in a scope
	sTableItem* item;
	sTable* prev;
	sTable* next;
};
struct store {//store assignments with the 2 scope names and var names
	string lhs;
	string rhs;
	char* scope1;
	char* scope2;
};
//Globals
vector< struct store> storeList;//List of all Assignments made from MakeAssignment()
struct sTable* SymbolTablePointer;
struct sTable* SymbolTableTemp;
struct sTable* SymbolTableNew;
char* sScope = "::";
int SymbolPermission = 0;

void MakeAssignment(Token tok1, Token tok2) {
	struct sTable* temp = SymbolTableNew;
	struct sTable* temp1 = SymbolTableNew;
	char* scope1 = (char*)malloc(sizeof(char) * 50);
	char* scope2 = (char*)malloc(sizeof(char) * 50);

	while (temp != NULL) {//Scope not null
		if (temp->item->name == tok1.lexeme) {//Find the correct upper level Scope item
			if (temp->item->permission == 2 && sScope != temp->item->scope) {
				temp = temp->prev;
				continue;
			}
			else {
				scope1 = temp->item->scope;//Set LHS Scope 
				break;
			}
		}
		temp = temp->prev;
	}
	while (temp1 != NULL) {//Scope not null
		if (temp1->item->name == tok2.lexeme) {//Find the correct upper level Scope item
			if (temp1->item->permission == 2 && sScope != temp1->item->scope) {
				temp1 = temp1->prev;
				continue;
			}
			else {
				scope2 = temp1->item->scope;//Set RHS Scope 
				break;
			}
		}
		temp1 = temp1->prev;
	}
	if (strlen(scope1) == 0) {
		scope1 = new char[2];
		scope1[0] = '?';
		scope1[1] = '\0';
	}
	if (strlen(scope2) == 0) {
		scope2 = new char[2];
		scope2[0] = '?';
		scope2[1] = '\0';
	}
	struct store temp_node;
	temp_node.lhs = tok1.lexeme;
	temp_node.rhs = tok2.lexeme;
	temp_node.scope1 = scope1;
	temp_node.scope2 = scope2;
	storeList.push_back(temp_node);//Add to the store of assignments to print
}
void PrintScope(){
	for (int i = 0; i < storeList.size(); i++) {
		cout << storeList[i].scope1;
		if (storeList[i].scope1 != "::")
			cout << ".";
		cout << storeList[i].lhs << " = " << storeList[i].scope2;
		if (storeList[i].scope2 != "::")
			cout << ".";
		cout << storeList[i].rhs << endl;

	}
}
void DeleteScope(char* lexeme) {
	if (SymbolTablePointer == NULL) {
		return;
	}
	while (SymbolTableNew->item->scope == lexeme) {
		SymbolTableTemp->next = NULL;
		SymbolTableNew->prev = NULL;
		if (SymbolTableTemp->prev != NULL) {
			SymbolTableTemp = SymbolTableTemp->prev;
			SymbolTableNew = SymbolTableTemp->next;
			SymbolTableNew->next = NULL;
			return;
		}
		else if (SymbolTableTemp == SymbolTableNew) {
			SymbolTableTemp = NULL;
			SymbolTableNew = NULL;
			return;
		}
		else {
			SymbolTableTemp->next = NULL;
			SymbolTableNew->prev = NULL;
			SymbolTableNew = SymbolTableTemp;
			SymbolTableTemp->next = NULL;
		}
	}
	sScope = SymbolTableNew->item->scope;
}
void AddScope(char* lexeme) {
	if (SymbolTablePointer == NULL) {
		SymbolTablePointer = (sTable*)malloc(sizeof(sTable));
		struct sTableItem* newItem = (sTableItem*)malloc(sizeof(sTableItem));
		SymbolTablePointer->item = newItem;
		SymbolTablePointer->next = NULL;
		SymbolTablePointer->prev = NULL;

		int len = strlen(lexeme);
		SymbolTablePointer->item->name = new char[len + 1];
		strcpy(SymbolTablePointer->item->name, lexeme);
		SymbolTablePointer->item->name[len] = '\0';
		SymbolTablePointer->item->scope = sScope;
		SymbolTablePointer->item->permission = SymbolPermission;

		SymbolTableNew = SymbolTablePointer;
		SymbolTableTemp = SymbolTablePointer;
	}
	else {
		SymbolTableTemp = SymbolTablePointer;
		while (SymbolTableTemp->next != NULL) {
			SymbolTableTemp = SymbolTableTemp->next;
		}
		SymbolTableNew = (sTable*)malloc(sizeof(sTable));
		struct sTableItem* newItem = (sTableItem*)malloc(sizeof(sTableItem));
		SymbolTableNew->item = newItem;
		SymbolTableNew->next = NULL;
		SymbolTableNew->prev = SymbolTableTemp;
		SymbolTableTemp->next = SymbolTableNew;
		int len = strlen(lexeme);
		SymbolTableNew->item->name = new char[len + 1];
		strcpy(SymbolTableNew->item->name, lexeme);
		SymbolTableNew->item->name[len] = '\0';
		SymbolTableNew->item->scope = sScope;
		SymbolTableNew->item->permission = SymbolPermission;
	}
}

LexicalAnalyzer lexer;
Token token1, token2, token3, token4;


void syntax_error() {
	cout << "Syntax Error" << endl;
	exit(1);
}
int LexicalAnalyzer::Parse_Program() {
	token1 = lexer.GetToken();
	//cout/**/ << "Parse_Program"<<endl;
	if (token1.token_type != END_OF_FILE) {
		lexer.UngetToken(token1);
		Parse_Global_Vars();
		Parse_Scope();
	}
	return 0;
}
int LexicalAnalyzer::Parse_Global_Vars() {
	//cout/**/ << "Parse_Global_Vars Start" << endl;
	token1 = lexer.GetToken();
	token2 = lexer.GetToken();

	//cout/**/ << "Parse_Global_Vars token1:" << token1.lexeme.length() << endl;
	//cout/**/ << "Parse_Global_Vars token2:" << token2.lexeme << endl;
	if (token1.token_type == ID && (token2.token_type == SEMICOLON || token2.token_type == COMMA)) {
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
		Parse_Var_List();
		token3 = lexer.GetToken();//Semicolon
	}
	else {//global_vars --> epsilon
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
	}

	//cout/**/ << "Parse_Global_Vars Done" << endl;
	return 0;
}
int LexicalAnalyzer::Parse_Var_List() {
	//cout/**/ << "Parse_Var_List" << endl;
	token1 = lexer.GetToken();//ID
	token2 = lexer.GetToken();//COMMA?
		
	if (token1.token_type == ID) {
		//cout/**/ << "Parse_Var_List: got ID" << endl;

		char* lexeme = (char*)malloc(sizeof(token1.lexeme));
		memcpy(lexeme, (token1.lexeme).c_str(), sizeof(token1));
		AddScope(lexeme);
		sTable* temp1 = SymbolTablePointer;
		while (temp1 != NULL) {
			temp1 = temp1->next;
		}

		if (token2.token_type == COMMA) {
			//cout/**/ << "Parse_Var_List: got COMMA" << endl;
			Parse_Var_List();
		}
		else {
			lexer.UngetToken(token2);
		}
	}
	else {
		syntax_error();
	}
	//cout/**/ << "New Var:" << token1.lexeme<<endl;
	return 0;
}
int LexicalAnalyzer::Parse_Scope() {
	//cout/**/ << "Parse_Scope Start" << endl;
	token1 = lexer.GetToken();//ID
	//cout/**/ << "Parse_Scope SCOPE:"<< token1.lexeme << endl;
	token2 = lexer.GetToken();//LBRACE
	if(token1.token_type == ID && token2.token_type == LBRACE){
		string slexeme = token1.lexeme;
		sScope = const_cast<char*>(slexeme.c_str());//New scope with name of ID
		Parse_Public_Vars();
		Parse_Private_Vars();
		Parse_Stmt_List();
		token3 = lexer.GetToken();
		if (token3.token_type == RBRACE) {
			DeleteScope(sScope);//Close current scope
			token4 = lexer.GetToken();
			if (token4.token_type == END_OF_FILE) {
				DeleteScope(sScope);//Close Global Scope
			}
			else {
				UngetToken(token4);
			}
		}
		else {
			syntax_error();
		}
	}
	else {
		syntax_error();
	}
	//cout/**/ << "Parse_Scope End" << endl;
	return 0;
}
int LexicalAnalyzer::Parse_Public_Vars() {
	//cout/**/ << "Parse_Public_Vars Start" << endl;
	token1 = lexer.GetToken();//PUBLIC, PRIVATE, ID
	//cout/**/ << "Parse_Public_Vars get token:" << token1.lexeme << endl;
	if (token1.lexeme.compare("public")==0) {//IF PUBLIC keyword
		//cout/**/ << "Parse_Public_Vars Got PUBLIC" << endl;
		SymbolPermission = 1;
		token2 = lexer.GetToken();//COLON
		if (token2.token_type != COLON) syntax_error();
		//cout/**/ << "Parse_Public_Vars Got COLON" << endl;
		Parse_Var_List();
		token2 = lexer.GetToken();//SEMICOLON
		if (token2.token_type != SEMICOLON) syntax_error();
		//cout/**/ << "Parse_Public_Vars Got SEMICOLON" << endl;
	}
	else {
		lexer.UngetToken(token1);
	}
	//cout/**/ << "Parse_Public_Vars END" << endl;
	return 0;
}
int LexicalAnalyzer::Parse_Private_Vars() {
	//cout/**/ << "Parse_Private_Vars Start" << endl;
	token1 = lexer.GetToken();//PRIVATE, ID
	if (token1.lexeme.compare("private") == 0) {
		//cout/**/ << "Parse_Private_Vars Got Private" << endl;
		SymbolPermission = 2;
		token2 = lexer.GetToken();//COLON
		if (token2.token_type != COLON) syntax_error();
		//cout/**/ << "Parse_Private_Vars Got COLON" << endl;
		Parse_Var_List();
		token2 = lexer.GetToken();//SEMICOLON
		if (token2.token_type != SEMICOLON) syntax_error();
		//cout/**/ << "Parse_Private_Vars Got SEMICOLON" << endl;
	}
	else {
		lexer.UngetToken(token1);
	}
	return 0;
}
int LexicalAnalyzer::Parse_Stmt_List() {
	Parse_Stmt();
	token1 = lexer.GetToken();
	if (token1.token_type == RBRACE) {//stmt_list FOLLOW set
		lexer.UngetToken(token1);
	}
	else {
		lexer.UngetToken(token1);
		Parse_Stmt_List();
	}
	return 0;
}
int LexicalAnalyzer::Parse_Stmt() {
	token1 = lexer.GetToken();
	token2 = lexer.GetToken();

	if (token2.token_type == LBRACE) {
		sScope = const_cast<char*>((token1.lexeme).c_str());//New Scope with ID of token1
		lexer.UngetToken(token2);
		lexer.UngetToken(token1);
		Parse_Scope();
	}
	else {
		token3 = lexer.GetToken();
		token4 = lexer.GetToken();
		if (token1.token_type == ID && token2.token_type == EQUAL &&
			token3.token_type == ID && token4.token_type == SEMICOLON) {
			MakeAssignment(token1, token3);
		}
		else {
			syntax_error();
		}
	}
	return 0;
}

int main()
{
    /*LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }*/
	//cout/**/ << "START"<<endl;
	int a = lexer.Parse_Program();
	PrintScope();
}
