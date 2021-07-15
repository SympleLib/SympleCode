using System;
using System.IO;
using System.Collections.Generic;
using LLVMSharp.Interop;

namespace SuperCode
{
	public abstract class Node
	{
		public virtual NodeKind kind => NodeKind.Unknown;

		public abstract LLVMValueRef Build(LLVMModuleRef module, LLVMBuilderRef builder);

		public Node[] GetChildren()
		{
			var fields = GetType().GetFields();
			var children = new List<Node>();

			foreach (var field in fields)
				if (field.FieldType.IsSubclassOf(typeof(Node)))
				{
					var child = (Node) field.GetValue(this);
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

				if (field.FieldType.IsSubclassOf(typeof(Node)))
				{
					var child = (Node) field.GetValue(this);
					if (child is not null)
						child.Print(writer, indent, fieldLast);
				}
				else
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
					writer.WriteLine(field.GetValue(this));
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
