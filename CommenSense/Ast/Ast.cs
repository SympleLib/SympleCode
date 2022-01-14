namespace CommenSense;

partial record Ast(Token token);
partial record ModuleAst(string name, string srcFile, StmtAst[] members, Container[] ctnrs);

abstract partial record TypeAst(Token token): Ast(token);

partial record PtrTypeAst(TypeAst baze, bool mutable, Token token): TypeAst(token);
partial record ArrayTypeAst(TypeAst baze, bool mutable,Token token, ExprAst length): TypeAst(token);
partial record BaseTypeAst(Token token): TypeAst(token);
partial record FuncTypeAst(TypeAst retType, Token token, TypeAst[] paramTypes, bool vaArg): TypeAst(token);
partial record ParamAst(string[] metadata, bool mutable, TypeAst type, Token token, string name, ExprAst defaultExpr): Ast(token);
partial record FieldAst(string[] metadata, Visibility visibility, bool mutable, TypeAst type, Token token, ExprAst initializer): Ast(token);
partial record EnumValueAst(string[] metadata, Token token, string asmName, ExprAst initializer): Ast(token);


partial record StmtAst(Token token): Ast(token);

readonly record struct FieldInfo(uint idx, bool mutable);
interface Container
{
	public Token token { get; }
	public string name { get; }
	public FieldAst[] fields { get; }

	public FieldInfo GetField(string name);
	public FieldInfo GetFieldWithLvl(string name, Visibility accessVis);
}

partial record ImplAst(string[] metadata, Visibility visibility, Token token, FuncAst[] funcs): StmtAst(token);
partial record EnumAst(string[] metadata, Visibility visibility, Token token, TypeAst type, EnumValueAst[] values): StmtAst(token);
partial record StructAst(string[] metadata, Visibility visibility, Token token, FieldAst[] fields): StmtAst(token), Container;
partial record ClassAst(string[] metadata, Visibility visibility, Token token, FieldAst[] fields, FuncAst[] funcs): StmtAst(token), Container;

partial record UsingAst(Visibility visibility, Token token, TypeAst realType, string alias): StmtAst(token);
partial record LinkAst(Visibility visibility, Token token, string filename): StmtAst(token);

partial record DeclFuncAst(string[] metadata, Visibility visibility, bool mutable, TypeAst retType, Token token, string asmName, ParamAst[] paramz, bool vaArg): StmtAst(token);
partial record FuncAst(string[] metadata, Visibility visibility, bool mutable, TypeAst retType, Token token, string asmName, ParamAst[] paramz, bool vaArg, StmtAst[] body): StmtAst(token);
partial record DeclVarAst(string[] metadata, Visibility visibility, bool mutable, TypeAst type, Token token, string asmName): StmtAst(token);
partial record VarAst(string[] metadata, Visibility visibility, bool mutable, TypeAst type, Token token, string asmName, ExprAst initializer): StmtAst(token);

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
partial record NullLiteralExprAst(Token token): LiteralExprAst(token);
partial record BoolLiteralExprAst(Token token, bool value): LiteralExprAst(token);
partial record StrLiteralExprAst(Token token, string value): LiteralExprAst(token);
partial record CStrLiteralExprAst(Token token, string value): LiteralExprAst(token);
partial record CharLiteralExprAst(Token token, ulong value, int nBits): LiteralExprAst(token);
partial record FloatLiteralExprAst(Token token, double value): LiteralExprAst(token);

partial record StallocExprAst(Token token, ExprAst size): ExprAst(token);
partial record SizeofExprAst(Token token, TypeAst type): ExprAst(token);
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