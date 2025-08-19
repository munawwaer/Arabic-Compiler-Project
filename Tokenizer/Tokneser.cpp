//#include "pch.h"
//#include "Tokenizer.h"
//#include <sstream>
//#include <cctype>
//#include <unordered_map>
//#include <combaseapi.h>
//
//// =================================================================================
//// SECTION 1: LEXICAL ANALYZER (TOKENIZER) IMPLEMENTATION
//// =================================================================================
//
//Tokenizer::Tokenizer(const std::wstring& source) : m_source(source) {
//    initializeKeywords();
//}
//
//void Tokenizer::initializeKeywords() {
//    m_keywords[L"برنامج"] = CplTokenType::KEYWORD_PROGRAM;
//    m_keywords[L"ثابت"] = CplTokenType::KEYWORD_CONST;
//    m_keywords[L"نوع"] = CplTokenType::KEYWORD_TYPE;
//    m_keywords[L"متغير"] = CplTokenType::KEYWORD_VAR;
//    m_keywords[L"اجراء"] = CplTokenType::KEYWORD_PROCEDURE;
//    m_keywords[L"اقرا"] = CplTokenType::KEYWORD_READ;
//    m_keywords[L"اطبع"] = CplTokenType::KEYWORD_WRITE;
//    m_keywords[L"اذا"] = CplTokenType::KEYWORD_IF;
//    m_keywords[L"فان"] = CplTokenType::KEYWORD_THEN;
//    m_keywords[L"والا"] = CplTokenType::KEYWORD_ELSE;
//    m_keywords[L"صحيح"] = CplTokenType::TYPE_INTEGER;
//    m_keywords[L"حقيقي"] = CplTokenType::TYPE_REAL;
//}
//
//std::vector<CplToken> Tokenizer::tokenizeAll() {
//    std::vector<CplToken> tokens;
//    while (!isAtEnd()) {
//        tokens.push_back(getNextToken());
//    }
//    tokens.push_back({ CplTokenType::END_OF_FILE, L"EOF", m_current_line });
//    return tokens;
//}
//
//CplToken Tokenizer::getNextToken() {
//    skipWhitespaceAndComments();
//    if (isAtEnd()) return { CplTokenType::END_OF_FILE, L"", m_current_line };
//
//    wchar_t c = peek();
//
//    if (iswalpha(c) || c == L'_') return readIdentifierOrKeyword();
//    if (iswdigit(c)) return readNumber();
//    if (c == L'"') return readStringLiteral();
//
//    // Two-character operators
//    if (peek(1) != L'\0') {
//        std::wstring two_char_op;
//        two_char_op += c;
//        two_char_op += peek(1);
//        if (two_char_op == L"==") { advance(); advance(); return { CplTokenType::EQUAL, two_char_op, m_current_line }; }
//        if (two_char_op == L"!=") { advance(); advance(); return { CplTokenType::NOT_EQUAL, two_char_op, m_current_line }; }
//        if (two_char_op == L">=") { advance(); advance(); return { CplTokenType::GREATER_EQUAL, two_char_op, m_current_line }; }
//        if (two_char_op == L"<=") { advance(); advance(); return { CplTokenType::LESS_EQUAL, two_char_op, m_current_line }; }
//        if (two_char_op == L"&&") { advance(); advance(); return { CplTokenType::LOGICAL_AND, two_char_op, m_current_line }; }
//        if (two_char_op == L"||") { advance(); advance(); return { CplTokenType::LOGICAL_OR, two_char_op, m_current_line }; }
//    }
//
//    // Single-character operators and punctuation
//    advance();
//    switch (c) {
//    case L'=': return { CplTokenType::ASSIGN, L"=", m_current_line };
//    case L'+': return { CplTokenType::PLUS, L"+", m_current_line };
//    case L'-': return { CplTokenType::MINUS, L"-", m_current_line };
//    case L'*': return { CplTokenType::MULTIPLY, L"*", m_current_line };
//    case L'/': return { CplTokenType::DIVIDE, L"/", m_current_line };
//    case L'%': return { CplTokenType::MODULO, L"%", m_current_line };
//    case L'>': return { CplTokenType::GREATER, L">", m_current_line };
//    case L'<': return { CplTokenType::LESS, L"<", m_current_line };
//    case L'!': return { CplTokenType::LOG_NOT, L"!", m_current_line };
//    case L'(': return { CplTokenType::L_PAREN, L"(", m_current_line };
//    case L')': return { CplTokenType::R_PAREN, L")", m_current_line };
//    case L'{': return { CplTokenType::L_BRACE, L"{", m_current_line };
//    case L'}': return { CplTokenType::R_BRACE, L"}", m_current_line };
//    case L'[': return { CplTokenType::L_BRACKET, L"[", m_current_line };
//    case L']': return { CplTokenType::R_BRACKET, L"]", m_current_line };
//    case L';': return { CplTokenType::SEMICOLON, L";", m_current_line };
//    case L',': return { CplTokenType::COMMA, L",", m_current_line };
//    case L':': return { CplTokenType::COLON, L":", m_current_line };
//    case L'.': return { CplTokenType::DOT, L".", m_current_line };
//    default:  return { CplTokenType::UNKNOWN, std::wstring(1, c), m_current_line };
//    }
//}
//
//void Tokenizer::skipWhitespaceAndComments() {
//    while (!isAtEnd()) {
//        if (iswspace(peek())) {
//            if (peek() == L'\n') m_current_line++;
//            advance();
//        }
//        else if (peek() == L'/' && peek(1) == L'/') {
//            while (!isAtEnd() && peek() != L'\n') advance();
//        }
//        else if (peek() == L'/' && peek(1) == L'*') {
//            advance(); advance();
//            while (!isAtEnd() && !(peek() == L'*' && peek(1) == L'/')) {
//                if (peek() == L'\n') m_current_line++;
//                advance();
//            }
//            if (!isAtEnd()) advance();
//            if (!isAtEnd()) advance();
//        }
//        else {
//            break;
//        }
//    }
//}
//
//CplToken Tokenizer::readNumber() {
//    std::wstring number_str;
//    bool is_float = false;
//    while (!isAtEnd() && iswdigit(peek())) {
//        number_str += advance();
//    }
//    if (peek() == L'.' && iswdigit(peek(1))) {
//        is_float = true;
//        number_str += advance();
//        while (!isAtEnd() && iswdigit(peek())) {
//            number_str += advance();
//        }
//    }
//    return { is_float ? CplTokenType::REAL_LITERAL : CplTokenType::INTEGER_LITERAL, number_str, m_current_line };
//}
//
//CplToken Tokenizer::readStringLiteral() {
//    std::wstring str;
//    advance(); // Consume opening quote
//    while (!isAtEnd() && peek() != L'"') {
//        str += advance();
//    }
//    if (isAtEnd()) { // Unterminated string
//        return { CplTokenType::UNKNOWN, str, m_current_line };
//    }
//    advance(); // Consume closing quote
//    return { CplTokenType::STRING_LITERAL, str, m_current_line };
//}
//
//CplToken Tokenizer::readIdentifierOrKeyword() {
//    std::wstring text;
//    while (!isAtEnd() && (iswalnum(peek()) || peek() == L'_')) {
//        text += advance();
//    }
//    auto it = m_keywords.find(text);
//    if (it != m_keywords.end()) {
//        return { it->second, text, m_current_line };
//    }
//    return { CplTokenType::IDENTIFIER, text, m_current_line };
//}
//
//wchar_t Tokenizer::peek(int offset) const {
//    if (m_current_pos + offset >= m_source.length()) return L'\0';
//    return m_source[m_current_pos + offset];
//}
//
//wchar_t Tokenizer::advance() {
//    return m_source[m_current_pos++];
//}
//
//bool Tokenizer::isAtEnd() const {
//    return m_current_pos >= m_source.length();
//}
//
//// =================================================================================
//// SECTION 2: SYNTAX ANALYZER (PARSER) IMPLEMENTATION
//// =================================================================================
//
//Parser::Parser(const std::vector<CplToken>& tokens) : m_tokens(tokens) {}
//
//void Parser::parse() {
//    parse_program();
//}
//
//void Parser::parse_program() {
//    consume(CplTokenType::KEYWORD_PROGRAM, "Expected 'برنامج' keyword");
//    consume(CplTokenType::IDENTIFIER, "Expected program name");
//    consume(CplTokenType::SEMICOLON, "Expected ';' after program name");
//    parse_block();
//    consume(CplTokenType::DOT, "Expected '.' at the end of the program");
//}
//
//void Parser::parse_block() {
//    // Simplified implementation for now
//    parse_statement_list();
//}
//
//void Parser::parse_statement_list() {
//    consume(CplTokenType::L_BRACE, "Expected '{' to start a statement list");
//    while (currentToken().type != CplTokenType::R_BRACE && currentToken().type != CplTokenType::END_OF_FILE) {
//        parse_statement();
//    }
//    consume(CplTokenType::R_BRACE, "Expected '}' to end a statement list");
//}
//
//void Parser::parse_statement() {
//    if (currentToken().type == CplTokenType::IDENTIFIER) {
//        parse_assignment_statement();
//        consume(CplTokenType::SEMICOLON, "Expected ';' after statement");
//    }
//    else {
//        throw SyntaxError("Invalid statement", currentToken().line);
//    }
//}
//
//void Parser::parse_assignment_statement() {
//    consume(CplTokenType::IDENTIFIER, "Expected identifier");
//    consume(CplTokenType::ASSIGN, "Expected '='");
//    parse_expression();
//}
//
//void Parser::parse_expression() {
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
//    if (currentToken().type == CplTokenType::IDENTIFIER || currentToken().type == CplTokenType::INTEGER_LITERAL) {
//        advance();
//    }
//    else if (currentToken().type == CplTokenType::L_PAREN) {
//        advance();
//        parse_expression();
//        consume(CplTokenType::R_PAREN, "Expected ')'");
//    }
//    else {
//        throw SyntaxError("Invalid factor in expression", currentToken().line);
//    }
//}
//
//const CplToken& Parser::currentToken() const {
//    return m_tokens[m_current_pos];
//}
//
//void Parser::advance() {
//    if (m_current_pos < m_tokens.size() - 1) { // Prevent advancing past EOF
//        m_current_pos++;
//    }
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
//
//// =================================================================================
//// SECTION 3: EXPORTED DLL INTERFACE
//// =================================================================================
//
//extern "C" TOKENIZER_API const wchar_t* analyze_syntax_w(const wchar_t* input) {
//    static std::wstring result_str;
//    try {
//        Tokenizer tokenizer(input);
//        std::vector<CplToken> tokens = tokenizer.tokenizeAll();
//
//        Parser parser(tokens);
//        parser.parse();
//
//        result_str = L"التحليل النحوي صحيح. (Syntax is correct.)";
//    }
//    catch (const SyntaxError& e) {
//        std::string error_msg = e.what();
//        result_str = std::wstring(error_msg.begin(), error_msg.end());
//    }
//    catch (const std::exception& e) {
//        std::string error_msg = e.what();
//        result_str = L"خطأ عام: " + std::wstring(error_msg.begin(), error_msg.end());
//    }
//
//    size_t sizeInBytes = (result_str.length() + 1) * sizeof(wchar_t);
//    wchar_t* result = (wchar_t*)::CoTaskMemAlloc(sizeInBytes);
//    if (result) {
//        wcscpy_s(result, result_str.length() + 1, result_str.c_str());
//    }
//    return result;
//}


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

    // Functions
    m_keywords[L"دالة"] = CplTokenType::KEYWORD_FUNCTION;
    m_keywords[L"ارجع"] = CplTokenType::KEYWORD_RETURN;
    m_keywords[L"بلا"] = CplTokenType::TYPE_VOID;
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

<<<<<<< HEAD
void Parser::parse_program() {
    consume(CplTokenType::KEYWORD_PROGRAM, "Expected 'برنامج' keyword");
    consume(CplTokenType::IDENTIFIER, "Expected program name");
    consume(CplTokenType::SEMICOLON, "Expected ';' after program name");
    parse_declarations_part();
    parse_block();
    consume(CplTokenType::DOT, "Expected '.' at the end of the program");

}

void Parser::parse_block() {
    // Simplified implementation for now
    parse_statement_list();
}




// =================================================================
// ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼
//   الكود الكامل والجاهز لمهمتك (محمد - جزء التعريفات)
// ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
// =================================================================

/**
 * @brief هذه الدالة هي "المنظم" لجزء التعريفات.
 * تستمر في التحقق من وجود كلمات "ثابت" أو "متغير" وتستدعي الدالة المناسبة.
 */
void Parser::parse_declarations_part() {
    while (currentToken().type == CplTokenType::KEYWORD_CONST ||
        currentToken().type == CplTokenType::KEYWORD_VAR) {

        if (currentToken().type == CplTokenType::KEYWORD_CONST) {
            parse_const_declarations();
        }
        else if (currentToken().type == CplTokenType::KEYWORD_VAR) {
            parse_var_declarations();
        }
    }
}

/**
 * @brief هذه الدالة تحلل كتلة تعريف الثوابت التي تبدأ بكلمة "ثابت".
 * يمكنها التعامل مع عدة تعريفات للثوابت واحداً تلو الآخر.
 */
void Parser::parse_const_declarations() {
    consume(CplTokenType::KEYWORD_CONST, "Expected 'ثابت'");

    // استمر في التحليل طالما أن الرمز التالي هو اسم لثابت جديد
    while (currentToken().type == CplTokenType::IDENTIFIER) {
        consume(CplTokenType::IDENTIFIER, "Expected constant name");
        consume(CplTokenType::ASSIGN, "Expected '=' for constant definition");

        // يجب أن تكون القيمة إما رقم صحيح، حقيقي، أو نص
        if (currentToken().type == CplTokenType::INTEGER_LITERAL ||
            currentToken().type == CplTokenType::REAL_LITERAL ||
            currentToken().type == CplTokenType::STRING_LITERAL) {
            advance(); // استهلك القيمة
        }
        else {
            throw SyntaxError("Expected a literal value (number or string) for the constant", currentToken().line);
        }
        consume(CplTokenType::SEMICOLON, "Expected ';' after constant definition");
    }
}

/**
 * @brief هذه الدالة تحلل كتلة تعريف المتغيرات التي تبدأ بكلمة "متغير".
 * يمكنها التعامل مع تعريف عدة متغيرات من نفس النوع في سطر واحد (مثل س, ص: صحيح;).
 */
void Parser::parse_var_declarations() {
    consume(CplTokenType::KEYWORD_VAR, "Expected 'متغير'");

    // استمر في التحليل طالما أننا في بداية سطر تعريف متغيرات جديد
    while (currentToken().type == CplTokenType::IDENTIFIER) {
        // استهلك اسم المتغير الأول
        consume(CplTokenType::IDENTIFIER, "Expected variable name");

        // تحقق من وجود فاصلة (,) لتعريف متغيرات أخرى من نفس النوع
        while (currentToken().type == CplTokenType::COMMA) {
            advance(); // استهلك الفاصلة
            consume(CplTokenType::IDENTIFIER, "Expected another variable name after comma");
        }

        consume(CplTokenType::COLON, "Expected ':' after variable name(s)");

        parse_type_specifier(); // استدع الدالة المساعدة لتحليل النوع

        consume(CplTokenType::SEMICOLON, "Expected ';' after variable definition");
    }
}

/**
 * @brief دالة مساعدة للتحقق من أنواع البيانات الأساسية.
 * يمكن تطويرها لاحقاً لتفهم الأنواع المعقدة مثل ARRAY.
 */
void Parser::parse_type_specifier() {
    if (currentToken().type == CplTokenType::TYPE_INTEGER ||
        currentToken().type == CplTokenType::TYPE_REAL ||
        currentToken().type == CplTokenType::TYPE_BOOLEAN ||
        currentToken().type == CplTokenType::TYPE_CHAR ||
        currentToken().type == CplTokenType::TYPE_STRING) {
        advance(); // النوع صحيح، استهلكه
    }
    else {
        throw SyntaxError("Expected a valid data type (like 'صحيح', 'حقيقي', etc.)", currentToken().line);
    }
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

=======
>>>>>>> f2843479fe22b75f173bd26cb97045930732b6b1
const CplToken& Parser::currentToken() const {
    return m_tokens[m_current_pos];
}

const CplToken& Parser::lookahead(size_t k) const {
    size_t idx = m_current_pos + k;
    if (idx >= m_tokens.size()) return m_tokens.back();
    return m_tokens[idx];
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

bool Parser::isBuiltinType(CplTokenType t) const {
    return t == CplTokenType::TYPE_INTEGER ||
        t == CplTokenType::TYPE_REAL ||
        t == CplTokenType::TYPE_BOOLEAN ||
        t == CplTokenType::TYPE_CHAR ||
        t == CplTokenType::TYPE_STRING ||
        t == CplTokenType::TYPE_VOID;
}

void Parser::parse_type() {
    // Accept built-in types OR user-defined type identifiers
    if (isBuiltinType(currentToken().type) || currentToken().type == CplTokenType::IDENTIFIER) {
        advance();
    }
    else {
        throw SyntaxError("Expected type", currentToken().line);
    }
}

void Parser::parse_program() {
    consume(CplTokenType::KEYWORD_PROGRAM, "Expected 'برنامج' keyword");
    consume(CplTokenType::IDENTIFIER, "Expected program name");
    consume(CplTokenType::SEMICOLON, "Expected ';' after program name");
    parse_block();
    consume(CplTokenType::DOT, "Expected '.' at the end of the program");
}

void Parser::parse_block() {
    parse_statement_list();
}

void Parser::parse_statement_list() {
    consume(CplTokenType::L_BRACE, "Expected '{' to start a statement list");
    while (currentToken().type != CplTokenType::R_BRACE && currentToken().type != CplTokenType::END_OF_FILE) {
        if (currentToken().type == CplTokenType::KEYWORD_FUNCTION) {
            parse_function_declaration();
        }
        else if (currentToken().type == CplTokenType::KEYWORD_PROCEDURE) {
            parse_procedure_declaration(); // ← هنا استدعاء الدالة الجديدة
        }
        else {
            parse_statement();
        }
    }
    consume(CplTokenType::R_BRACE, "Expected '}' to end a statement list");
}

void Parser::parse_statement() {
    if (currentToken().type == CplTokenType::IDENTIFIER) {
        // assignment or function call statement
        if (lookahead().type == CplTokenType::L_PAREN) {
            parse_call_statement();
        }
        else {
            parse_assignment_statement();
            consume(CplTokenType::SEMICOLON, "Expected ';' after statement");
        }
    }
    else if (currentToken().type == CplTokenType::KEYWORD_RETURN) {
        parse_return_statement();
    }
    else if (currentToken().type == CplTokenType::KEYWORD_VAR) {
        parse_var_declaration(); // consumes trailing ';'
    }
    else if (currentToken().type == CplTokenType::L_BRACE) {
        // allow nested block as a statement
        parse_block();
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

void Parser::parse_var_declaration() {
    consume(CplTokenType::KEYWORD_VAR, "Expected 'متغير'");
    consume(CplTokenType::IDENTIFIER, "Expected variable name");
    consume(CplTokenType::COLON, "Expected ':' after variable name");
    parse_type();
    consume(CplTokenType::SEMICOLON, "Expected ';' after variable declaration");
}
void Parser::parse_procedure_declaration() {
    consume(CplTokenType::KEYWORD_PROCEDURE, "Expected 'اجراء'");
    consume(CplTokenType::IDENTIFIER, "Expected procedure name");
    consume(CplTokenType::L_PAREN, "Expected '('");
    // هنا نقرأ باراميترات مثل الدالة
    if (currentToken().type != CplTokenType::R_PAREN) {
        parse_parameter_list();
    }
    consume(CplTokenType::R_PAREN, "Expected ')'");
    consume(CplTokenType::SEMICOLON, "Expected ';'");
    parse_block(); // جسم الإجراء
}
void Parser::parse_function_declaration() {
    consume(CplTokenType::KEYWORD_FUNCTION, "Expected 'دالة'");
    consume(CplTokenType::IDENTIFIER, "Expected function name");
    consume(CplTokenType::L_PAREN, "Expected '(' after function name");

    // Parameters (now supported)
    if (currentToken().type != CplTokenType::R_PAREN) {
        parse_parameter_list();
    }
    consume(CplTokenType::R_PAREN, "Expected ')' after parameters");

    consume(CplTokenType::COLON, "Expected ':' before return type");
    parse_type(); // accept built-in or user-defined type

    consume(CplTokenType::SEMICOLON, "Expected ';' before function body");
    parse_block();
}

void Parser::parse_parameter_list() {
    // parameter := <type> <identifier>
    parse_type();
    consume(CplTokenType::IDENTIFIER, "Expected parameter name");

    while (currentToken().type == CplTokenType::COMMA) {
        advance(); // consume ','
        parse_type();
        consume(CplTokenType::IDENTIFIER, "Expected parameter name");
    }
}

void Parser::parse_return_statement() {
    consume(CplTokenType::KEYWORD_RETURN, "Expected 'ارجع'");
    parse_expression();
    consume(CplTokenType::SEMICOLON, "Expected ';' after return statement");
}

void Parser::parse_call_statement() {
    parse_function_call_core();
    consume(CplTokenType::SEMICOLON, "Expected ';' after function call");
}

void Parser::parse_function_call_core() {
    // IDENTIFIER '(' [arg_list] ')'
    consume(CplTokenType::IDENTIFIER, "Expected function name");
    consume(CplTokenType::L_PAREN, "Expected '('");
     
    if (currentToken().type != CplTokenType::R_PAREN) {
        // arg_list := expression (',' expression)*
        parse_expression();
        while (currentToken().type == CplTokenType::COMMA) {
            advance();
            parse_expression();
        }
    }
    consume(CplTokenType::R_PAREN, "Expected ')'");
}

void Parser::parse_expression() {
    parse_term();
    while (currentToken().type == CplTokenType::PLUS || currentToken().type == CplTokenType::MINUS ||
        currentToken().type == CplTokenType::LOGICAL_OR) {
        advance();
        parse_term();
    }
}

void Parser::parse_term() {
    parse_factor();
    while (currentToken().type == CplTokenType::MULTIPLY || currentToken().type == CplTokenType::DIVIDE ||
        currentToken().type == CplTokenType::MODULO || currentToken().type == CplTokenType::LOGICAL_AND) {
        advance();
        parse_factor();
    }
}

void Parser::parse_factor() {
    if (currentToken().type == CplTokenType::IDENTIFIER) {
        if (lookahead().type == CplTokenType::L_PAREN) {
            // function call inside expression
            parse_function_call_core();
        }
        else {
            advance(); // variable / identifier
        }
    }
    else if (currentToken().type == CplTokenType::INTEGER_LITERAL ||
        currentToken().type == CplTokenType::REAL_LITERAL ||
        currentToken().type == CplTokenType::STRING_LITERAL) {
        advance();
    }
    else if (currentToken().type == CplTokenType::L_PAREN) {
        advance();
        parse_expression();
        consume(CplTokenType::R_PAREN, "Expected ')'");
    }
    else if (currentToken().type == CplTokenType::MINUS || currentToken().type == CplTokenType::LOG_NOT) {
        // unary - or !
        advance();
        parse_factor();
    }
    else {
        throw SyntaxError("Invalid factor in expression", currentToken().line);
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




