using System;
using System.Collections.Generic;
using System.IO;

namespace SuperCode
{
	public struct Threat
	{
		public enum Level
		{
			Think,
			Avengers,
		}

		public readonly Level lvl;
		public readonly string msg;
		public readonly Token where;

		public Threat(Level lvl, string msg, Token where)
		{
			this.lvl = lvl;
			this.msg = msg;
			this.where = where;
		}

		public void Print(TextWriter writer)
		{
			bool toConsole = writer == Console.Out;

			if (toConsole)
				Console.ForegroundColor = ConsoleColor.DarkGray;
			writer.Write($"'{where.file}'<{where.line}:{where.col}> ");

			switch (lvl)
			{
			case Level.Think:
				if (toConsole)
					Console.ForegroundColor = ConsoleColor.Green;
				writer.Write("Think");
				break;
			case Level.Avengers:
				if (toConsole)
					Console.ForegroundColor = ConsoleColor.Red;
				writer.Write("Avengers level threat");
				break;
			}

			if (toConsole)
				Console.ForegroundColor = ConsoleColor.Yellow;
			writer.WriteLine($": {msg}");
		}

		public override string ToString()
		{
			using var writer = new StringWriter();
			Print(writer);
			return writer.ToString();
		}
	}

	public class PermaSafe
	{
		public readonly List<Threat> threats = new List<Threat>();

		public bool MustSelfDestruct()
		{
			foreach (var threat in threats)
				if (threat.lvl == Threat.Level.Avengers)
					return true;
			return false;
		}

		public void Report(Threat.Level lvl, string msg, Token tok) =>
			threats.Add(new Threat(lvl, msg, tok));

		public void ReportUnexpectedEof(Token where) =>
			Report(Threat.Level.Avengers, "Unexpected end of file", where);

		public void ReportExpectedToken(TokenKind expected, Token got) =>
			Report(Threat.Level.Avengers, $"Expected {expected}, got {got.kind}", got);

		public void ReportExpectedExpr(Token where) =>
			Report(Threat.Level.Avengers, "Expected expression", where);

		public void ReportNSField(Token where) =>
			Report(Threat.Level.Avengers, "No such field found", where);

		public void ReportUS(Token where) =>
			Report(Threat.Level.Avengers, $"Undefined symbol `{where.text}`", where);

		public void ReportNMod(Token where) =>
			Report(Threat.Level.Avengers, "No a modifiable value", where);

		public void ReportPossibleLossOfData(Token where) =>
			Report(Threat.Level.Think, "Possible loss of data", where);


		public void Print(TextWriter writer)
		{
			foreach (var threat in threats)
				threat.Print(writer);
		}

		public override string ToString()
		{
			using var writer = new StringWriter();
			Print(writer);
			return writer.ToString();
		}
	}
}
