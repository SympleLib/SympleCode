namespace CommenSense;

partial record Ast;
partial record ModuleAst(string name, StmtAst[] members, StructAst[] structs);

partial record TypeAst(int ptrCount): Ast;
partial record BaseTypeAst(string typeBase, int ptrCount): TypeAst(ptrCount);
partial record FuncTypeAst(TypeAst retType, TypeAst[] paramTypes, bool vaArg, int ptrCount): TypeAst(ptrCount);
partial record ParamAst(TypeAst type, string name, ExprAst defaultExpr): Ast;
partial record FieldAst(Enum vis, TypeAst type, string name, ExprAst initializer): Ast;


partial record StmtAst: Ast;

partial record ImplAst(Enum vis, string name, FuncAst[] funcs): StmtAst;
partial record StructAst(Enum vis, string name, FieldAst[] fields): StmtAst;
partial record ClassAst(Enum vis, string name, FieldAst[] fields, FuncAst[] funcs): StmtAst;

partial record DeclFuncAst(Enum vis, TypeAst retType, string name, ParamAst[] paramz, bool vaArg): StmtAst;
partial record FuncAst(Enum vis, TypeAst retType, string name, ParamAst[] paramz, StmtAst[] body, bool vaArg): StmtAst;
partial record DeclVarAst(Enum vis, TypeAst type, string name): StmtAst;
partial record VarAst(Enum vis, TypeAst type, string name, ExprAst initializer): StmtAst;

partial record ExprStmtAst(ExprAst expr): StmtAst;


partial record ExprAst: Ast;

partial record LiteralExprAst(): ExprAst;
partial record IntLiteralExprAst(ulong value): LiteralExprAst;
partial record BoolLiteralExprAst(bool value): LiteralExprAst;
partial record StrLiteralExprAst(string value): LiteralExprAst;
partial record CharLiteralExprAst(ulong value, int nBits): LiteralExprAst;
partial record FloatLiteralExprAst(double value): LiteralExprAst;

partial record CallExprAst(ExprAst ptr, ExprAst[] args): ExprAst;
partial record VarExprAst(string varName): ExprAst;
partial record FuncPtrAst(string funcName): ExprAst;
partial record MemberExprAst(ExprAst container, string memberName): ExprAst;

partial record ArrayExprAst(ExprAst[] elements): ExprAst;
partial record IndexExprAst(ExprAst ptr, ExprAst index): ExprAst;
partial record CastExprAst(ExprAst value, TypeAst to): ExprAst;
partial record BitCastExprAst(ExprAst value, TypeAst to): ExprAst;

partial record UnExprAst(Enum op, ExprAst operand): ExprAst;
partial record BiExprAst(Enum op, ExprAst left, ExprAst right): ExprAst;