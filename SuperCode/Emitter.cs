using LLVMSharp.Interop;
using System;

namespace SuperCode
{
	public class Emitter
	{
		public readonly LLVMModuleRef module;
		public readonly Ast tree;

		public LLVMValueRef mainFn { get; private set; }
		public LLVMValueRef runFn { get; private set; }

		private readonly LLVMBuilderRef builder;

		public Emitter(Ast ast)
		{
			tree = ast;

			module = LLVMModuleRef.CreateWithName("SympleCode");
			builder = LLVMBuilderRef.Create(module.Context);
		}

		private void EmitRunFn()
		{
			var fnTy = LLVMTypeRef.CreateFunction(LLVMTypeRef.Int32, Array.Empty<LLVMTypeRef>());
			runFn = module.AddFunction("TypePunToFloat", fnTy);
			var entry = runFn.AppendBasicBlock("Entry");
			builder.PositionAtEnd(entry);

			var fret = builder.BuildCall(mainFn, Array.Empty<LLVMValueRef>());
			var fptr = builder.BuildAlloca(LLVMTypeRef.Float);
			builder.BuildStore(fret, fptr);
			var iptr = builder.BuildBitCast(fptr, LLVMTypeRef.CreatePointer(LLVMTypeRef.Int32, 0));
			var iret = builder.BuildLoad(iptr);
			builder.BuildRet(iret);
		}

		public LLVMModuleRef Emit()
		{
			var fnTy = LLVMTypeRef.CreateFunction(LLVMTypeRef.Float, Array.Empty<LLVMTypeRef>());
			mainFn = module.AddFunction("Main", fnTy);
			var entry = mainFn.AppendBasicBlock("Entry");

			builder.PositionAtEnd(entry);
			var ret = Emit((ExprAst) tree);
			builder.BuildRet(ret);

			EmitRunFn();
			return module;
		}

		private LLVMValueRef Emit(ExprAst expr) =>
			expr.kind switch
			{
				AstKind.LitExpr =>
					Emit((LitExprAst) expr),
				AstKind.BinExpr =>
					Emit((BinExprAst) expr),

				_ =>
					throw new Exception("Unknown expr"),
			};

		private static LLVMValueRef Emit(LitExprAst expr) =>
			expr.literal.kind switch
			{
				TokenKind.Number =>
					LLVMValueRef.CreateConstReal(LLVMTypeRef.Float, float.Parse(expr.literal.text)),

				_ =>
					throw new Exception("Unknown lit-expr"),
			};

		private LLVMValueRef Emit(BinExprAst expr) =>
			expr.op.kind switch
			{
				TokenKind.Plus =>
					builder.BuildFAdd(Emit(expr.left), Emit(expr.right)),
				TokenKind.Minus =>
					builder.BuildFSub(Emit(expr.left), Emit(expr.right)),
				TokenKind.Star =>
					builder.BuildFMul(Emit(expr.left), Emit(expr.right)),
				TokenKind.Slash =>
					builder.BuildFDiv(Emit(expr.left), Emit(expr.right)),
				TokenKind.Percent =>
					builder.BuildFRem(Emit(expr.left), Emit(expr.right)),

				_ =>
					throw new Exception("Unknown bin-expr"),
			};
	}
}
