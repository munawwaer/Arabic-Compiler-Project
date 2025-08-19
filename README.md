# Arabic-Compiler-Project
مترجم بسيط للغة برمجة جديدة تدعم الكلمات المفتاحية والمعرفات باللغة العربية .هذا مشروع من مقرر CS351 في جامعة اب
/*
============================================================
   ✦ Osama's Work Summary ✦
   هذا التلخيص يوضح بالضبط العمليات (Tasks) التي أنجزها أسامة
   مع التعليقات الأصلية التي تركناها داخل الملفات من أجل الوضوح
   لفريق العمل لاحقًا.
============================================================
*/

// -----------------------------------------------------------
// 1) إضافة الكلمات المحجوزة (Keywords)
// -----------------------------------------------------------
// File: Tokenizer.cpp (داخل initializeKeywords)
// -----------------------------------------------------------
// ➤ أضفت:
//   بينما   → KEYWORD_WHILE
//   كرر     → KEYWORD_FOR
//   الى     → KEYWORD_TO
//   اعِد    → KEYWORD_REPEAT
//   حتى     → KEYWORD_UNTIL
//   منطقي   → TYPE_BOOL
//   محرف    → TYPE_CHAR
//   نصي     → TYPE_STRING
//   اطبع    → KEYWORD_WRITE
// -----------------------------------------------------------

// -----------------------------------------------------------
// 2) تعريف الدوال الجديدة في Parser
// -----------------------------------------------------------
// File: Tokenizer.h (داخل class Parser)
// -----------------------------------------------------------
//   parse_if_stmt();
//   parse_while_stmt();
//   parse_for_stmt();
//   parse_repeat_stmt();
//   parse_print_statement();
//   parse_logic_or();
//   parse_logic_and();
//   parse_equality();
//   parse_comparison();
//   parse_addition();
//   parse_multiplication();
//   parse_unary();
//   parse_primary();
// -----------------------------------------------------------

// -----------------------------------------------------------
// 3) تنفيذ الدوال (Implementations)
// -----------------------------------------------------------
// File: Tokenizer.cpp (أسفل الملف)
// -----------------------------------------------------------
//   ✔ parse_if_stmt(): يدعم if / else if / else
//   ✔ parse_while_stmt(): يدعم بينما (شرط) {...}
//   ✔ parse_for_stmt(): يدعم كرر i = 1 الى 5 {...}
//   ✔ parse_repeat_stmt(): يدعم اعِد {...} حتى (شرط);
//   ✔ parse_print_statement(): يدعم اطبع "نص"; أو اطبع expr;
//   ✔ سلم أولويات التعبيرات (من logic_or حتى primary)
// -----------------------------------------------------------

// -----------------------------------------------------------
// 4) تعديل parse_statement()
// -----------------------------------------------------------
// File: Tokenizer.cpp (داخل Parser::parse_statement)
// -----------------------------------------------------------
//   case KEYWORD_WRITE:   return parse_print_statement();
//   case KEYWORD_IF:      return parse_if_stmt();
//   case KEYWORD_WHILE:   return parse_while_stmt();
//   case KEYWORD_FOR:     return parse_for_stmt();
//   case KEYWORD_REPEAT:  return parse_repeat_stmt();
// -----------------------------------------------------------

// -----------------------------------------------------------
// 5) دعم سلم الأولويات للتعبيرات
// -----------------------------------------------------------
// File: Tokenizer.cpp (داخل Parser)
// -----------------------------------------------------------
// parse_expression()  ← parse_logic_or()
// parse_logic_or()    ← parse_logic_and()
// parse_logic_and()   ← parse_equality()
// parse_equality()    ← parse_comparison()
// parse_comparison()  ← parse_addition()
// parse_addition()    ← parse_multiplication()
// parse_multiplication() ← parse_unary()
// parse_unary()       ← parse_primary()
// parse_primary()     ← literals, identifiers, (expr)
// -----------------------------------------------------------

/*
============================================================
   ✔ الخلاصة
   - الآن المشروع يدعم: if / while / for / repeat-until / print
   - يدعم سلم أولويات كامل للتعبيرات الحسابية والمنطقية
   - تم ترك التعليقات داخل الكود (// Osama's Task Only)
     لسهولة الرجوع من قِبل أي عضو في الفريق.
============================================================
*/
