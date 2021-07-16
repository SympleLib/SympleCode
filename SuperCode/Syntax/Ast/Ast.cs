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

				if (field.FieldType.IsSubclassOf(typeof(Ast)))
				{
					var child = (Ast)field.GetValue(this);
					if (child is not null)
						child.Print(writer, indent, $"{field.Name}: ", fieldLast);
				}
				else if (field.FieldType.IsArray)
				{
					foreach (var obj in (Ast[]) field.GetValue(this))
					{
						var child = obj;
						if (child is not null)
							child.Print(writer, indent, $"[{field.Name[..^1]}] ", fieldLast);
					}
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
