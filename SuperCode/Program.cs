//#define SYNTAX_ONLY

using System;
using System.Runtime.InteropServices;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class Program
	{
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate void Run();

		private static void Main(string[] _)
		{
			var parser = new Parser("Main.sy");
			// Console.ForegroundColor = ConsoleColor.DarkGreen;
			int pos = 0;
			foreach (var tok in parser.tokens)
			{
				while (pos < tok.pos)
					Console.Write(parser.lexer.src[pos++]);

				switch (tok.kind)
				{
				case TokenKind.Iden:
					Console.ForegroundColor = ConsoleColor.White;
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

				Console.Write(tok.text);
				pos += tok.text.Length;
			}
			Console.WriteLine('\n');

			var tree = parser.Parse();
			tree.Print(Console.Out);
			Console.WriteLine();

#if !SYNTAX_ONLY

			var noder = new Noder(tree);
			var node = noder.Nodify();

			var cg = new CodeGen(node);
			var module = cg.Gen();
			// cg.Optimize();

			Console.ForegroundColor = ConsoleColor.DarkGreen;
			Console.WriteLine(module);

			Console.ForegroundColor = ConsoleColor.White;
			RunJIT(module);
#endif
			Console.ReadKey();
		}

#if !SYNTAX_ONLY
		private static void RunJIT(LLVMModuleRef module)
		{
			LLVM.LinkInMCJIT();
			LLVM.InitializeNativeTarget();
			LLVM.InitializeNativeAsmParser();
			LLVM.InitializeNativeAsmPrinter();
			LLVM.InitializeNativeDisassembler();

			if (!module.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string err))
				Console.Error.WriteLine(err);

			var options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
			if (!module.TryCreateMCJITCompiler(out var engine, ref options, out err))
				Console.Error.WriteLine(err);

			var exec = (Run) Marshal.GetDelegateForFunctionPointer(engine.GetPointerToGlobal(module.GetNamedFunction("Run")), typeof(Run));
			exec();
		}
#endif
	}
}
