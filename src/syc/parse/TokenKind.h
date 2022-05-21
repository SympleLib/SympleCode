/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#ifndef TOKEN
#define TOKEN(x)
#endif

#ifndef KEYWORD
#define KEYWORD(x, y) TOKEN(x)
#endif

#ifndef PUNCTUATOR
#define PUNCTUATOR(x,y) TOKEN(x)
#endif

TOKEN(Unknown)

TOKEN(Eof)
TOKEN(Identifier)
TOKEN(Integer)

PUNCTUATOR(Equal, "=")
PUNCTUATOR(Semicolon, ";")
PUNCTUATOR(Plus, "+")
PUNCTUATOR(Minus, "-")

KEYWORD(Return, ret)

#undef TOKEN
#undef PUNCTUATOR
#undef KEYWORD