using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public partial class Noder
	{
		public void Symbolize()
		{
			foreach (var mem in module.mems)
				Symbolize(mem);
		}

		public void Symbolize(MemAst ast)
		{
			switch (ast.kind)
			{
			case AstKind.DeclFuncMem:
				Symbolize((DeclFuncMemAst) ast);
				break;
			case AstKind.ImportMem:
				Symbolize((ImportMemAst) ast);
				break;
			case AstKind.FuncMem:
				Symbolize((FuncMemAst) ast);
				break;
			case AstKind.StructMem:
				Symbolize((StructMemAst) ast);
				break;
			case AstKind.VarMem:
				Symbolize((VarMemAst) ast);
				break;

			default:
				throw new InvalidOperationException("Invalid mem");
			}
		}

		public void Symbolize(DeclFuncMemAst ast)
		{
			var paramTypes = new LLVMTypeRef[ast.paramz.Length];
			for (int i = 0; i < paramTypes.Length; i++)
				paramTypes[i] = Type(ast.paramz[i].type);

			var ty = LLVMTypeRef.CreateFunction(Type(ast.retType), paramTypes, ast.vaArg.HasValue);
			string name = ast.name.text;
			string asmTag = ast.asmTag.HasValue ? ast.asmTag.Value.text : name;
			var node = new DeclFuncMemNode(ty, asmTag);
			syms.Add(name, node);
		}

		public void Symbolize(ImportMemAst ast)
		{
			string what = ast.what.text;
			syc.Symbolize(what);
		}
	}
}
