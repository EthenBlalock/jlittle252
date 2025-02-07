#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>
// Hi my name is Ethen
// Token struct to hold the lexeme and its type
// yooooooooo
// COMP 360
// change
struct Token {
    std::string lexeme;
    std::string type;
};

// Function to check if a character is a delimiter
bool isDelimiter(char ch) {
    return isspace(ch) || ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}';
}

// Function to check if a character is an operator
bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>';
}

// Function to identify the type of lexeme
std::string getTokenType(const std::string& lexeme) {
    static std::unordered_map<std::string, std::string> tokenTypes = {
        {"if", "KEYWORD"}, {"else", "KEYWORD"}, {"for", "KEYWORD"},
        {"while", "KEYWORD"}, {"int", "KEYWORD"}, {"float", "KEYWORD"},
        {"double", "KEYWORD"}, {"char", "KEYWORD"}
    };
    if (tokenTypes.count(lexeme)) return tokenTypes[lexeme];
    if (isdigit(lexeme[0])) return "NUMBER";
    if (isalpha(lexeme[0])) return "IDENTIFIER";
    return "UNKNOWN";
}

// Function to perform lexical analysis
std::vector<Token> lexicalAnalysis(const std::string& sourceCode) {
    std::vector<Token> tokens;
    std::string lexeme;
    for (char ch : sourceCode) {
        if (isDelimiter(ch) || isOperator(ch)) {
            if (!lexeme.empty()) {
                tokens.push_back({lexeme, getTokenType(lexeme)});
                lexeme.clear();
            }
            if (isOperator(ch)) {
                tokens.push_back({std::string(1, ch), "OPERATOR"});
            }
        } else {
            lexeme += ch;
        }
    }
    if (!lexeme.empty()) {
        tokens.push_back({lexeme, getTokenType(lexeme)});
    }
    return tokens;
}

int main() {
    // Read the source code from a file
    std::ifstream file("source_code.txt");
    std::string sourceCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Perform lexical analysis
    std::vector<Token> tokens = lexicalAnalysis(sourceCode);
    
    // Display tokens
    for (const Token& token : tokens) {
        std::cout << "Lexeme: " << token.lexeme << ", Token Type: " << token.type << std::endl;
    }

    return 0;
}
