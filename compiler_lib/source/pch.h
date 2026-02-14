// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#include <cstring>

// add headers that you want to pre-compile here
//#include "../framework.h"
#define STRINGS_EQUAL(str, str2) \
  (str && str2 && (strcmp(str, str2) == 0))


#endif // PCH_H


/*
  ExpressionableOperatorPrecedence()
    : data{
      {{"++", "--", "()", "[]", "(", "[", ".", "->"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"*", "/", "%"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"+", "-"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"<<", ">>"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"<", ">", "<=", ">="}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"==", "!="}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"&"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"^"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"|"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"&&"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"||"}, ASSOCIATIVITY_LEFT_TO_RIGHT},
      {{"?", ":"}, ASSOCIATIVITY_RIGHT_TO_LEFT},
      {{"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|="}, ASSOCIATIVITY_RIGHT_TO_LEFT},
      {",", ASSOCIATIVITY_LEFT_TO_RIGHT}
    }
  {
*/
