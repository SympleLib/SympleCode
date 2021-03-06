﻿using System;
using System.Collections.Generic;

namespace SuperCode
{
	public class FuncMemAst: MemAst
	{
		public readonly TypeAst retType;
		public readonly Token name;

		public readonly Token openArg;
		public readonly Token closeArg;

		public readonly Token arrow;

		public readonly Token open;
		public readonly ParamAst[] paramz;
		public readonly Token close;

		public readonly StmtAst[] stmts;

		public FuncMemAst(TypeAst retType, Token name, Token openArg, ParamAst[] paramz, Token closeArg, Token arrow, StmtAst[] stmts)
		{
			token = name;

			this.retType = retType;
			this.name = name;

			this.openArg = openArg;
			this.paramz = paramz;
			this.closeArg = closeArg;

			this.arrow = arrow;
			this.stmts = stmts;
		}

		public FuncMemAst(TypeAst retType, Token name, Token openArg, ParamAst[] paramz, Token closeArg, Token open, Token close, StmtAst[] stmts)
		{
			token = name;

			this.retType = retType;
			this.name = name;

			this.openArg = openArg;
			this.paramz = paramz;
			this.closeArg = closeArg;

			this.open = open;
			this.close = close;
			this.stmts = stmts;
		}

		public override AstKind kind => AstKind.FuncMem;
	}
}
