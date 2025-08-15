#include "pch.h"
#include "Tokenizer.h"
#include <sstream>
#include <cctype>
#include <unordered_map>
#include <combaseapi.h>

// =================================================================================
// SECTION 1: LEXICAL ANALYZER (TOKENIZER) IMPLEMENTATION
// =================================================================================

Tokenizer::Tokenizer(const std::wstring& source) : m_source(source) {
    initializeKeywords();
}

void Tokenizer::initializeKeywords() {
    m_keywords[L"برنامج"] = CplTokenType::KEYWORD_PROGRAM;
    m_keywords[L"ثابت"] = CplTokenType::KEYWORD_CONST;
    m_keywords[L"نوع"] = CplTokenType::KEYWORD_TYPE;
    m_keywords[L"متغير"] = CplTokenType::KEYWORD_VAR;
    m_keywords[L"اجراء"] = CplTokenType::KEYWORD_PROCEDURE;
    m_keywords[L"اقرا"] = CplTokenType::KEYWORD_READ;
    m_keywords[L"اطبع"] = CplTokenType::KEYWORD_WRITE;
    m_keywords[L"اذا"] = CplTokenType::KEYWORD_IF;
    m_keywords[L"فان"] = CplTokenType::KEYWORD_THEN;
    m_keywords[L"والا"] = CplTokenType::KEYWORD_ELSE;
    m_keywords[L"صحيح"] = CplTokenType::TYPE_INTEGER;
    m_keywords[L"حقيقي"] = CplTokenType::TYPE_REAL;
}

std::vector<CplToken> Tokenizer::tokenizeAll() {
    std::vector<CplToken> tokens;
    while (!isAtEnd()) {
        tokens.push_back(getNextToken());
    }
    tokens.push_back({ CplTokenType::END_OF_FILE, L"EOF", m_current_line });
    return tokens;
}

CplToken Tokenizer::getNextToken() {
    skipWhitespaceAndComments();
    if (isAtEnd()) return { CplTokenType::END_OF_FILE, L"", m_current_line };

    wchar_t c = peek();

    if (iswalpha(c) || c == L'_') return readIdentifierOrKeyword();
    if (iswdigit(c)) return readNumber();
    if (c == L'"') return readStringLiteral();

    // Two-character operators
    if (peek(1) != L'\0') {
        std::wstring two_char_op;
        two_char_op += c;
        two_char_op += peek(1);
        if (two_char_op == L"==") { advance(); advance(); return { CplTokenType::EQUAL, two_char_op, m_current_line }; }
        if (two_char_op == L"!=") { advance(); advance(); return { CplTokenType::NOT_EQUAL, two_char_op, m_current_line }; }
        if (two_char_op == L">=") { advance(); advance(); return { CplTokenType::GREATER_EQUAL, two_char_op, m_current_line }; }
        if (two_char_op == L"<=") { advance(); advance(); return { CplTokenType::LESS_EQUAL, two_char_op, m_current_line }; }
        if (two_char_op == L"&&") { advance(); advance(); return { CplTokenType::LOGICAL_AND, two_char_op, m_current_line }; }
        if (two_char_op == L"||") { advance(); advance(); return { CplTokenType::LOGICAL_OR, two_char_op, m_current_line }; }
    }

    // Single-character operators and punctuation
    advance();
    switch (c) {
    case L'=': return { CplTokenType::ASSIGN, L"=", m_current_line };
    case L'+': return { CplTokenType::PLUS, L"+", m_current_line };
    case L'-': return { CplTokenType::MINUS, L"-", m_current_line };
    case L'*': return { CplTokenType::MULTIPLY, L"*", m_current_line };
    case L'/': return { CplTokenType::DIVIDE, L"/", m_current_line };
    case L'%': return { CplTokenType::MODULO, L"%", m_current_line };
    case L'>': return { CplTokenType::GREATER, L">", m_current_line };
    case L'<': return { CplTokenType::LESS, L"<", m_current_line };
    case L'!': return { CplTokenType::LOG_NOT, L"!", m_current_line };
    case L'(': return { CplTokenType::L_PAREN, L"(", m_current_line };
    case L')': return { CplTokenType::R_PAREN, L")", m_current_line };
    case L'{': return { CplTokenType::L_BRACE, L"{", m_current_line };
    case L'}': return { CplTokenType::R_BRACE, L"}", m_current_line };
    case L'[': return { CplTokenType::L_BRACKET, L"[", m_current_line };
    case L']': return { CplTokenType::R_BRACKET, L"]", m_current_line };
    case L';': return { CplTokenType::SEMICOLON, L";", m_current_line };
    case L',': return { CplTokenType::COMMA, L",", m_current_line };
    case L':': return { CplTokenType::COLON, L":", m_current_line };
    case L'.': return { CplTokenType::DOT, L".", m_current_line };
    default:  return { CplTokenType::UNKNOWN, std::wstring(1, c), m_current_line };
    }
}

void Tokenizer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        if (iswspace(peek())) {
            if (peek() == L'\n') m_current_line++;
            advance();
        }
        else if (peek() == L'/' && peek(1) == L'/') {
            while (!isAtEnd() && peek() != L'\n') advance();
        }
        else if (peek() == L'/' && peek(1) == L'*') {
            advance(); advance();
            while (!isAtEnd() && !(peek() == L'*' && peek(1) == L'/')) {
                if (peek() == L'\n') m_current_line++;
                advance();
            }
            if (!isAtEnd()) advance();
            if (!isAtEnd()) advance();
        }
        else {
            break;
        }
    }
}

CplToken Tokenizer::readNumber() {
    std::wstring number_str;
    bool is_float = false;
    while (!isAtEnd() && iswdigit(peek())) {
        number_str += advance();
    }
    if (peek() == L'.' && iswdigit(peek(1))) {
        is_float = true;
        number_str += advance();
        while (!isAtEnd() && iswdigit(peek())) {
            number_str += advance();
        }
    }
    return { is_float ? CplTokenType::REAL_LITERAL : CplTokenType::INTEGER_LITERAL, number_str, m_current_line };
}

CplToken Tokenizer::readStringLiteral() {
    std::wstring str;
    advance(); // Consume opening quote
    while (!isAtEnd() && peek() != L'"') {
        str += advance();
    }
    if (isAtEnd()) { // Unterminated string
        return { CplTokenType::UNKNOWN, str, m_current_line };
    }
    advance(); // Consume closing quote
    return { CplTokenType::STRING_LITERAL, str, m_current_line };
}

CplToken Tokenizer::readIdentifierOrKeyword() {
    std::wstring text;
    while (!isAtEnd() && (iswalnum(peek()) || peek() == L'_')) {
        text += advance();
    }
    auto it = m_keywords.find(text);
    if (it != m_keywords.end()) {
        return { it->second, text, m_current_line };
    }
    return { CplTokenType::IDENTIFIER, text, m_current_line };
}

wchar_t Tokenizer::peek(int offset) const {
    if (m_current_pos + offset >= m_source.length()) return L'\0';
    return m_source[m_current_pos + offset];
}

wchar_t Tokenizer::advance() {
    return m_source[m_current_pos++];
}

bool Tokenizer::isAtEnd() const {
    return m_current_pos >= m_source.length();
}

// =================================================================================
// SECTION 2: SYNTAX ANALYZER (PARSER) IMPLEMENTATION
// =================================================================================

Parser::Parser(const std::vector<CplToken>& tokens) : m_tokens(tokens) {}

void Parser::parse() {
    parse_program();
}

void Parser::parse_program() {
    consume(CplTokenType::KEYWORD_PROGRAM, "Expected 'برنامج' keyword");
    consume(CplTokenType::IDENTIFIER, "Expected program name");
    consume(CplTokenType::SEMICOLON, "Expected ';' after program name");
    parse_block();
    consume(CplTokenType::DOT, "Expected '.' at the end of the program");
}

void Parser::parse_block() {
    // Simplified implementation for now
    parse_statement_list();
}

void Parser::parse_statement_list() {
    consume(CplTokenType::L_BRACE, "Expected '{' to start a statement list");
    while (currentToken().type != CplTokenType::R_BRACE && currentToken().type != CplTokenType::END_OF_FILE) {
        parse_statement();
    }
    consume(CplTokenType::R_BRACE, "Expected '}' to end a statement list");
}

void Parser::parse_statement() {
    if (currentToken().type == CplTokenType::IDENTIFIER) {
        parse_assignment_statement();
        consume(CplTokenType::SEMICOLON, "Expected ';' after statement");
    }
    else {
        throw SyntaxError("Invalid statement", currentToken().line);
    }
}

void Parser::parse_assignment_statement() {
    consume(CplTokenType::IDENTIFIER, "Expected identifier");
    consume(CplTokenType::ASSIGN, "Expected '='");
    parse_expression();
}

void Parser::parse_expression() {
    parse_term();
    while (currentToken().type == CplTokenType::PLUS || currentToken().type == CplTokenType::MINUS) {
        advance();
        parse_term();
    }
}

void Parser::parse_term() {
    parse_factor();
    while (currentToken().type == CplTokenType::MULTIPLY || currentToken().type == CplTokenType::DIVIDE) {
        advance();
        parse_factor();
    }
}

void Parser::parse_factor() {
    if (currentToken().type == CplTokenType::IDENTIFIER || currentToken().type == CplTokenType::INTEGER_LITERAL) {
        advance();
    }
    else if (currentToken().type == CplTokenType::L_PAREN) {
        advance();
        parse_expression();
        consume(CplTokenType::R_PAREN, "Expected ')'");
    }
    else {
        throw SyntaxError("Invalid factor in expression", currentToken().line);
    }
}

const CplToken& Parser::currentToken() const {
    return m_tokens[m_current_pos];
}

void Parser::advance() {
    if (m_current_pos < m_tokens.size() - 1) { // Prevent advancing past EOF
        m_current_pos++;
    }
}

void Parser::consume(CplTokenType type, const std::string& error_message) {
    if (currentToken().type == type) {
        advance();
    }
    else {
        throw SyntaxError(error_message, currentToken().line);
    }
}

// =================================================================================
// SECTION 3: EXPORTED DLL INTERFACE
// =================================================================================

extern "C" TOKENIZER_API const wchar_t* analyze_syntax_w(const wchar_t* input) {
    static std::wstring result_str;
    try {
        Tokenizer tokenizer(input);
        std::vector<CplToken> tokens = tokenizer.tokenizeAll();

        Parser parser(tokens);
        parser.parse();

        result_str = L"التحليل النحوي صحيح. (Syntax is correct.)";
    }
    catch (const SyntaxError& e) {
        std::string error_msg = e.what();
        result_str = std::wstring(error_msg.begin(), error_msg.end());
    }
    catch (const std::exception& e) {
        std::string error_msg = e.what();
        result_str = L"خطأ عام: " + std::wstring(error_msg.begin(), error_msg.end());
    }

    size_t sizeInBytes = (result_str.length() + 1) * sizeof(wchar_t);
    wchar_t* result = (wchar_t*)::CoTaskMemAlloc(sizeInBytes);
    if (result) {
        wcscpy_s(result, result_str.length() + 1, result_str.c_str());
    }
    return result;
}
