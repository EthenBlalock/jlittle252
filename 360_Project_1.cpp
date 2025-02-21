// Jonathan Little, Ethen Blalock
// February 21st, 2025
// COMP 360
// Project 1, Lexical Analyzer and Recursive Decent Parser

#include <iostream>
#include <cctype>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cstring>
using namespace std;

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
const int END_OF_FILE = EOF; // Using a named constant for EOF
string UserFileName;
ifstream file;

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Token codes for operators and other symbols
#define INT_LIT 10
#define IDENT 11
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define semicolon 29
#define EQOPP 30
#define GT 31
#define LEFT_BRACE 40
#define RIGHT_BRACE 41
#define GE_OP 44
#define FLOAT_KEYWORD 50
#define WHILE_KEYWORD 51

// Token name mapping
unordered_map<int, string> tokenNames;

// Function to initialize the token names
void initializeTokenNames() {
    tokenNames[INT_LIT] = "INT_LIT";
    tokenNames[IDENT] = "IDENT";
    tokenNames[ADD_OP] = "ADD_OP";
    tokenNames[SUB_OP] = "SUB_OP";
    tokenNames[MULT_OP] = "MULT_OP";
    tokenNames[DIV_OP] = "DIV_OP";
    tokenNames[LEFT_PAREN] = "LEFT_PAREN";
    tokenNames[RIGHT_PAREN] = "RIGHT_PAREN";
    tokenNames[semicolon] = "semicolon";
    tokenNames[EQOPP] = "EQOPP";
    tokenNames[GT] = "GT";
    tokenNames[LEFT_BRACE] = "LEFT_BRACE";
    tokenNames[RIGHT_BRACE] = "RIGHT_BRACE";
    tokenNames[FLOAT_KEYWORD] = "KEYWORD";
    tokenNames[WHILE_KEYWORD] = "KEYWORD";
    tokenNames[GE_OP] = "GE_OP";
}

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lookup(char ch);
int lex();
void error(const string &msg);

// --- New match helper that prints an error if the expected token is not found.
void match(int expectedToken, const string &expectedTokenName) {
    if (nextToken == expectedToken) {
        lex();
    } else {
        // When an error occurs, output a detailed message:
        error("Expected token '" + expectedTokenName + "' but found '" + tokenNames[nextToken] + "' with lexeme \"" + lexeme + "\"");
    }
}

/* Parser function declarations for the BNF grammar */
void program();
void declares();
void stmts();
void assign();
void loopStmt();
void expr();  // extended expression production
void keyword();
void ident();

/* main driver */
int main() {
    initializeTokenNames();

    std::cout << "Enter File Name containing source code include .txt" << endl;
    std::cin >> UserFileName; 

    file.open(UserFileName);

    if (!file) {
        cout << "ERROR - cannot open file" << endl;
        return 1;
    } else {
        // Prime the lexing process
        getChar();
        lex();
        
        // Attempt to parse the program according to the grammar.
        program();
        
        // If parsing has consumed the entire file successfully...
        if (nextToken == END_OF_FILE) {
            cout << "The Source Code is generated by the BNF grammar" << endl;
        } else {
            error("Extra tokens after a valid program");
        }
    }
    file.close();
    return 0;
}

// --- Updated error function that immediately displays the error message and exits.
void error(const string &msg) {
    cout << "The Source Code cannot be generated by the Sample Function defined language, and the error is: " << msg << endl;
    exit(1);
}

// Function to add nextChar to lexeme
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        cerr << "Error - lexeme is too long" << endl;
    }
}

void getChar() {
    if (file.get(nextChar)) {
        if (isalpha(nextChar))
            charClass = 0; // LETTER
        else if (isdigit(nextChar))
            charClass = 1; // DIGIT
        else
            charClass = 99; // UNKNOWN
    } else {
        charClass = END_OF_FILE;
    }
}

// Function to call getChar until it returns a non-whitespace character
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/* lookup - a function to lookup operators and parentheses
   and return the token */
int lookup(char ch) {
    switch (ch) {
        case '(': 
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '{':
            addChar();
            nextToken = LEFT_BRACE;
            break;
        case '}':
            addChar();
            nextToken = RIGHT_BRACE;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case ';':
            addChar();
            nextToken = semicolon;
            break;
        case '=':
            addChar();
            nextToken = EQOPP;
            break;
        case '>':
            addChar();  // add '>'
        // Look ahead for '=' to form '>='
            if (file.peek() == '=') {
                getChar();   // consume '='
                addChar();   // add '=' to lexeme
                nextToken = GE_OP;
            } else {
                nextToken = GT;
            }
            break;
        default:
            addChar();
            nextToken = UNKNOWN;
            break;
    }
    return nextToken;
}

// Lexical analyzer function
int lex() {
    lexLen = 0;
    getNonBlank();
    
    if (charClass == 0) { // LETTER
        // Build up an identifier or keyword
        addChar();
        getChar();
        while (charClass == 0 || charClass == 1) {
            addChar();
            getChar();
        }
        // Check if lexeme is a keyword: "float" or "while"
        string lexStr(lexeme);
        if (lexStr == "float") {
            nextToken = FLOAT_KEYWORD;
        } else if (lexStr == "while") {
            nextToken = WHILE_KEYWORD;
        } else {
            nextToken = IDENT;
        }
    } else if (charClass == 1) { // DIGIT
        addChar();
        getChar();
        while (charClass == 1) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
    } else if (charClass == 99) { // UNKNOWN
        lookup(nextChar);
        getChar();
    } else if (charClass == END_OF_FILE) {
        nextToken = END_OF_FILE;
        strcpy(lexeme, "EOF");
    }
    
    cout << "Token: " << tokenNames[nextToken] << ", Lexeme: " << lexeme << endl;
    return nextToken;
}

// Parser functions

// <program> -> <keyword> <ident> () { <declares> <stmts> <loop> <stmts> }
void program() {
    
    // The first keyword must be 'float'
    if (nextToken == FLOAT_KEYWORD)
        keyword();
    else
        error("Program should start with the keyword 'float'");
    
    ident(); // Function name
    
    // Match the function parameter list: ()
    match(LEFT_PAREN, "LEFT_PAREN");
    match(RIGHT_PAREN, "RIGHT_PAREN");
    
    // Match the left brace {
    match(LEFT_BRACE, "{");
    
    // Parse declarations
    declares();
    
    // Parse statements
    stmts();
    
    // Parse loop statement
    loopStmt();
    
    // Parse statements after loop
    stmts();
    
    // Match the right brace }
    match(RIGHT_BRACE, "}");
}

// <declares> -> <keyword> <ident> ; | <keyword> <ident> ; <declares>
void declares() {
    
    if (nextToken == FLOAT_KEYWORD) {
        keyword();
        ident();
        match(semicolon, ";");
        // Check if more declarations exist
        while (nextToken == FLOAT_KEYWORD) {
            keyword();
            ident();
            match(semicolon, ";");
        }
    } else {
        error("Expected declaration starting with 'float'");
    }
}

// <stmts> -> <assign> ; <stmts> | <assign> ;
void stmts() {
    
    assign();
    match(semicolon, ";");
    while (nextToken == IDENT) {
        assign();
        match(semicolon, ";");
    }
}

// <assign> -> <ident> = <expr>
void assign() {
    
    ident();
    match(EQOPP, "=");
    expr();
}

// <expr> -> <ident> {*|/} <expr> | <ident>
void expr() {
    
    ident();
    // Check for multiplication or division operator
    if (nextToken == MULT_OP || nextToken == DIV_OP) {
        int op = nextToken;
        lex(); 
        expr();
    }
}

// <loop> -> <keyword> ( <ident> >= 10 )
void loopStmt() {
    
    if (nextToken == WHILE_KEYWORD)
        keyword();
    else
        error("Loop should start with the keyword 'while'");
    
    match(LEFT_PAREN, "LEFT_PAREN");
    ident();

    // Match compound operator >= 
    if (nextToken == GE_OP)
        lex();
    else
        error("Expected '>=' in loop condition");

    // Expect an integer literal
    if (nextToken == INT_LIT)
        lex();
    else
        error("Expected integer literal in loop condition");
    
    match(RIGHT_PAREN, "RIGHT_PAREN");
}

// <keyword> -> float | while
void keyword() {
    
    if (nextToken == FLOAT_KEYWORD || nextToken == WHILE_KEYWORD)
        lex();
    else
        error("Expected a keyword 'float' or 'while'");
}

// <ident> -> handled by the lexer (IDENT token)
void ident() {
    
    if (nextToken == IDENT)
        lex();
    else
        error("Expected an identifier");
}