using System;
using System.IO;
using System.Collections.Generic;

namespace SuperCode
{
	public abstract class Ast
	{
		public abstract AstKind kind { get; }

		private void PrintChildren(TextWriter writer, string indent)
		{
			var fields = GetType().GetFields();
			bool toConsole = writer == Console.Out;

			for (int i = 0; i < fields.Length; i++)
			{
				bool fieldLast = i == fields.Length - 1;
				var field = fields[i];

				if (field.FieldType.IsAssignableTo(typeof(Ast)))
				{
					var child = (Ast)field.GetValue(this);
					if (child is not null)
						child.Print(writer, indent, $"{field.Name}: ", fieldLast);
				}
				else if (field.FieldType.IsArray)
				{
					if (field.FieldType.IsAssignableTo(typeof(Ast[])))
					{
						var arr = (Ast[]) field.GetValue(this);
						foreach (var obj in arr)
						{
							var child = obj;
							if (child is not null)
								child.Print(writer, indent, $"[{field.Name[..^1]}] ", fieldLast && obj == arr[^1]);
						}
					}
					else if (field.FieldType.IsAssignableTo(typeof(Token[])))
					{
						var arr = (Token[]) field.GetValue(this);
						if (arr.Length == 0)
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
							writer.WriteLine("Empty");
						}

						foreach (var token in arr)
						{
							if (token.kind == TokenKind.Unknown)
								continue;

							bool last = fieldLast && (token == arr[^1]);
							string prefix = indent + (last ? "└──" : "├──");

							if (toConsole)
								Console.ForegroundColor = ConsoleColor.Cyan;
							writer.Write(prefix);
							if (toConsole)
								Console.ForegroundColor = ConsoleColor.DarkGray;
							writer.Write($"[{field.Name[..^1]}] ");
							if (toConsole)
								Console.ForegroundColor = ConsoleColor.DarkYellow;
							writer.WriteLine(token);
						}
					}
				}
				else if (field.FieldType.IsAssignableTo(typeof(Token)))
				{
					var token = (Token) field.GetValue(this);
					if (token.kind == TokenKind.Unknown)
						continue;

					string prefix = indent + (fieldLast ? "└──" : "├──");

					if (toConsole)
						Console.ForegroundColor = ConsoleColor.Cyan;
					writer.Write(prefix);
					if (toConsole)
						Console.ForegroundColor = ConsoleColor.DarkGray;
					writer.Write($"{field.Name}: ");
					if (toConsole)
						Console.ForegroundColor = ConsoleColor.DarkYellow;
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
			writer.WriteLine(kind);

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
