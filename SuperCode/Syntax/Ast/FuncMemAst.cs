using System;
using System.Collections.Generic;

namespace SuperCode
{
	public class FuncMemAst: MemAst
	{
		public readonly Token key;
		public readonly Token name;

		public readonly Token openArg;
		public readonly Token closeArg;

		public readonly Token arrow;

		public readonly Token open;
		public readonly Token close;
		public readonly StmtAst[] stmts;

		public FuncMemAst(Token key, Token name, Token openArg, Token closeArg, Token arrow, StmtAst[] stmts)
		{
			this.key = key;
			this.name = name;

			this.openArg = openArg;
			this.closeArg = closeArg;

			this.arrow = arrow;
			this.stmts = stmts;
		}

		public FuncMemAst(Token key, Token name, Token openArg, Token closeArg, Token open, Token close, StmtAst[] stmts)
		{
			this.key = key;
			this.name = name;

			this.openArg = openArg;
			this.closeArg = closeArg;

			this.open = open;
			this.close = close;
			this.stmts = stmts;
		}

		public override AstKind kind => AstKind.FuncMem;
	}
}
