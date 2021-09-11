namespace CommenSense;

record Ast;
record ModuleAst(string name, StmtAst[] members);

record TypeAst(string typeBase, int ptrCount): Ast;
record ParamAst(TypeAst type, string name, ExprAst defaultExpr): Ast;
record FieldAst(Enum vis, TypeAst type, string name, ExprAst initializer): Ast;


record StmtAst: Ast;

record ImplAst(Enum vis, string name, FuncAst[] funcs): StmtAst;
record StructAst(Enum vis, string name, FieldAst[] fields): StmtAst;
record ClassAst(Enum vis, string name, FieldAst[] fields, FuncAst[] funcs): StmtAst;

record DeclFuncAst(Enum vis, TypeAst retType, string name, ParamAst[] paramz): StmtAst;
record FuncAst(Enum vis, TypeAst retType, string name, ParamAst[] paramz, StmtAst[] body): StmtAst;
record DeclVarAst(Enum vis, TypeAst type, string name): StmtAst;
record VarAst(Enum vis, TypeAst type, string name, ExprAst initializer): StmtAst;

record ExprStmtAst(ExprAst expr): StmtAst;


record ExprAst: Ast;

record LiteralExprAst(): ExprAst;
record IntLiteralExprAst(ulong value): LiteralExprAst;
record StrLiteralExprAst(string value): LiteralExprAst;
record FloatLiteralExprAst(double value): LiteralExprAst;

record VarExprAst(string varName): ExprAst;
record FuncPtrAst(string funcName, TypeAst[] paramTypes): ExprAst;

record UnExprAst(Enum op, ExprAst operand): ExprAst;
record BiExprAst(Enum op, ExprAst left, ExprAst right): ExprAst;