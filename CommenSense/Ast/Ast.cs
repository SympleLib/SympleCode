namespace CommenSense;

partial record Ast(Token token);
partial record ModuleAst(string name, StmtAst[] members, Container[] ctnrs);

partial record TypeAst(int ptrCount, Token token): Ast(token);
partial record BaseTypeAst(Token token, int ptrCount): TypeAst(ptrCount, token);
partial record FuncTypeAst(TypeAst retType, Token token, TypeAst[] paramTypes, bool vaArg, int ptrCount): TypeAst(ptrCount, token);
partial record ParamAst(TypeAst type, Token token, string name, ExprAst defaultExpr): Ast(token);
partial record FieldAst(Visibility visibility, TypeAst type, Token token, ExprAst initializer): Ast(token);


partial record StmtAst(Token token): Ast(token);

interface Container
{
	public Token token { get; }
	public string name { get; }
	public FieldAst[] fields { get; }

	public uint GetField(string name);
	public uint GetFieldWithLvl(string name, Visibility accessVis);
}

partial record ImplAst(Visibility visibility, Token token, FuncAst[] funcs): StmtAst(token);
partial record StructAst(Visibility visibility, Token token, FieldAst[] fields): StmtAst(token), Container;
partial record ClassAst(Visibility visibility, Token token, FieldAst[] fields, FuncAst[] funcs): StmtAst(token), Container;

partial record UsingAst(Visibility visibility, Token token, TypeAst realType, string alias): StmtAst(token);
partial record LinkAst(Visibility visibility, Token token, string filename): StmtAst(token);

partial record DeclFuncAst(Visibility visibility, TypeAst retType, Token token, string asmName, ParamAst[] paramz, bool vaArg): StmtAst(token);
partial record FuncAst(Visibility visibility, TypeAst retType, Token token, string asmName, ParamAst[] paramz, bool vaArg, StmtAst[] body): StmtAst(token);
partial record DeclVarAst(Visibility visibility, TypeAst type, Token token, string asmName): StmtAst(token);
partial record VarAst(Visibility visibility, TypeAst type, Token token, string asmName, ExprAst initializer): StmtAst(token);

partial record IfStmtAst(Token token, ExprAst cond, StmtAst then, StmtAst? elze): StmtAst(token);
partial record WhileStmtAst(Token token, ExprAst cond, StmtAst then): StmtAst(token);
partial record ForStmtAst(Token token, StmtAst init, ExprAst cond, ExprAst step, StmtAst then): StmtAst(token);
partial record BlockStmtAst(Token token, StmtAst[] stmts): StmtAst(token);

partial record BreakStmtAst(Token token, int depth): StmtAst(token);
partial record BreakAllStmtAst(Token token): StmtAst(token);
partial record RetStmtAst(Token token, ExprAst expr): StmtAst(token);
partial record ExprStmtAst(ExprAst expr): StmtAst(expr.token);


partial record ExprAst(Token token): Ast(token);

partial record LiteralExprAst(Token token): ExprAst(token);
partial record IntLiteralExprAst(Token token, ulong value): LiteralExprAst(token);
partial record BoolLiteralExprAst(Token token, bool value): LiteralExprAst(token);
partial record StrLiteralExprAst(Token token, string value): LiteralExprAst(token);
partial record CharLiteralExprAst(Token token, ulong value, int nBits): LiteralExprAst(token);
partial record FloatLiteralExprAst(Token token, double value): LiteralExprAst(token);

partial record CallExprAst(ExprAst ptr, Token token, ExprAst[] args): ExprAst(token);
partial record VarExprAst(Token token): ExprAst(token);
partial record FuncPtrAst(Token token): ExprAst(token);
partial record MemberExprAst(ExprAst container, Token token): ExprAst(token);

partial record ArrayExprAst(TypeAst? eleType, Token token, ExprAst[] elements): ExprAst(token);
partial record GroupExprAst(TypeAst groupType, Token token, ExprAst[] members): ExprAst(token);
partial record IndexExprAst(ExprAst ptr, Token token, ExprAst index): ExprAst(token);
partial record CastExprAst(ExprAst value, Token token, TypeAst to): ExprAst(token);
partial record BitCastExprAst(ExprAst value, Token token, TypeAst to): ExprAst(token);

partial record PreExprAst(Enum op, Token token, ExprAst operand): ExprAst(token);
partial record PostExprAst(Token token, Enum op, ExprAst operand): ExprAst(token);
partial record BiExprAst(Enum op, Token token, ExprAst left, ExprAst right): ExprAst(token);