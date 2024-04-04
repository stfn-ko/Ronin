#include "../inc/parser.h"

auto parse(std::vector<token> tokens) -> AST {

  // expression -> literal | unary | binary | grouping;
  // literal -> LIT_NUM | LIT_STR | KW_TRUE | KW_FALSE;
  // grouping -> PUNCT_L_PAREN + expression + PUNCT_R_PAREN;
  // unary -> (PUNCT_MINUS | PUNCT_EXCL) + expression;
  // binary -> expression + operator + expression;
  // operator -> COMBO_EQ |

  for (const auto &t : tokens) {
    std::cout << "type: " << t.type << " | (ln:" << t.pos.ln
              << ", | col:" << t.pos.col << ")\n"
              << t.lexeme << "\n\n";
  }

  return AST();
}
