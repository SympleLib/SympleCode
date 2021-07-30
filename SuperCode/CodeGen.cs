using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Linq;

using LLVMSharp.Interop;

namespace SuperCode
{
	public class CodeGen
	{
		private LLVMValueRef func;
		private readonly Dictionary<Symbol, LLVMValueRef> syms = new();

		public readonly ModuleNode modNode;
		public readonly LLVMModuleRef module;
		private readonly LLVMBuilderRef builder;
		private readonly LLVMDIBuilderRef dbuilder;
		private LLVMMetadataRef file;

		public CodeGen(ModuleNode mod)
		{
			modNode = mod;
			module = LLVMModuleRef.CreateWithName(mod.filename);
			builder = LLVMBuilderRef.Create(module.Context);
			dbuilder = module.CreateDIBuilder();
		}

		public LLVMModuleRef Gen()
		{
			file = dbuilder.CreateFile(modNode.filename, "");
			dbuilder.CreateCompileUnit(LLVMDWARFSourceLanguage.LLVMDWARFSourceLanguageC, file,
				"SuperCode d1.0.0", 0, "", 0, "", LLVMDWARFEmissionKind.LLVMDWARFEmissionFull, 0, 0, 1, "", "");
			foreach (var mem in modNode.mems)
				Gen(mem);
			dbuilder.DIBuilderFinalize();
			return module;
		}

		public void Optimize()
		{
			var pass = LLVMPassManagerRef.Create();
			pass.AddGVNPass();
			pass.AddReassociatePass();
			pass.AddCFGSimplificationPass();
			pass.AddInstructionCombiningPass();
			pass.InitializeFunctionPassManager();
			pass.Run(module);
		}

		private LLVMValueRef Gen(Node node)
		{
			if (node is null)
				return null;

			switch (node.kind)
			{
			case NodeKind.DeclFuncMem:
				return Gen((DeclFuncMemNode) node);
			case NodeKind.FuncMem:
				return Gen((FuncMemNode) node);
			case NodeKind.StructMem:
				return Gen((StructMemNode) node);
			case NodeKind.VarMem:
				return Gen((VarMemNode) node);

			case NodeKind.BlockStmt:
				return Gen((BlockStmtNode) node);
			case NodeKind.IfStmt:
				return Gen((IfStmtNode) node);
			case NodeKind.RetStmt:
				return Gen((RetStmtNode) node);
			case NodeKind.TypedefStmt:
				return Gen((TypedefStmtNode) node);
			case NodeKind.VarStmt:
				return Gen((VarStmtNode) node);

			case NodeKind.AssignExpr:
				return Gen((AssignExprNode) node);
			case NodeKind.BinExpr:
				return Gen((BinExprNode) node);
			case NodeKind.CallExpr:
				return Gen((CallExprNode) node);
			case NodeKind.CastExpr:
				return Gen((CastExprNode) node);
			case NodeKind.FNumExpr:
				return Gen((FNumExprNode) node);
			case NodeKind.MemExpr:
				return Gen((MemExprNode) node);
			case NodeKind.NumExpr:
				return Gen((NumExprNode) node);
			case NodeKind.StrExpr:
				return Gen((StrExprNode) node);
			case NodeKind.SymExpr:
				return Gen((SymExprNode) node);
			case NodeKind.TypePunExpr:
				return Gen((TypePunExprNode) node);
			case NodeKind.UnExpr:
				return Gen((UnExprNode) node);

			default:
				throw new InvalidOperationException("Invalid node");
			}
		}

		private LLVMValueRef Gen(DeclFuncMemNode mem)
		{
			var fn = module.AddFunction(mem.name, mem.type);
			syms.Add(mem, fn);
			return fn;
		}

		private LLVMValueRef Gen(FuncMemNode mem)
		{
			func = module.AddFunction(mem.name, mem.type);
			var entry = func.AppendBasicBlock();
			builder.PositionAtEnd(entry);
			syms.Add(mem, func);

			for (int i = 0; i < mem.paramz.Length; i++)
				syms.Add(mem.paramz[i], func.Params[i]);

			foreach (var stmt in mem.stmts)
				Gen(stmt);

			if (mem.type.ReturnType == LLVMTypeRef.Void)
				builder.BuildRetVoid();
			var fn = func;
			func = null;
			return fn;
		}

		private LLVMValueRef Gen(StructMemNode node) =>
			null;

		private LLVMValueRef Gen(VarMemNode node)
		{
			var var = module.AddGlobal(node.type, node.name);
			if (node.init is null)
				var.Initializer = LLVMValueRef.CreateConstNull(node.type);
			else
				var.Initializer = Gen(node.init);
			syms.Add(node, var);
			return var;
		}


		private LLVMValueRef Gen(BlockStmtNode node)
		{
			foreach (var stmt in node.stmts)
				Gen(stmt);
			return null;
		}

		private LLVMValueRef Gen(IfStmtNode node)
		{
			bool hasElse = node.elze != default;

			var cond = Gen(node.cond);
			var then = func.AppendBasicBlock();
			var elze = hasElse ? func.AppendBasicBlock() : default;
			var end = func.AppendBasicBlock();
			var branch = builder.BuildCondBr(cond, then, hasElse ? elze : end);

			builder.PositionAtEnd(then);
			foreach (var stmt in node.then)
				Gen(stmt);
			builder.BuildBr(end);

			if (hasElse)
			{
				builder.PositionAtEnd(elze);
				Gen(node.elze);
				builder.BuildBr(end);
			}

			builder.PositionAtEnd(end);
			return branch;
		}

		private LLVMValueRef Gen(RetStmtNode node) =>
			builder.BuildRet(Gen(node.value));

		private LLVMValueRef Gen(TypedefStmtNode node) =>
			null;

		private LLVMValueRef Gen(VarStmtNode node)
		{
			var ptr = builder.BuildAlloca(node.type, node.name);
			var init = Gen(node.init);
			if (init != null)
				builder.BuildStore(init, ptr);
			syms.Add(node, ptr);
			return ptr;
		}


		private LLVMValueRef Gen(AssignExprNode node)
		{
			var val = GenAddr(node.value);
			var expr = Gen(node.expr);
			return builder.BuildStore(expr, val);
		}

		private LLVMValueRef Gen(BinExprNode node)
		{
			var left = Gen(node.left);
			var right = Gen(node.right);

			switch (node.op)
			{
			case BinOp.Add:
				return builder.BuildAdd(left, right);
			case BinOp.Sub:
				return builder.BuildSub(left, right);
			case BinOp.Mul:
				return builder.BuildMul(left, right);
			case BinOp.SDiv:
				return builder.BuildSDiv(left, right);
			case BinOp.SMod:
				return builder.BuildSRem(left, right);
			case BinOp.UDiv:
				return builder.BuildUDiv(left, right);
			case BinOp.UMod:
				return builder.BuildURem(left, right);

			case BinOp.FAdd:
				return builder.BuildFAdd(left, right);
			case BinOp.FSub:
				return builder.BuildFSub(left, right);
			case BinOp.FMul:
				return builder.BuildFMul(left, right);
			case BinOp.FDiv:
				return builder.BuildFDiv(left, right);
			case BinOp.FMod:
				return builder.BuildFRem(left, right);

			case BinOp.Eql:
				return builder.BuildICmp(LLVMIntPredicate.LLVMIntEQ, left, right);

			case BinOp.Index:
				return builder.BuildLoad(builder.BuildGEP(left, new LLVMValueRef[] { right }));

			default:
				throw new InvalidOperationException("Invalid bin-expr");
			}
		}

		private LLVMValueRef Gen(CallExprNode expr)
		{
			var what = Gen(expr.what);
			var args = new LLVMValueRef[expr.args.Length];
			for (int i = 0; i < args.Length; i++)
				args[i] = Gen(expr.args[i]);

			return builder.BuildCall(what, args);
		}

		private LLVMValueRef Gen(CastExprNode expr)
		{
			var val = Gen(expr.value);
			var from = val.TypeOf;
			var to = expr.type;
			if (from.IsFloat() && !to.IsFloat())
				return builder.BuildFPToSI(val, to);
			if (!from.IsFloat() && to.IsFloat())
				return builder.BuildSIToFP(val, to);
			if (from.IsFloat() && to.IsFloat())
				return builder.BuildFPCast(val, to);

			if (from.IsPtr() && to.IsPtr())
				return builder.BuildPointerCast(val, to);
			if (from.IsPtr() && !to.IsPtr())
				return builder.BuildPtrToInt(val, to);
			if (!from.IsPtr() && to.IsPtr())
				return builder.BuildIntToPtr(val, to);
			return builder.BuildIntCast(val, to); ;
		}

		private LLVMValueRef Gen(FNumExprNode node) =>
			LLVMValueRef.CreateConstReal(node.type, node.value);

		private LLVMValueRef Gen(MemExprNode node)
		{
			var ptr = GenAddr(node.expr);
			var field = builder.BuildStructGEP(ptr, (uint) node.index);
			return builder.BuildLoad(field);
		}

		private LLVMValueRef Gen(NumExprNode node) =>
			LLVMValueRef.CreateConstInt(node.type, node.value);

		private LLVMValueRef Gen(StrExprNode node)
		{
			switch (node.strType)
			{
			case StrType.Short:
			case StrType.Unicode:
			case StrType.Wide:
			{
				var values = new LLVMValueRef[node.str.Length + 1];
				for (int i = 0; i < node.str.Length; i++)
					values[i] = LLVMValueRef.CreateConstInt(node.type.ElementType, node.str[i]);
				values[values.Length - 1] = LLVMValueRef.CreateConstInt(node.type.ElementType, 0);
				var arr = LLVMValueRef.CreateConstArray(node.type.ElementType, values);

				var str = module.AddGlobal(arr.TypeOf, "..str");
				str.Linkage = LLVMLinkage.LLVMPrivateLinkage;
				str.HasUnnamedAddr = true;
				str.Initializer = arr;
				return builder.BuildBitCast(str, node.type);
			}

			default:
				throw new InvalidOperationException("Invalid str-type");
			}
		}

		private LLVMValueRef Gen(SymExprNode expr)
		{
			var sym = syms[expr.symbol];
			var symbol = (Node) expr.symbol;
			if (symbol.kind is NodeKind.VarStmt or NodeKind.VarMem)
				return builder.BuildLoad(sym);
			return sym;
		}

		private LLVMValueRef Gen(TypePunExprNode node)
		{
			var expr = Gen(node.expr);
			var fptr = builder.BuildAlloca(expr.TypeOf);
			builder.BuildStore(expr, fptr);
			var tptr = builder.BuildPointerCast(fptr, node.type.Ref());
			return builder.BuildLoad(tptr);
		}

		private LLVMValueRef Gen(UnExprNode node)
		{
			if (node.op is UnOp.Ref)
				return GenAddr(node.expr);

			var expr = Gen(node.expr);

			switch (node.op)
			{
			case UnOp.Neg:
				return builder.BuildNeg(expr);
			case UnOp.FNeg:
				return builder.BuildFNeg(expr);
			case UnOp.Deref:
				return builder.BuildLoad(expr);

			default:
				throw new InvalidOperationException("Invalid un-expr");
			}
		}


		private LLVMValueRef GenAddr(Node node)
		{
			switch (node.kind)
			{
			case NodeKind.MemExpr:
				return GenAddr((MemExprNode) node);
			case NodeKind.SymExpr:
				return GenAddr((SymExprNode) node);
			case NodeKind.UnExpr:
				return GenAddr((UnExprNode) node);

			default:
				// TODO: PermaSafe
				throw new InvalidOperationException("Not an addr");
			}
		}

		private LLVMValueRef GenAddr(MemExprNode node)
		{
			var ptr = GenAddr(node.expr);
			return builder.BuildStructGEP(ptr, (uint) node.index);
		}

		private LLVMValueRef GenAddr(SymExprNode node) =>
			syms[node.symbol];

		private LLVMValueRef GenAddr(UnExprNode node)
		{
			if (node.op is not UnOp.Deref)
				// TODO: PermaSafe
				throw new InvalidOperationException("Not an addr");
			return Gen(node.expr);
		}
	}
}
