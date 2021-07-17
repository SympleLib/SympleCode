using LLVMSharp.Interop;

namespace SuperCode
{
	public class TypeAst: Ast
	{
		public readonly Token baze;

		public TypeAst(Token baze) =>
			this.baze = baze;

		public override AstKind kind => AstKind.Type;

		public LLVMTypeRef builtinType => baze.builtinType;
	}
}
