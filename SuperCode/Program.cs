﻿//#define SYNTAX_ONLY

using System;
using System.Runtime.InteropServices;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class Program
	{
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate long Run();

		[StructLayout(LayoutKind.Explicit)]
		private unsafe struct FPIUnion
		{
			[FieldOffset(0)]
			public double fval;
			[FieldOffset(0)]
			public long ival;

			public override string ToString() =>
				$"Int: {ival}, Float: {fval}";
		}

		private static void Main(string[] _)
		{
			//Console.Write("Input file: ");
			//string file = Console.ReadLine();
			string file = "Code.sy";
			var parser = new Parser(file);
			// Console.ForegroundColor = ConsoleColor.DarkGreen;
			int pos = 0;
			foreach (var tok in parser.tokens)
			{
				while (pos < tok.pos)
					Console.Write(parser.lexer.src[pos++]);

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

			var safety = parser.Parse(out var tree);
			safety.Print(Console.Out);
			if (safety.MustSelfDestruct())
				goto Stop;

			tree.Print(Console.Out);
			Console.WriteLine();

#if !SYNTAX_ONLY
			LLVM.LinkInMCJIT();
			LLVM.InitializeNativeTarget();
			LLVM.InitializeNativeAsmParser();
			LLVM.InitializeNativeAsmPrinter();
			LLVM.InitializeNativeDisassembler();

			var noder = new Noder(tree);
			safety = noder.Nodify(out var node);
			safety.Print(Console.Out);
			if (safety.MustSelfDestruct())
				goto Stop;

			var cg = new CodeGen(node);
			var module = cg.Gen();
			// cg.Optimize();

			Console.ForegroundColor = ConsoleColor.DarkGreen;
			Console.WriteLine(module);

			Console.ForegroundColor = ConsoleColor.White;
			Compile(module);
			RunJIT(module);
#endif
		Stop:
			Console.ReadKey();
		}

#if !SYNTAX_ONLY
		private static unsafe void RunJIT(LLVMModuleRef module)
		{
			var options = new LLVMMCJITCompilerOptions { NoFramePointerElim = 1 };
			if (!module.TryCreateMCJITCompiler(out var engine, ref options, out string err))
			{
				Console.Error.WriteLine(err);
				return;
			}

			var exec = (Run) Marshal.GetDelegateForFunctionPointer(engine.GetPointerToGlobal(module.GetNamedFunction("Run")), typeof(Run));
			var union = new FPIUnion { ival = exec() };
			Console.WriteLine(union);
		}

		private static void Compile(LLVMModuleRef module)
		{
			if (!module.TryVerify(LLVMVerifierFailureAction.LLVMPrintMessageAction, out string err))
			{
				Console.Error.WriteLine(err);
				return;
			}

			var target = LLVMTargetRef.GetTargetFromTriple(LLVMTargetRef.DefaultTriple);
			var targetMachine = target.CreateTargetMachine(LLVMTargetRef.DefaultTriple, "generic", "",
				LLVMCodeGenOptLevel.LLVMCodeGenLevelAggressive, LLVMRelocMode.LLVMRelocDefault, LLVMCodeModel.LLVMCodeModelDefault);
			targetMachine.EmitToFile(module, "Code.o", LLVMCodeGenFileType.LLVMObjectFile);
		}
#endif
	}
}
