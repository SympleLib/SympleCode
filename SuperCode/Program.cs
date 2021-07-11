using System;
using LLVMSharp.Interop;

namespace SuperCode
{
	public class Program
	{
		private static void Main(string[] args)
		{
			var lexer = new Lexer(@"6969 four20 L33T");
			var tokens = lexer.Lex();

			foreach (var token in tokens)
				Console.WriteLine(token);
			Console.ReadKey();
		}
	}
}
