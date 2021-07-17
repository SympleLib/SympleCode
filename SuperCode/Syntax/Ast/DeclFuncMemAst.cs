using System;

namespace SuperCode.Syntax.Ast
{
	// Imagine if I typed the whole thing out: DeclareFunctionMemberAbstractSyntaxTree. Ope, no need to imagine!
	public class DeclFuncMemAst: MemAst
	{
		public readonly Token key;
		public readonly TypeAst retType;
		public readonly Token ret;

		public override AstKind kind => throw new NotImplementedException();
	}
}
