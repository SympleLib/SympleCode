/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#ifndef ASTKIND
#define ASTKIND(x)
#endif

ASTKIND(None)

// Type
ASTKIND(PrimitiveType)

// Stmt
ASTKIND(ReturnStmt)
ASTKIND(VariableStmt)

// Expr
ASTKIND(BinaryExpr)
ASTKIND(VariableExpr)

// Literal
ASTKIND(IntLiteral)

#undef ASTKind