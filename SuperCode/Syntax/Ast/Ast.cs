using System;
using System.IO;
using System.Collections.Generic;

namespace SuperCode
{
	public class Ast
	{
		public virtual AstKind kind => AstKind.Unknown;

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

		public Ast[] GetChildren()
		{
			var fields = GetType().GetFields();
			var children = new List<Ast>();

			foreach (var field in fields)
				if (field.FieldType.IsSubclassOf(typeof(Ast)))
				{
					var child = (Ast)field.GetValue(this);
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

				if (field.FieldType.IsSubclassOf(typeof(Ast)))
				{
					var child = (Ast)field.GetValue(this);
					if (child is not null)
						child.Print(writer, indent, $"{field.Name}: ", fieldLast);
				}
				else if (field.FieldType == typeof(Token))
				{
					string prefix = indent + (fieldLast ? "└──" : "├──");

					if (toConsole)
						Console.ForegroundColor = ConsoleColor.Cyan;
					writer.Write(prefix);
					if (toConsole)
						Console.ForegroundColor = ConsoleColor.DarkGray;
					writer.Write($"{field.Name}: ");
					if (toConsole)
						Console.ForegroundColor = ConsoleColor.DarkYellow;
					var token = (Token)field.GetValue(this);
					writer.WriteLine(token);
				}
			}
		}

		public void Print(TextWriter writer, string indent = "", string prefix = "", bool last = true)
		{
			bool toConsole = writer == Console.Out;
			string before = indent + (last ? "└──" : "├──");

			if (toConsole)
				Console.ForegroundColor = ConsoleColor.Cyan;
			writer.Write(before);
			if (toConsole)
				Console.ForegroundColor = ConsoleColor.DarkGray;
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
