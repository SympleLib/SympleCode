//#define PRINT

using LLVMSharp.Interop;
using System.IO;
using System;
using System.Collections.Generic;

namespace SuperCode
{
	public enum SycMode
	{
		DefaultPrivate = 1 << 0,
		DefaultConstant = 1 << 1,
		AllUnsafe = 1 << 2,
		Optimize = 1 << 3,

		Default = DefaultPrivate | Optimize,
		Dev = AllUnsafe | Optimize,
		Industry = DefaultPrivate | DefaultConstant | Optimize,
	}

	public class SympleCode
	{
		public readonly SycMode mode;
		public readonly string dir;

		public bool defPriv => mode.HasFlag(SycMode.DefaultPrivate);
		public bool defConst => mode.HasFlag(SycMode.DefaultConstant);
		public bool allUnsafe => mode.HasFlag(SycMode.AllUnsafe);
		public bool optimize => mode.HasFlag(SycMode.Optimize);

		public Visibility defVis => defPriv ? Visibility.Private : Visibility.Public;

		public readonly List<ModuleNode> modules = new List<ModuleNode>();

		public LLVMExecutionEngineRef execEngine => engine;
		private LLVMExecutionEngineRef engine;

		public SympleCode(SycMode mode, string dir)
		{
			this.mode = mode;
			this.dir = dir;
		}

		public ModuleNode? CompileJIT(string file)
		{
			var module = Compile(file);
			if (module is null)
				return null;

			var options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
			if (!module.llmodule.TryCreateMCJITCompiler(out engine, ref options, out string err))
			{
				Console.Error.WriteLine(err);
				return null;
			}

			foreach (var omod in modules)
				if (omod != module)
					engine.AddModule(omod.llmodule);
			return module;
		}

		public ModuleNode? Compile(string file)
		{
			string filebase = dir + (file.Contains('.') ? file[(..file.LastIndexOf('.'))] : file);

			var parser = new Parser(dir + file);
#if PRINT
			SyntaxColors(parser.lexer.src, parser.tokens);
#endif
			Console.WriteLine();

			var safety = parser.Parse(out var tree);
			safety.Print(Console.Out);
			if (safety.MustSelfDestruct())
				return null;

			File.WriteAllText(filebase + ".tree", tree.ToString());
#if PRINT
			tree.Print(Console.Out);
			Console.WriteLine();
#endif

			LLVM.LinkInMCJIT();
			LLVM.InitializeNativeTarget();
			LLVM.InitializeNativeAsmParser();
			LLVM.InitializeNativeAsmPrinter();
			LLVM.InitializeNativeDisassembler();

			var noder = new Noder(this, tree);
			safety = noder.Nodify(out var node);
			safety.Print(Console.Out);
			if (safety.MustSelfDestruct())
				return null;

			var cg = new CodeGen(node);
			var module = cg.Gen();
			if (optimize)
				cg.Optimize();

			File.WriteAllText(filebase + ".ll", module.ToString());
#if PRINT
			Console.ForegroundColor = ConsoleColor.DarkGreen;
			Console.WriteLine(module);
#endif

			Console.ForegroundColor = ConsoleColor.White;

			if (!module.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string err))
			{
				Console.Error.WriteLine(err);
				return null;
			}

			var target = LLVMTargetRef.GetTargetFromTriple(LLVMTargetRef.DefaultTriple);
			var targetMachine = target.CreateTargetMachine(LLVMTargetRef.DefaultTriple, "generic", "",
				LLVMCodeGenOptLevel.LLVMCodeGenLevelAggressive, LLVMRelocMode.LLVMRelocDefault, LLVMCodeModel.LLVMCodeModelDefault);
			targetMachine.EmitToFile(module, filebase + ".o", LLVMCodeGenFileType.LLVMObjectFile);
			node.llmodule = module;
			modules.Add(node);
			return node;
		}

#if PRINT
		private static void SyntaxColors(string src, Token[] tokens)
		{
			int pos = 0;
			foreach (var tok in tokens)
			{
				while (pos < tok.pos)
					Console.Write(src[pos++]);

				switch (tok.kind)
				{
				case TokenKind.Iden:
					if (tok.isBuiltinType)
						Console.ForegroundColor = ConsoleColor.Magenta;
					else
						Console.ForegroundColor = ConsoleColor.White;
					break;
				case TokenKind.Str:
					Console.ForegroundColor = ConsoleColor.Green;
					break;
				case TokenKind.LineComment:
				case TokenKind.LongComment:
					Console.ForegroundColor = ConsoleColor.DarkGreen;
					break;
				case TokenKind.Num:
					Console.ForegroundColor = ConsoleColor.Yellow;
					break;

				default:
					if (tok.kind >= TokenFacts.firstKey)
					{
						Console.ForegroundColor = ConsoleColor.Magenta;
						break;
					}
					else if (tok.kind >= TokenFacts.firstPunc)
					{
						Console.ForegroundColor = ConsoleColor.Gray;
						break;
					}

					Console.ForegroundColor = ConsoleColor.Red;
					break;
				}

				Console.Write(tok.rawText);
				pos += tok.rawText.Length;
			}
			Console.WriteLine();
		}
#endif
	}
}
