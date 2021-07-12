using LLVMSharp.Interop;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace SuperCode
{
	public abstract class Node
	{
		public abstract Token token { get; }
		public abstract LLVMValueRef CodeGen(LLVMBuilderRef builder);

		public Token[] GetTokens()
		{
			var fields = GetType().GetFields();
			var tokens = new List<Token>();

			foreach (var field in fields)
				if (field.FieldType == typeof(Token))
				{
					var token = (Token)field.GetValue(this);
					tokens.Add(token);
				}

			return tokens.ToArray();
		}

		public ExprNode[] GetChildren()
		{
			var fields = GetType().GetFields();
			var children = new List<ExprNode>();

			foreach (var field in fields)
				if (typeof(ExprNode).IsAssignableFrom(field.FieldType))
				{
					var child = (ExprNode)field.GetValue(this);
					if (child is not null)
						children.Add(child);
				}

			return children.ToArray();
		}

		private void PrintChildren(TextWriter writer, string indent)
		{
			var fields = GetType().GetFields();
			bool toConsole = writer == Console.Out;

			for (int i = 0; i < fields.Length; i++)
			{
				bool fieldLast = i == fields.Length - 1;
				var field = fields[i];

				if (typeof(ExprNode).IsAssignableFrom(field.FieldType))
				{
					var child = (ExprNode)field.GetValue(this);
					if (child is not null)
						child.Print(writer, indent, fieldLast);
				}
				else if (field.FieldType == typeof(Token))
				{
					string prefix = indent + (fieldLast ? "└──" : "├──");

					if (toConsole)
						Console.ForegroundColor = ConsoleColor.Cyan;
					writer.Write(prefix);
					if (toConsole)
						Console.ForegroundColor = ConsoleColor.Green;
					writer.Write($"{field.Name}: ");
					if (toConsole)
						Console.ForegroundColor = ConsoleColor.Yellow;
					var token = (Token)field.GetValue(this);
					writer.WriteLine(token);
				}
			}
		}

		public void Print(TextWriter writer, string indent = "", bool last = true)
		{
			bool toConsole = writer == Console.Out;
			string prefix = indent + (last ? "└──" : "├──");

			if (toConsole)
				Console.ForegroundColor = ConsoleColor.Cyan;
			writer.Write(prefix);
			if (toConsole)
				Console.ForegroundColor = ConsoleColor.Green;
			writer.WriteLine(GetType().Name);

			indent += last ? "   " : "│  ";
			PrintChildren(writer, indent);
		}

		public override string ToString()
		{
			using var writer = new StringWriter();
			Print(writer);
			return writer.ToString();
		}
	}
}
