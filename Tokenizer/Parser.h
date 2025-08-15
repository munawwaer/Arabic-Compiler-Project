#pragma once
//#include "Tokenizer.h"
//#include <vector>
//#include <stdexcept>
//
//// A simple exception class for syntax errors.
//class SyntaxError : public std::runtime_error {
//public:
//    SyntaxError(const std::string& message, int line)
//        : std::runtime_error(message + " at line " + std::to_string(line)) {}
//};
//
//class Parser {
//public:
//    Parser(const std::vector<CplToken>& tokens);
//    void parse(); // Main entry point for parsing
//
//private:
//    const std::vector<CplToken>& m_tokens;
//    size_t m_current_pos = 0;
//
//    // Helper methods for token manipulation
//    const CplToken& currentToken() const;
//    const CplToken& peekToken(int offset = 1) const;
//    void advance();
//    bool match(CplTokenType type);
//    void consume(CplTokenType type, const std::string& error_message);
//
//    // Methods corresponding to grammar rules (Non-terminals)
//    void parse_program();
//    void parse_block();
//    void parse_definitions_part();
//    void parse_var_definitions();
//    void parse_statement_list();
//    void parse_statement();
//    void parse_assignment_statement();
//    void parse_if_statement();
//    void parse_expression();
//    void parse_simple_expression();
//    void parse_term();
//    void parse_factor();
//};
