using System;
using System.Collections.Generic;

namespace SuperCode
{
	public class FuncMemAst: MemAst
	{
		public readonly Token key;
		public readonly Token name;
		public readonly Token arrow;
		public readonly StmtAst[] stmts;

		public FuncMemAst(Token key, Token name, Token arrow, StmtAst[] stmts)
		{
			this.key = key;
			this.name = name;
			this.arrow = arrow;
			this.stmts = stmts;
		}
	}
}
