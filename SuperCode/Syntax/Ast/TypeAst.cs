using LLVMSharp.Interop;

namespace SuperCode
{
	public class TypeAst: Ast
	{
		public readonly Token baze;
		public readonly Token[] addons;

		public TypeAst(Token baze, Token[] addons)
		{
			this.baze = baze;
			this.addons = addons;
		}

		public override AstKind kind => AstKind.Type;
		public LLVMTypeRef builtinType => baze.builtinType;
	}
}
