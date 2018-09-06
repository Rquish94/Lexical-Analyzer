//============================================================================
// Name        : RRR.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "stdafx.h"
#include iostream
#include fstream
#include cctype
using namespace std;

string fileName = "syntax.txt";
ifstream in_fp(fileName);
int nxtT;
int errorTotal = 0;
int charClass;
char lexe[100];
char nextChar;
int lexLen;
int token;

// Set to true to see an all error and lexile outputs
bool debuging = true;

void boolFact();
void boolTerm();
void boolExpr();
void ifStat();
void term();
void factor();
void expr();
void assign();
void getChar();
void addChar();
void NotBlank();
int lex(); //next token

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */ #define INT_LIT 10
#define IDENT 11
#define ADD_OP 21
#define SUB_OP 22
#define ASSIGN_OP 23
#define SEMI 24
#define T_STMT 25
#define F_STMT 26
#define NOT_STMT 27
#define N_STMT 28
#define MULT_OP 29
#define DIV_OP 30
#define LEFT_PARENTH 31 
#define RIGHT_PARENTH 32
#define MOD_OP 33
#define OR_STMT 34
#define IF_STMT 35
#define ELSE_STMT 36

//---------------------------------------------------------------------------------------------
// main program
void main()
{ // sysem output
cout << "1> the build has started \n";
cout << fileName;

if (in_fp.fail())
{// system error output
cout << "1> Error Could not find file \n";
cin.get();
exit(1);
}
else {
getChar();
lex(); // grabs the next token
do {
ifStat();	//Start by looking for an if statment
lex(); //Grabs the next token
} while (nxtT != EOF);
}
in_fp.close();
cout << endl;
if (errorTotal < 1) {
cout << "Successful build\n";
}
else {
cout << "ERROR: ";
cout << errorTotal << endl;
cout << "BUILD ERROR\n";
}
}

//----------------------------------------------------------------------
//operator initalization
int lookup(char ch) {
switch (ch) {
case '(':
addChar();
nxtT = LEFT_PARENTH;
break;
case ')':
addChar();
nxtT = RIGHT_PARENTH;
break;
case '*':
addChar();
nxtT = MULT_OP;
break;
case '/':
addChar();
nxtT = DIV_OP;
break;
case '%':
addChar();
nxtT = MOD_OP;
break;
case '=':
addChar();
nxtT = ASSIGN_OP;
break;
case ';':
addChar();
nxtT = SEMI;
break;
case '+':
addChar();
nxtT = ADD_OP;
break;
case '-':
addChar();
nxtT = SUB_OP;
break;
default:
addChar();
nxtT = EOF;
break;
}
return nxtT;
}
//--------------------------------------------------
// function which adds anohter char to the lexium
void addChar() {
if (lexLen <= 98) {
lexe[lexLen++] = nextChar;
lexe[lexLen] = 0;
}
else
cout << " Error - lexeme is too long \n";
}
//------------------------------------------------------------

// goes until finds another char
void NotBlank() {
while (isspace(nextChar))
getChar();
}
// Function to actually grab and add another char.
// sets limit and notifiys if your at the end of your file
//--------------------------------------------------
void getChar() {
in_fp.get(nextChar);
if (in_fp.eof()) // if no more character in the file
nextChar = EOF;

if (nextChar != EOF) {
if (isalpha(nextChar))
charClass = LETTER;
else if (isdigit(nextChar))
charClass = DIGIT;
else charClass = UNKNOWN;
}
else
charClass = EOF;
}

//----------------------------------------------------------------------------------
// Easiest way to implement lex for opperators that arnt just a symbol
int lex() {
lexLen = 0;
NotBlank();
switch (charClass) {
case LETTER:
addChar();
getChar();
while (charClass == LETTER || charClass == DIGIT) {
addChar();
getChar();
}
nxtT = IDENT;
if (lexe[0] == 'T' &&lexe[1] == 'R' &&lexe[2] == 'U' &&lexe[3] == 'E') {
nxtT = T_STMT;
}
else {
if (lexe[0] == 'F' &&lexe[1] == 'A' &&lexe[2] == 'L' && lexe[3] == 'S'&&lexe[4] == 'E') {
nxtT = F_STMT;
}
else
if (lexe[0] == 'A' &&lexe[1] == 'N' &&lexe[2] == 'D') {
nxtT = N_STMT;
}
else
if (lexe[0] == 'O' &&lexe[1] == 'R') {
nxtT = OR_STMT;
}
else
if (lexe[0] == 'i' &&lexe[1] == 'f') {
nxtT = IF_STMT;
}
else
if (lexe[0] == 'e' &&lexe[1] == 'l' &&lexe[2] == 's' && lexe[3] == 'e') {
nxtT = ELSE_STMT;
}
else
if (lexe[0] == 'N' &&lexe[1] == 'O' &&lexe[2] == 'T'){
nxtT = NOT_STMT;
}

}
break;
// sets for case digit
case DIGIT:
addChar();
getChar();
while (charClass == DIGIT) {
addChar();
getChar();
}
nxtT = INT_LIT;
break;
// if known draws next xhar
case UNKNOWN:
lookup(nextChar);
getChar();
break;
// lets the user know they are at the end of file. runs through to see if EOF are the characters used.
case EOF:
nxtT = EOF;
lexe[0] = 'E';
lexe[1] = 'O';
lexe[2] = 'F';
lexe[3] = 0;
break;
}
// Displays the next token for the user
if (debuging) {
cout << "Next token is: " << nxtT
<< " Next lexeme is " << lexe << "\n";
}
return nxtT;
}
void term() {
factor();
while (nxtT == MULT_OP || nxtT == MOD_OP ||
nxtT == DIV_OP)
{
lex(); //grabs the next token
factor();
}
}
void assign() {
// next token id then make sure assign is used to expr or the same but with semi colon then assign
if (nxtT == IDENT) {
lex(); //grabs the next token
if (nxtT == ASSIGN_OP) {
lex(); //grabs the next token
expr();
if (nxtT == SEMI) {
lex(); //grabs the next token
if (nxtT == IDENT) {
assign();
}
}
else {
if (debuging) { cout << "could'nt find error\n"; }
errorTotal++;
}
}

}
else {
// IF something else is thats not EOF throw error
if (nxtT != EOF) {
if (debuging) {

}
errorTotal++;
}
}

}
void expr()
{

term();
while (nxtT == ADD_OP ||xtT == SUB_OP) {
lex(); //grabs the next token
term();
}
}
void factor()
{ // directional operator for factor
if (nxtT == IDENT || nxtT == INT_LIT)
lex(); //grabs the next token
else if (nxtT == LEFT_PARENTH) {
lex(); //grabs the next token
expr();
if (nxtT == RIGHT_PARENTH)
lex(); //grabs the next token
else {
if (debuging) { cout << "R pair not found, sorry \n"; }
errorTotal++;
}
}
else {
if (debuging) { cout << "Factor Error\n"; }
errorTotal++;
}
}
void boolFact() {
// sets T/F and grabs next token
if (nxtT == T_STMT || nxtT == F_STMT) {
lex(); //grabs the next token
}
// grab next if not a statement
else if (nxtT == NOT_STMT) {
lex(); //grabs the next token
boolFact();
}
// If open paren look for a new Boolean Expresion
else if (nxtT == LEFT_PARENTH) {
lex(); //grabs the next token
boolExpr();
if (nxtT == RIGHT_PARENTH)
lex(); //grabs the next token
else {
if (debuging) { cout << "Missing pair \n"; }
errorTotal++;}
}
else { // just add another error if error
if (debuging) {cout << "Boolean Factor Error\n";}
errorTotal++;
}
}

void boolTerm() {
// Implementing and stmt
boolFact();
if (nxtT == N_STMT) {
lex(); //grabs the next token
boolTerm();
}
}
void ifStat() {
// checks for if and then parenth around a bool expr followed by assign. if not then choose else stmt
if (nxtT == IF_STMT) {
lex(); //grabs the next token
if (nxtT == LEFT_PARENTH) {
lex(); //grabs the next token
boolExpr();
if (nxtT == RIGHT_PARENTH) {
lex(); //grabs the next token
assign();
if (nxtT == ELSE_STMT) {
lex(); //grabs the next token
assign();
}
}
else {
if (debuging) { cout << "PARENTH ERROR \n"; }
errorTotal++; // Does not start with an if
}
}
else {
if (debuging) { cout << "PARENTH ERROR \n"; }
errorTotal++; // Does not start with an if
}
}
else {
if (debuging) { cout << " NEED IF /n"; }
errorTotal++;
}
}

void boolExpr() {
//implement or stmt
boolTerm();
if (nxtT == OR_STMT) {
lex(); //grabs the next token
boolExpr();}}

