//#pragma once
//
//#include <string>
//#include <vector>
//#include <stdexcept>
//#include <unordered_map>
//
//#ifdef TOKENIZER_EXPORTS
//#define TOKENIZER_API __declspec(dllexport)
//#else
//#define TOKENIZER_API __declspec(dllimport)
//#endif
//
// A comprehensive enum of all token types derived from the grammar document.
//enum class CplTokenType {
//     Keywords from the grammar file
//    KEYWORD_PROGRAM, KEYWORD_CONST, KEYWORD_TYPE, KEYWORD_VAR,
//    KEYWORD_PROCEDURE, KEYWORD_ARRAY, KEYWORD_RECORD, KEYWORD_OF,
//    KEYWORD_READ, KEYWORD_WRITE, KEYWORD_IF, KEYWORD_THEN, KEYWORD_ELSE,
//    KEYWORD_FOR, KEYWORD_TO, KEYWORD_WHILE, KEYWORD_REPEAT, KEYWORD_UNTIL,
//    KEYWORD_BY_VALUE, KEYWORD_BY_REF,
//
//     Data Types recognized as special tokens
//    TYPE_INTEGER, TYPE_REAL, TYPE_BOOLEAN, TYPE_CHAR, TYPE_STRING,
//
//     General Tokens
//    IDENTIFIER,
//    INTEGER_LITERAL,
//    REAL_LITERAL,
//    STRING_LITERAL,
//
//     Operators
//    ASSIGN, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
//    EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
//    LOGICAL_AND, LOGICAL_OR, LOG_NOT,
//
//     Punctuation
//    L_PAREN, R_PAREN, L_BRACE, R_BRACE, L_BRACKET, R_BRACKET,
//    SEMICOLON, COMMA, COLON, DOT,
//
//     End of File & Unknown
//    END_OF_FILE,
//    UNKNOWN
//};
//
// The unified structure for a token.
//struct CplToken {
//    CplTokenType type;
//    std::wstring value;
//    int line;
//};
//
// Forward declaration of the Parser class
//class Parser;
//
// Class responsible for lexical analysis (source text to tokens)
//class Tokenizer {
//public:
//    Tokenizer(const std::wstring& source);
//    std::vector<CplToken> tokenizeAll();
//
//private:
//     Implementation details...
//    std::wstring m_source;
//    size_t m_current_pos = 0;
//    int m_current_line = 1;
//    std::unordered_map<std::wstring, CplTokenType> m_keywords;
//
//    void initializeKeywords();
//    CplToken getNextToken();
//    void skipWhitespaceAndComments();
//    CplToken readNumber();
//    CplToken readStringLiteral();
//    CplToken readIdentifierOrKeyword();
//    wchar_t peek(int offset = 0) const;
//    wchar_t advance();
//    bool isAtEnd() const;
//};
//
// A simple exception class for syntax errors.
//class SyntaxError : public std::runtime_error {
//public:
//    SyntaxError(const std::string& message, int line)
//        : std::runtime_error(message + " at line " + std::to_string(line)) {}
//};
//
// Class responsible for syntax analysis (tokens to syntax validation)
//class Parser {
//public:
//    Parser(const std::vector<CplToken>& tokens);
//    void parse(); // Main entry point for parsing
//
//private:
//    const std::vector<CplToken>& m_tokens;
//    size_t m_current_pos = 0;
//
//     Helper methods for token manipulation
//    const CplToken& currentToken() const;
//    void advance();
//    void consume(CplTokenType type, const std::string& error_message);
//
//     Methods corresponding to grammar rules (Non-terminals)
//    void parse_program();
//    void parse_block();
//    void parse_statement_list();
//    void parse_statement();
//    void parse_assignment_statement();
//    void parse_expression();
//    void parse_term();
//    void parse_factor();
//};
//
// The C-style function that will be the single entry point from C#.
//extern "C" {
//    TOKENIZER_API const wchar_t* analyze_syntax_w(const wchar_t* input);
//}


#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_map>

#ifdef TOKENIZER_EXPORTS
#define TOKENIZER_API __declspec(dllexport)
#else
#define TOKENIZER_API __declspec(dllimport)
#endif

// A comprehensive enum of all token types derived from the grammar document.
enum class CplTokenType {
    // Keywords from the grammar file
    KEYWORD_PROGRAM, KEYWORD_CONST, KEYWORD_TYPE, KEYWORD_VAR,
    KEYWORD_PROCEDURE, KEYWORD_ARRAY, KEYWORD_RECORD, KEYWORD_OF,
    KEYWORD_READ, KEYWORD_WRITE, KEYWORD_IF, KEYWORD_THEN, KEYWORD_ELSE,
    KEYWORD_FOR, KEYWORD_TO, KEYWORD_WHILE, KEYWORD_REPEAT, KEYWORD_UNTIL,
    KEYWORD_BY_VALUE, KEYWORD_BY_REF, TYPE_VOID,

    // New keywords for functions
    KEYWORD_FUNCTION,   // دالة
    KEYWORD_RETURN,     // ارجع

    // Data Types recognized as special tokens
    TYPE_INTEGER, TYPE_REAL, TYPE_BOOLEAN, TYPE_CHAR, TYPE_STRING,

    // General Tokens
    IDENTIFIER,
    INTEGER_LITERAL,
    REAL_LITERAL,
    STRING_LITERAL,

    // Operators
    ASSIGN, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
    EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
    LOGICAL_AND, LOGICAL_OR, LOG_NOT,

    // Punctuation
    L_PAREN, R_PAREN, L_BRACE, R_BRACE, L_BRACKET, R_BRACKET,
    SEMICOLON, COMMA, COLON, DOT,

    // End of File & Unknown
    END_OF_FILE,
    UNKNOWN
};

// The unified structure for a token.
struct CplToken {
    CplTokenType type;
    std::wstring value;
    int line;
};

// Forward declaration of the Parser class
class Parser;

// Class responsible for lexical analysis (source text to tokens)
class Tokenizer {
public:
    Tokenizer(const std::wstring& source);
    std::vector<CplToken> tokenizeAll();

private:
    std::wstring m_source;
    size_t m_current_pos = 0;
    int m_current_line = 1;
    std::unordered_map<std::wstring, CplTokenType> m_keywords;

    void initializeKeywords();
    CplToken getNextToken();
    void skipWhitespaceAndComments();
    CplToken readNumber();
    CplToken readStringLiteral();
    CplToken readIdentifierOrKeyword();
    wchar_t peek(int offset = 0) const;
    wchar_t advance();
    bool isAtEnd() const;
};

// A simple exception class for syntax errors.
class SyntaxError : public std::runtime_error {
public:
    SyntaxError(const std::string& message, int line)
        : std::runtime_error(message + " at line " + std::to_string(line)) {}
};

// Class responsible for syntax analysis (tokens to syntax validation)
class Parser {
public:
    Parser(const std::vector<CplToken>& tokens);
    void parse(); // Main entry point for parsing

private:
    const std::vector<CplToken>& m_tokens;
    size_t m_current_pos = 0;

    // Helper methods for token manipulation
    const CplToken& currentToken() const;
    const CplToken& lookahead(size_t k = 1) const;
    void advance();
    void consume(CplTokenType type, const std::string& error_message);

    // Utility
    bool isBuiltinType(CplTokenType t) const;
    void parse_type();

    // Grammar rules (Non-terminals)
    void parse_program();
    void parse_block();
    void parse_statement_list();
    void parse_statement();
    void parse_assignment_statement();
    void parse_expression();
    void parse_term();
    void parse_factor();

    // Functions
    void parse_function_declaration();
    void parse_parameter_list();
    void parse_return_statement();

    // الاجراء
    void parse_procedure_declaration();

    // Variables
    void parse_var_declaration();

    // Calls
    void parse_function_call_core();     // call without trailing ';' (for expressions)
    void parse_call_statement();         // call + ';'
};

// The C-style function that will be the single entry point from C#.
extern "C" {
    TOKENIZER_API const wchar_t* analyze_syntax_w(const wchar_t* input);
}