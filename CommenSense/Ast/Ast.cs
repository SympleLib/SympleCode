namespace CommenSense;

partial record Ast;
partial record ModuleAst(string name, StmtAst[] members, StructAst[] structs, ClassAst[] classes);

partial record TypeAst(int ptrCount): Ast;
partial record BaseTypeAst(string typeBase, int ptrCount): TypeAst(ptrCount);
partial record FuncTypeAst(TypeAst retType, TypeAst[] paramTypes, bool vaArg, int ptrCount): TypeAst(ptrCount);
partial record ParamAst(TypeAst type, string name, ExprAst defaultExpr): Ast;
partial record FieldAst(Visibility visibility, TypeAst type, string name, ExprAst initializer): Ast;


partial record StmtAst: Ast;

partial record ImplAst(Visibility visibility, string name, FuncAst[] funcs): StmtAst;
partial record StructAst(Visibility visibility, string name, FieldAst[] fields): StmtAst;
partial record ClassAst(Visibility visibility, string name, FieldAst[] fields, FuncAst[] funcs): StmtAst;

partial record DeclFuncAst(Visibility visibility, TypeAst retType, string realName, string asmName, ParamAst[] paramz, bool vaArg): StmtAst;
partial record FuncAst(Visibility visibility, TypeAst retType, string realName, string asmName, ParamAst[] paramz, bool vaArg, StmtAst[] body): StmtAst;
partial record DeclVarAst(Visibility visibility, TypeAst type, string realName, string asmName): StmtAst;
partial record VarAst(Visibility visibility, TypeAst type, string realName, string asmName, ExprAst initializer): StmtAst;

partial record RetStmtAst(ExprAst expr): StmtAst;
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

partial record ArrayExprAst(TypeAst? eleType, ExprAst[] elements): ExprAst;
partial record GroupExprAst(TypeAst groupType, ExprAst[] members): ExprAst;
partial record IndexExprAst(ExprAst ptr, ExprAst index): ExprAst;
partial record CastExprAst(ExprAst value, TypeAst to): ExprAst;
partial record BitCastExprAst(ExprAst value, TypeAst to): ExprAst;

partial record UnExprAst(Enum op, ExprAst operand): ExprAst;
partial record BiExprAst(Enum op, ExprAst left, ExprAst right): ExprAst;