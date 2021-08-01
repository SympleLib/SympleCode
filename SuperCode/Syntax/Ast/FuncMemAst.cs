using System;
using System.Collections.Generic;

namespace SuperCode
{
	public class FuncMemAst: MemAst
	{
		public readonly TypeAst retType;
		public readonly Token name;
		public readonly Token? asmTag;

		public readonly Token openArg;
		public readonly FieldAst[] paramz;
		public readonly Token? vaArg;
		public readonly Token closeArg;

		public readonly Token? arrow;

		public readonly Token? open;
		public readonly StmtAst[] stmts;
		public readonly Token? close;

		public FuncMemAst(TypeAst retType, Token name, Token? asmTag, Token openArg, FieldAst[] paramz, Token? vaArg, Token closeArg, Token arrow, StmtAst[] stmts)
		{
			token = name;

			this.retType = retType;
			this.name = name;
			this.asmTag = asmTag;

			this.openArg = openArg;
			this.paramz = paramz;
			this.vaArg = vaArg;
			this.closeArg = closeArg;

			this.arrow = arrow;
			this.stmts = stmts;
		}

		public FuncMemAst(TypeAst retType, Token name, Token? asmTag, Token openArg, FieldAst[] paramz, Token? vaArg, Token closeArg, Token open, Token close, StmtAst[] stmts)
		{
			token = name;

			this.retType = retType;
			this.name = name;
			this.asmTag = asmTag;

			this.openArg = openArg;
			this.paramz = paramz;
			this.vaArg = vaArg;
			this.closeArg = closeArg;

			this.open = open;
			this.close = close;
			this.stmts = stmts;
		}

		public override AstKind kind => AstKind.FuncMem;
	}
}
