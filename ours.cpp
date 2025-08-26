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

    // Keywords from the other branch
    m_keywords[L"دالة"] = CplTokenType::KEYWORD_FUNCTION;
    m_keywords[L"ارجع"] = CplTokenType::KEYWORD_RETURN;
    m_keywords[L"بلا"] = CplTokenType::TYPE_VOID;



    // ------- Osama additions: control-flow & extra builtin types (tokenization) -------
    m_keywords[L"بينما"] = CplTokenType::KEYWORD_WHILE;      // Osama: add Arabic 'while' keyword
    m_keywords[L"كرر"] = CplTokenType::KEYWORD_FOR;        // Osama: add Arabic 'for' keyword
    m_keywords[L"الى"] = CplTokenType::KEYWORD_TO;         // Osama: add Arabic 'to' (for loops)
    m_keywords[L"اعِد"] = CplTokenType::KEYWORD_REPEAT;     // Osama: add Arabic 'repeat' keyword
    m_keywords[L"حتى"] = CplTokenType::KEYWORD_UNTIL;      // Osama: add Arabic 'until' keyword

    m_keywords[L"منطقي"] = CplTokenType::TYPE_BOOLEAN;       // Osama: add builtin type 'boolean'
    m_keywords[L"محرف"] = CplTokenType::TYPE_CHAR;          // Osama: add builtin type 'char'
    m_keywords[L"نصي"] = CplTokenType::TYPE_STRING;        // Osama: add builtin type 'string'
    // -------------------------------------------------------------------------------
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
    // (Osama) normalize Arabic forms so keywords match even مع (أ/إ/آ -> ا)
    std::wstring key = normalize_arabic(text); // (إضافة أسامة)
    auto it = m_keywords.find(key);            // <-- كان البحث بـ text؛ تم إصلاحه إلى key
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
// ---- Osama's helpers (impl) ----
// (إضافة أسامة) دوال خدمة لتوحيد الألف/الياء والتعامل مع أرقام عربية إن لزم.
std::wstring Tokenizer::normalize_arabic(const std::wstring& in) {
    std::wstring out; out.reserve(in.size());
    for (wchar_t ch : in) {
        switch (ch) {
        case L'أ': case L'إ': case L'آ': ch = L'ا'; break;
        case L'ى': ch = L'ي'; break;
        default: break;
        }
        out.push_back(ch);
    }
    return out;
}
bool Tokenizer::is_arabic_decimal_separator(wchar_t ch) { return ch == L'.' || ch == L'\u066B'; }
bool Tokenizer::is_digit_any(wchar_t ch) { return iswdigit(ch) != 0; }

// =================================================================================
// SECTION 2: SYNTAX ANALYZER (PARSER) IMPLEMENTATION
// =================================================================================

Parser::Parser(const std::vector<CplToken>& tokens) : m_tokens(tokens) {}

void Parser::parse() {
    parse_program();
}

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
    parse_declarations_part(); // Your declarations part
    parse_block();
    consume(CplTokenType::DOT, "Expected '.' at the end of the program");
}

void Parser::parse_block() {
    parse_statement_list();
}

// Your declarations parsing functions
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

void Parser::parse_const_declarations() {
    consume(CplTokenType::KEYWORD_CONST, "Expected 'ثابت'");
    while (currentToken().type == CplTokenType::IDENTIFIER) {
        consume(CplTokenType::IDENTIFIER, "Expected constant name");
        consume(CplTokenType::ASSIGN, "Expected '=' for constant definition");
        if (currentToken().type == CplTokenType::INTEGER_LITERAL ||
            currentToken().type == CplTokenType::REAL_LITERAL ||
            currentToken().type == CplTokenType::STRING_LITERAL) {
            advance();
        }
        else {
            throw SyntaxError("Expected a literal value (number or string) for the constant", currentToken().line);
        }
        consume(CplTokenType::SEMICOLON, "Expected ';' after constant definition");
    }
}

void Parser::parse_var_declarations() {
    consume(CplTokenType::KEYWORD_VAR, "Expected 'متغير'");
    while (currentToken().type == CplTokenType::IDENTIFIER) {
        consume(CplTokenType::IDENTIFIER, "Expected variable name");
        while (currentToken().type == CplTokenType::COMMA) {
            advance();
            consume(CplTokenType::IDENTIFIER, "Expected another variable name after comma");
        }
        consume(CplTokenType::COLON, "Expected ':' after variable name(s)");
        parse_type_specifier();
        consume(CplTokenType::SEMICOLON, "Expected ';' after variable definition");
    }
}

void Parser::parse_type_specifier() {
    if (isBuiltinType(currentToken().type)) {
        advance();
    }
    else {
        throw SyntaxError("Expected a valid data type (like 'صحيح', 'حقيقي', etc.)", currentToken().line);
    }
}

// Teammate's functions and statements parsing
void Parser::parse_statement_list() {
    consume(CplTokenType::L_BRACE, "Expected '{' to start a statement list");
    while (currentToken().type != CplTokenType::R_BRACE && currentToken().type != CplTokenType::END_OF_FILE) {
        if (currentToken().type == CplTokenType::KEYWORD_FUNCTION) {
            parse_function_declaration();
        }
        else if (currentToken().type == CplTokenType::KEYWORD_PROCEDURE) {
            parse_procedure_declaration();
        }
        else {
            parse_statement();
        }
    }
    consume(CplTokenType::R_BRACE, "Expected '}' to end a statement list");
}

void Parser::parse_statement() {
    if (currentToken().type == CplTokenType::IDENTIFIER) {
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
    else if (currentToken().type == CplTokenType::KEYWORD_WRITE) {
        parse_print_statement();        // (Osama) اطبع "نص"; أو اطبع تعبير;
        consume(CplTokenType::SEMICOLON, "Expected ';' after statement");
    }
    else if (currentToken().type == CplTokenType::KEYWORD_IF) {
        parse_if_stmt();                // (Osama) if / else if / else
    }
    else if (currentToken().type == CplTokenType::KEYWORD_WHILE) {
        parse_while_stmt();             // (Osama) while
    }
    else if (currentToken().type == CplTokenType::KEYWORD_FOR) {
        parse_for_stmt();               // (Osama) for "كرر .. الى .."
    }
    else if (currentToken().type == CplTokenType::KEYWORD_REPEAT) {
        parse_repeat_stmt();            // (Osama) repeat .. until ;
    }
    else if (currentToken().type == CplTokenType::L_BRACE) {
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

void Parser::parse_procedure_declaration() {
    consume(CplTokenType::KEYWORD_PROCEDURE, "Expected 'اجراء'");
    consume(CplTokenType::IDENTIFIER, "Expected procedure name");
    consume(CplTokenType::L_PAREN, "Expected '('");
    if (currentToken().type != CplTokenType::R_PAREN) {
        parse_parameter_list();
    }
    consume(CplTokenType::R_PAREN, "Expected ')'");
    consume(CplTokenType::SEMICOLON, "Expected ';'");
    parse_block(); // Procedure body
}

void Parser::parse_function_declaration() {
    consume(CplTokenType::KEYWORD_FUNCTION, "Expected 'دالة'");
    consume(CplTokenType::IDENTIFIER, "Expected function name");
    consume(CplTokenType::L_PAREN, "Expected '(' after function name");
    if (currentToken().type != CplTokenType::R_PAREN) {
        parse_parameter_list();
    }
    consume(CplTokenType::R_PAREN, "Expected ')' after parameters");
    consume(CplTokenType::COLON, "Expected ':' before return type");
    parse_type();
    consume(CplTokenType::SEMICOLON, "Expected ';' before function body");
    parse_block();
}

void Parser::parse_parameter_list() {
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
    consume(CplTokenType::IDENTIFIER, "Expected function name");
    consume(CplTokenType::L_PAREN, "Expected '('");
    if (currentToken().type != CplTokenType::R_PAREN) {
        parse_expression();
        while (currentToken().type == CplTokenType::COMMA) {
            advance();
            parse_expression();
        }
    }
    consume(CplTokenType::R_PAREN, "Expected ')'");
}

void Parser::parse_expression() {
    // Osama: أعلى مستوى — OR
    parse_logic_or();
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
            parse_function_call_core();
        }
        else {
            advance();
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
        advance();
        parse_factor();
    }
    else {
        throw SyntaxError("Invalid factor in expression", currentToken().line);
    }
}


// ====================== Osama: Expression Precedence Ladder ======================

void Parser::parse_logic_or() {
    parse_logic_and();                                         // Osama: parse left operand at AND level
    while (currentToken().type == CplTokenType::LOGICAL_OR) {  // Osama: while there's "||" chained
        advance();                                             // Osama: consume "||"
        parse_logic_and();                                     // Osama: parse the next right operand
    }
}

void Parser::parse_logic_and() {
    parse_equality();                                          // Osama: go one level down to equality
    while (currentToken().type == CplTokenType::LOGICAL_AND) { // Osama: while there's "&&" chained
        advance();                                             // Osama: consume "&&"
        parse_equality();                                      // Osama: parse the next right operand
    }
}

void Parser::parse_equality() {
    parse_comparison();                                        // Osama: lower to comparison level
    while (currentToken().type == CplTokenType::EQUAL          // Osama: "=="
        || currentToken().type == CplTokenType::NOT_EQUAL) {   // Osama: "!="
        advance();                                             // Osama: consume == / !=
        parse_comparison();                                    // Osama: parse right operand
    }
}

void Parser::parse_comparison() {
    parse_addition();                                          // Osama: lower to +/-
    while (currentToken().type == CplTokenType::LESS           // Osama: "<"
        || currentToken().type == CplTokenType::LESS_EQUAL     // Osama: "<="
        || currentToken().type == CplTokenType::GREATER        // Osama: ">"
        || currentToken().type == CplTokenType::GREATER_EQUAL) // Osama: ">="
    {
        advance();                                             // Osama: consume comparator
        parse_addition();                                      // Osama: parse right operand
    }
}

void Parser::parse_addition() {
    parse_multiplication();                                    // Osama: lower to */%
    while (currentToken().type == CplTokenType::PLUS           // Osama: "+"
        || currentToken().type == CplTokenType::MINUS) {       // Osama: "-"
        advance();                                             // Osama: consume + / -
        parse_multiplication();                                // Osama: parse the next term
    }
}

void Parser::parse_multiplication() {
    parse_unary();                                             // Osama: lower to unary
    while (currentToken().type == CplTokenType::MULTIPLY       // Osama: "*"
        || currentToken().type == CplTokenType::DIVIDE         // Osama: "/"
        || currentToken().type == CplTokenType::MODULO) {      // Osama: "%"
        advance();                                             // Osama: consume operator
        parse_unary();                                         // Osama: parse the next factor
    }
}

void Parser::parse_unary() {
    if (currentToken().type == CplTokenType::LOG_NOT           // Osama: "!"
        || currentToken().type == CplTokenType::MINUS) {       // Osama: unary "-"
        advance();                                             // Osama: consume unary op
        parse_unary();                                         // Osama: support chains like !!-x
        return;                                                // Osama: done with this unary chain
    }
    parse_primary();                                           // Osama: otherwise → primary unit
}

void Parser::parse_primary() {
    switch (currentToken().type) {
    case CplTokenType::IDENTIFIER: {                       // Osama: identifier (var or function call)
        if (lookahead().type == CplTokenType::L_PAREN) {   // Osama: IDENT "(" → a function call
            parse_function_call_core();                    // Osama: parse call inside expression
        }
        else {
            advance();                                     // Osama: plain variable/identifier
        }
        return;
    }
    case CplTokenType::INTEGER_LITERAL:                    // Osama: numeric literal (int)
    case CplTokenType::REAL_LITERAL:                       // Osama: numeric literal (real)
    case CplTokenType::STRING_LITERAL:                     // Osama: string literal
        advance();                                         // Osama: consume the literal
        return;
    case CplTokenType::L_PAREN: {                          // Osama: parenthesized "( ... )"
        advance();                                         // Osama: consume '('
        parse_expression();                                // Osama: parse inner expression
        consume(CplTokenType::R_PAREN, "Expected ')'");    // Osama: require closing ')'
        return;
    }
    default:
        throw SyntaxError("Invalid factor in expression",  // Osama: unexpected token at primary
            currentToken().line);
    }
}


// ====== Control-flow statements (Osama) ======
// (إضافة أسامة) جمل التحكم: اذا/فان/والا – بينما – كرر/الى – أعد/حتى


void Parser::parse_if_stmt() {                                        // Osama: if/then/else chain
    consume(CplTokenType::KEYWORD_IF, "Expected 'اذا'");            // Osama
    parse_expression();                                               // Osama: condition
    consume(CplTokenType::KEYWORD_THEN, "Expected 'فان'");            // Osama
    parse_statement();                                                // Osama: then-branch

    while (currentToken().type == CplTokenType::KEYWORD_ELSE) {       // Osama: handle else / else if
        advance();                                                    // Osama: consume 'والا'
        if (currentToken().type == CplTokenType::KEYWORD_IF) {        // Osama: else-if
            advance();                                                // Osama: consume 'اذا'
            parse_expression();                                       // Osama: new condition
            consume(CplTokenType::KEYWORD_THEN, "Expected 'فان'");    // Osama
            parse_statement();                                        // Osama: else-if body
        }
        else {
            parse_statement();                                        // Osama: plain else body
            break;                                                    // Osama: end chain
        }
    }
}

void Parser::parse_while_stmt() {                                     // Osama: while-loop
    consume(CplTokenType::KEYWORD_WHILE, "Expected 'بينما'");         // Osama
    parse_expression();                                               // Osama: loop condition
    parse_statement();                                                // Osama: single stmt or { block }
}

void Parser::parse_for_stmt() {                                       // Osama: for i = a الى b do stmt
    consume(CplTokenType::KEYWORD_FOR, "Expected 'كرر'");             // Osama
    consume(CplTokenType::IDENTIFIER, "Expected loop variable");     // Osama: loop var
    consume(CplTokenType::ASSIGN, "Expected '=' after loop variable"); // Osama
    parse_expression();                                               // Osama: start expr
    consume(CplTokenType::KEYWORD_TO, "Expected 'الى'");             // Osama: 'to' keyword
    parse_expression();                                               // Osama: end expr
    parse_statement();                                                // Osama: loop body
}

void Parser::parse_repeat_stmt() {                                    // Osama: repeat ... until cond ;
    consume(CplTokenType::KEYWORD_REPEAT, "Expected 'اعِد'");         // Osama
    if (currentToken().type == CplTokenType::L_BRACE)                 // Osama: allow { ... } block
        parse_block();                                                // Osama
    else
        parse_statement();                                            // Osama: single stmt
    consume(CplTokenType::KEYWORD_UNTIL, "Expected 'حتى'");           // Osama
    parse_expression();                                               // Osama: exit condition
    consume(CplTokenType::SEMICOLON, "Expected ';' after 'حتى' condition"); // Osama
}
// -----------------------------------------------------------------------

// ====== اطبع (Osama) ======
// (إضافة أسامة) دعم جملة:  اطبع "نص";  أو  اطبع تعبير;
void Parser::parse_print_statement() {
    consume(CplTokenType::KEYWORD_WRITE, "Expected 'اطبع'");
    if (currentToken().type == CplTokenType::STRING_LITERAL) {
        advance(); // "النص" (Osama)
    }
    else {
        parse_expression(); // اطبع x+1 (Osama)
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
