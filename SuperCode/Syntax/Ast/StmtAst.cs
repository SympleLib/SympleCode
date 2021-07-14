using System;

namespace SuperCode
{
	public class StmtAst: Ast
	{
		public readonly Token semicol;

		public StmtAst() {}
		public StmtAst(Token semi) =>
			semicol = semi;
	}
}
