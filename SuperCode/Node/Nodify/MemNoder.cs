using LLVMSharp.Interop;
using System.Collections.Generic;
using System;

using static SuperCode.BuiltinTypes;

namespace SuperCode
{
	public partial class Noder
	{
		private Node Nodify(MemAst ast)
		{
			switch (ast.kind)
			{
			case AstKind.DeclFuncMem:
				return Nodify((DeclFuncMemAst) ast);
			case AstKind.ImportMem:
				return Nodify((ImportMemAst) ast);
			case AstKind.FuncMem:
				return Nodify((FuncMemAst) ast);
			case AstKind.StmtMem:
				return Nodify((StmtMemAst) ast);
			case AstKind.StructMem:
				return Nodify((StructMemAst) ast);
			case AstKind.VarMem:
				return Nodify((VarMemAst) ast);

			default:
				throw new InvalidOperationException("Invalid mem");
			}
		}

		private DeclFuncMemNode Nodify(DeclFuncMemAst ast)
		{
			var paramTypes = new LLVMTypeRef[ast.paramz.Length];
			var paramz = new FieldNode[ast.paramz.Length];
			for (int i = 0; i < paramz.Length; i++)
			{
				var param = Nodify(ast.paramz[i], i);
				paramz[i] = param;
				paramTypes[i] = param.type;
			}

			retType = Type(ast.retType);
			var ty = LLVMTypeRef.CreateFunction(retType, paramTypes, ast.vaArg != default);
			string name = ast.name.text;
			if (ast.asmTag is not null)
			{
				var asmTag = ast.asmTag.Value;
				name = asmTag.text;
				if (asmTag.kind is TokenKind.Str)
					name = name[1..^1];
			}

			var decl = new DeclFuncMemNode(ty, name, paramz) { syntax = ast };
			syms.TryAdd(ast.name.text, decl);
			return decl;
		}

		private ImportMemNode Nodify(ImportMemAst ast)
		{
			string what = ast.what.text[1..^1];
			var omodule = syc.Compile(what);
			if (omodule is null)
				throw new Exception("Module compiled with errors");
			foreach (var osym in omodule.symbols)
				syms.Add(osym.Key, osym.Value);
			return new ImportMemNode(what, omodule);
		}

		private FuncMemNode Nodify(FuncMemAst ast)
		{
			var psyms = new Dictionary<string, Symbol>(syms);

			var paramTypes = new LLVMTypeRef[ast.paramz.Length];
			for (int i = 0; i < paramTypes.Length; i++)
				paramTypes[i] = Type(ast.paramz[i].type);

			var paramz = new FieldNode[ast.paramz.Length];
			for (int i = 0; i < paramz.Length; i++)
			{
				var paramNode = Nodify(ast.paramz[i], i);
				paramz[i] = paramNode;
				syms.Add(paramNode.name, paramNode);
			}

			retType = Type(ast.retType);
			var ty = LLVMTypeRef.CreateFunction(retType, paramTypes, ast.vaArg != default);
			string name = ast.name.text;
			if (ast.asmTag is not null)
			{
				var asmTag = ast.asmTag.Value;
				name = asmTag.text;
				if (asmTag.kind is TokenKind.Str)
					name = name[1..^1];
			}

			var stmts = new Node[ast.stmts.Length];
			for (int i = 0; i < stmts.Length; i++)
				stmts[i] = Nodify(ast.stmts[i]);

			var func = new FuncMemNode(ty, Vis(ast.vis), name, paramz, stmts) { syntax = ast };
			syms = psyms;
			if (syms.ContainsKey(ast.name.text))
			{
				syms.Remove(ast.name.text, out var sym);
				((DeclFuncMemNode) sym!).impl = func;
			}
			syms.Add(ast.name.text, func);
			return func;
		}

		private Node Nodify(StmtMemAst ast) =>
			Nodify(ast.stmt);

		private StructMemNode Nodify(StructMemAst ast)
		{
			var fields = new FieldNode[ast.fields.Length];
			var fieldTypes = new LLVMTypeRef[ast.fields.Length];
			for (int i = 0; i < fields.Length; i++)
			{
				var field = Nodify(ast.fields[i], i);
				fields[i] = field;
				fieldTypes[i] = field.type;
			}

			string name = ast.name.text;
			var type = ctx.CreateNamedStruct(name);
			type.StructSetBody(fieldTypes, true);
			var node = new StructMemNode(type, name, fields);
			ztructs.Add(type, node);
			types.Add(name, type);
			return node;
		}

		private VarMemNode Nodify(VarMemAst ast)
		{
			var ty = Type(ast.type);
			var init = ast.init is null ? null : Nodify(ast.init, ty.IsRef() ? default : ty);
			var var = new VarMemNode(ty, Vis(ast.vis), IsMut(ast.mutKey), ast.name.text, init) { syntax = ast };
			syms.Add(var.name, var);
			return var;
		}
	}
}
