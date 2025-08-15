#include "pch.h"
//#include "Parser.h"
//#include "pch.h"
//#include "Parser.h"
//
//Parser::Parser(const std::vector<CplToken>& tokens) : m_tokens(tokens) {}
//
//void Parser::parse() {
//    parse_program();
//    if (currentToken().type != CplTokenType::END_OF_FILE) {
//        throw SyntaxError("Unexpected tokens after end of program", currentToken().line);
//    }
//}
//
//// Rule: <برنامج> : برنامج <اسم برنامج> <فاصلة منقوطة> <كتلة برمجية> <نقطة>
//void Parser::parse_program() {
//    consume(CplTokenType::KEYWORD_PROGRAM, "Expected 'برنامج' keyword");
//    consume(CplTokenType::IDENTIFIER, "Expected program name");
//    consume(CplTokenType::SEMICOLON, "Expected ';' after program name");
//    parse_block();
//    consume(CplTokenType::DOT, "Expected '.' at the end of the program");
//}
//
//// Rule: <كتلة برمجية> : [ <جزء_التعريفات> ] <قائمة التعليمات>
//void Parser::parse_block() {
//    parse_definitions_part();
//    parse_statement_list();
//}
//
//// Rule: <جزء_التعريفات> : [ <تعريف المتغيرات> ]
//void Parser::parse_definitions_part() {
//    if (match(CplTokenType::KEYWORD_VAR)) {
//        parse_var_definitions();
//    }
//    // Add parsing for const, type, procedure definitions here
//}
//
//// Rule: <تعريف المتغيرات> : متغير <تعريف_متغير> (<تعريف_متغير>)*
//void Parser::parse_var_definitions() {
//    consume(CplTokenType::KEYWORD_VAR, "Expected 'متغير' keyword");
//    do {
//        // This is a simplified version of <مجموعة متغيرات>
//        consume(CplTokenType::IDENTIFIER, "Expected variable name");
//        while (match(CplTokenType::COMMA)) {
//            consume(CplTokenType::IDENTIFIER, "Expected variable name after comma");
//        }
//        consume(CplTokenType::COLON, "Expected ':' after variable names");
//        // Simplified data type check
//        if (currentToken().type == CplTokenType::TYPE_INTEGER || currentToken().type == CplTokenType::TYPE_REAL) {
//            advance();
//        }
//        else {
//            throw SyntaxError("Expected a data type (e.g., 'صحيح', 'حقيقي')", currentToken().line);
//        }
//        consume(CplTokenType::SEMICOLON, "Expected ';' after variable definition");
//    } while (currentToken().type == CplTokenType::IDENTIFIER);
//}
//
//// Rule: <قائمة تعليمات> : { <تعليمة> (؛ <تعليمة>)* }
//void Parser::parse_statement_list() {
//    consume(CplTokenType::L_BRACE, "Expected '{' to start a statement list");
//    parse_statement();
//    while (match(CplTokenType::SEMICOLON)) {
//        parse_statement();
//    }
//    consume(CplTokenType::R_BRACE, "Expected '}' to end a statement list");
//}
//
//// Rule: <تعليمة> : <تعليمة اسناد> | <تعليمة شرط> | ...
//void Parser::parse_statement() {
//    if (currentToken().type == CplTokenType::IDENTIFIER) {
//        parse_assignment_statement();
//    }
//    else if (currentToken().type == CplTokenType::KEYWORD_IF) {
//        parse_if_statement();
//    }
//    // Add cases for other statements (read, write, while, etc.)
//}
//
//// Rule: <تعليمة اسناد> : <متغير> = <تعبير>
//void Parser::parse_assignment_statement() {
//    consume(CplTokenType::IDENTIFIER, "Expected identifier for assignment");
//    consume(CplTokenType::ASSIGN, "Expected '=' for assignment");
//    parse_expression();
//}
//
//// Rule: <تعليمة اذا> : اذا ( <شرط> ) فان <تعليمة> [ والا <تعليمة> ]
//void Parser::parse_if_statement() {
//    consume(CplTokenType::KEYWORD_IF, "Expected 'اذا'");
//    consume(CplTokenType::L_PAREN, "Expected '(' after 'اذا'");
//    parse_expression(); // The condition is an expression
//    consume(CplTokenType::R_PAREN, "Expected ')' after condition");
//    consume(CplTokenType::KEYWORD_THEN, "Expected 'فان' after condition");
//    parse_statement();
//    if (match(CplTokenType::KEYWORD_ELSE)) {
//        parse_statement();
//    }
//}
//
//// --- Expression Parsing (Simplified) ---
//void Parser::parse_expression() {
//    parse_simple_expression();
//    if (currentToken().type == CplTokenType::GREATER || currentToken().type == CplTokenType::LESS ||
//        currentToken().type == CplTokenType::EQUAL) {
//        advance();
//        parse_simple_expression();
//    }
//}
//
//void Parser::parse_simple_expression() {
//    parse_term();
//    while (currentToken().type == CplTokenType::PLUS || currentToken().type == CplTokenType::MINUS) {
//        advance();
//        parse_term();
//    }
//}
//
//void Parser::parse_term() {
//    parse_factor();
//    while (currentToken().type == CplTokenType::MULTIPLY || currentToken().type == CplTokenType::DIVIDE) {
//        advance();
//        parse_factor();
//    }
//}
//
//void Parser::parse_factor() {
//    if (currentToken().type == CplTokenType::IDENTIFIER ||
//        currentToken().type == CplTokenType::INTEGER_LITERAL ||
//        currentToken().type == CplTokenType::REAL_LITERAL) {
//        advance();
//    }
//    else if (match(CplTokenType::L_PAREN)) {
//        parse_expression();
//        consume(CplTokenType::R_PAREN, "Expected ')' to close expression");
//    }
//    else {
//        throw SyntaxError("Unexpected token in expression", currentToken().line);
//    }
//}
//
//// --- Helper Method Implementations ---
//const CplToken& Parser::currentToken() const {
//    if (m_current_pos >= m_tokens.size()) {
//        // Return END_OF_FILE token if out of bounds
//        static CplToken eof{ CplTokenType::END_OF_FILE, L"", -1 };
//        return eof;
//    }
//    return m_tokens[m_current_pos];
//}
//
//void Parser::advance() {
//    if (m_current_pos < m_tokens.size()) {
//        m_current_pos++;
//    }
//}
//
//bool Parser::match(CplTokenType type) {
//    if (currentToken().type == type) {
//        advance();
//        return true;
//    }
//    return false;
//}
//
//void Parser::consume(CplTokenType type, const std::string& error_message) {
//    if (currentToken().type == type) {
//        advance();
//    }
//    else {
//        throw SyntaxError(error_message, currentToken().line);
//    }
//}
#pragma once