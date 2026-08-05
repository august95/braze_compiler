#include "pch.h"
#include "../precedenceHandler.h"
#include <cstddef>

#define ASSOSCIATIVITY_LEFT_TO_RIGHT 0
#define ASSOSCIATIVITY_RIGHT_TO_LEFT 1

expresssionable_op_precedence_group op_precedence[TOTAL_OPERATOR_GROUPS] = {
  {{"++", "--", "()", "[]", "(", "[", ".", "->", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"*", "/", "%", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"+", "-", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"<<", ">>", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"<", ">", "<=", ">=", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"==", "!=", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"&", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"^", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"|", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"&&", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"||", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT},
  {{"?", ":", NULL}, ASSOSCIATIVITY_RIGHT_TO_LEFT},
  {{"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", NULL}, ASSOSCIATIVITY_RIGHT_TO_LEFT},
  {{",", NULL}, ASSOSCIATIVITY_LEFT_TO_RIGHT}};

int precedenceHandler::getPrecendenceForOperator(const char *op, expresssionable_op_precedence_group **group_out)
{
  *group_out = nullptr;
  for (int i = 0; i < TOTAL_OPERATOR_GROUPS; i++)
  {
    for (int b = 0; op_precedence[i].operators[b]; b++)
    {
      const char *_op = op_precedence[i].operators[b];
      if (STRINGS_EQUAL(op, _op))
      {
        *group_out = &op_precedence[i];
        return i;
      }
    }
  }
  return -1;
}

bool precedenceHandler::leftOperatorHasPriority(const char *op_left, const char *op_right)
{
  expresssionable_op_precedence_group *group_left = nullptr;
  expresssionable_op_precedence_group *group_right = nullptr;

  if (STRINGS_EQUAL(op_left, op_right))
  {
    return false;
  }

  int precedence_left = getPrecendenceForOperator(op_left, &group_left);
  int precedence_right = getPrecendenceForOperator(op_right, &group_right);

  if (precedence_left == -1 || precedence_right == -1)
  {
    return false;
  }

  if (group_left->associtivity == ASSOSCIATIVITY_RIGHT_TO_LEFT)
  {
    return precedence_left < precedence_right;
  }

  return precedence_left <= precedence_right;
}
