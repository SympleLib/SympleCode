using System;
using System.Collections.Generic;

using LLVMSharp.Interop;

namespace SuperCode
{
	public partial class Noder
	{
		public readonly PermaSafe safety = new PermaSafe();
		public readonly ModuleAst module;
		public readonly LLVMContextRef ctx;

		private Dictionary<string, Symbol> syms = new Dictionary<string, Symbol>();
		private readonly Dictionary<string, LLVMTypeRef> types = new Dictionary<string, LLVMTypeRef>();
		private readonly Dictionary<LLVMTypeRef, StructMemNode> ztructs = new Dictionary<LLVMTypeRef, StructMemNode>();
		private readonly SympleCode syc;
		private LLVMTypeRef retType;

		public Noder(SympleCode syc, ModuleAst module)
		{
			this.syc = syc;
			this.module = module;
			ctx = LLVMContextRef.Global;
		}

		public PermaSafe Nodify(out ModuleNode node)
		{
			var mems = new Node[module.mems.Length];
			for (int i = 0; i < mems.Length; i++)
				mems[i] = Nodify(module.mems[i]);
			node = new ModuleNode(module.filename, mems, syms) { syntax = module };
			return safety;
		}

		private ExprNode Nodify(ElementAst ast) =>
			Nodify(ast.value);

		private LLVMTypeRef Type(TypeAst ast)
		{
			LLVMTypeRef ty;
			if (ast.baze.isBuiltinType)
				ty = ast.baze.builtinType;
			else
				ty = types[ast.baze.text];
			
			if (ast.args.Length > 0)
			{
				var args = new LLVMTypeRef[ast.args.Length];
				for (int i = 0; i < args.Length; i++)
					args[i] = Type(ast.args[i]);
				ty = LLVMTypeRef.CreateFunction(ty, args);
			}
			
			if (ast.addons.Length > 0 && !syc.allUnsafe)
					safety.ReportEvilPtr(ast);
			for (int i = 0; i < ast.addons.Length; i++)
				ty = ty.Ptr();
			if (ast.refTok is not null)
				ty = ty.Ref();
			return ty;
		}

		private FieldNode Nodify(FieldAst ast, int index) =>
			new FieldNode(Type(ast.type), IsMut(ast.mutKey), index, ast.name.text) { syntax = ast };


		private ExprNode Cast(ExprNode node, LLVMTypeRef to)
		{
			if (to == default || node.type == to)
				return node;

			if (to.IntWidth < node.type.IntWidth && !to.IsFloat() && !to.IsPtr())
				safety.ReportPossibleLossOfData(node.syntax!.token);
			return new CastExprNode(node, to);
		}

		private FieldNode? Field(LLVMTypeRef ty, Token name)
		{
			if (ty == null)
				return null;
			var ztruct = ztructs[ty];
			foreach (var field in ztruct.fields)
				if (field.name == name.text)
					return field;
			safety.ReportNSField(name);
			return null;
		}

		private bool IsMut(Token? mutKey)
		{
			if (mutKey is null)
				return !syc.defConst;
			return mutKey.Value.kind is TokenKind.MutKey;

			throw new InvalidOperationException("Can't deduce whether mutable");
		}

		private Visibility Vis(Token? token)
		{
			if (token is null)
				return syc.defVis;
			switch (token?.kind)
			{
			case TokenKind.PubKey:
				return Visibility.Public;
			case TokenKind.ProtKey:
				return Visibility.Protected;
			case TokenKind.PrivKey:
				return Visibility.Private;

			default:
				return syc.defVis;
			}
		}
	}
}
