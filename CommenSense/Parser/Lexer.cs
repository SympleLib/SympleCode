using System.Text;

using static System.Net.Mime.MediaTypeNames;

namespace CommenSense;

public enum TokenKind
{
	Unknown = -1,
	HashTag = -2,
	Eof = 0,

	Str,
	Int,
	Char,
	Float,
	Identifier,
	Annotation,

	Plus,
	Minus,
	Star,
	Slash,
	Percent,
	SlashDash,
	PercentDash,

	Bang,
	And,
	Pipe,
	Carot,
	Tilde,

	Eql,
	Dot,
	Arrow,
	DotDotDot,

	LeftBrace,
	RightBrace,
	LeftBracket,
	RightBracket,
	LeftParen,
	RightParen,
	LeftChevron,
	RightChevron,

	PlusEql,
	MinusEql,
	StarEql,
	SlashEql,
	PercentEql,
	AndEql,
	PipeEql,
	CarotEql,
	LEql,
	REql,
	EqlEql,
	BangEql,
	TildeEql,

	Plus2,
	Minus2,
	Star2,
	Bang2,
	Tilde2,
	And2,
	Pipe2,
	Left2,
	Right2,

	Star2Eql,
	Left2Eql,
	Right2Eql,

	Comma,
	Colon,
	Semicol,


	TrueKeyword,
	FalseKeyword,
	NullKeyword,

	DeclKeyword,
	StructKeyword,
	ClassKeyword,

	IfKeyword,
	ElseKeyword,
	WhileKeyword,
	ForKeyword,
	BreakKeyword,
	BreakAllKeyword,
	RetKeyword,

	AsKeyword,
	ToKeyword,

	PublicKeyword,
	PrivateKeyword,
	ProtectedKeyword,

	UsingKeyword,
	LinkKeyword,
}

public record Token(TokenKind kind, string text, int line, int col)
{ public static readonly Token devault = new Token(TokenKind.Unknown, string.Empty, 0, 0); }

class Lexer
{
	public static void PrintMatching()
	{
		var vals = Enum.GetValues<TokenKind>();
		for (int i = (int)punctuatorStart; i < (int) keywordStart; i++)
			Console.WriteLine($"{vals[i]}: '{punctuators[i - (int) punctuatorStart]}'");
		for (int i = (int) keywordStart; i < vals.Length - 2; i++)
			Console.WriteLine($"{vals[i]}: '{keywords[i - (int) keywordStart]}'");
	}

	const TokenKind punctuatorStart = TokenKind.Plus;
	static readonly string[] punctuators = {
			"+",
			"-",
			"*",
			"/",
			"%",
			"/-",
			"%-",

			"!",
			"&",
			"|",
			"^",
			"~",

			"=",
			".",
			"->",
			"...",

			"{",
			"}",
			"[",
			"]",
			"(",
			")",
			"<",
			">",

			"+=",
			"-=",
			"*=",
			"/=",
			"%=",
			"&=",
			"|=",
			"^=",
			"<=",
			">=",
			"==",
			"!=",
			"~=",

			"++",
			"--",
			"**",
			"!!",
			"~~",
			"&&",
			"||",
			"<<",
			">>",

			"**=",
			"<<=",
			">>=",

			",",
			":",
			";",
		};

	const TokenKind keywordStart = TokenKind.TrueKeyword;
	static readonly string[] keywords = {
			"true",
			"false",
			"null",

			"decl",
			"struct",
			"class",

			"if",
			"else",
			"while",
			"for",
			"break",
			"breakall",
			"ret",

			"as",
			"to",

			"public",
			"private",
			"protected",

			"using",
			"link",
		};

	readonly string src;
	int pos;
	int col;
	int colStart;
	int currentCol => pos - colStart;
	bool tokenThisLine = false;
	int line = 1;
	char current => Peek();
	char next => Peek(1);

	public Lexer(string source) =>
		src = source;

	public Token LexNext()
	{
		while (true)
		{
			if (current is '\n')
			{
				pos++;
				colStart = pos;
				line++;
				tokenThisLine = false;
			}
			else if (char.IsWhiteSpace(current))
				pos++;
			else if (current is '/' && next is '/')
				SingleComment();
			else if (current is '/' && next is '*')
				MultiComment();
			else
				break;
		}


		col = currentCol;
		if (current is '#')
			return HashTag();

		tokenThisLine = true;
		if (current is '\0')
			return new Token(TokenKind.Eof, string.Empty, line, col);

		if (char.IsDigit(current) || (current is '.' && char.IsDigit(next)))
			return Num();
		if (char.IsLetter(current) || current is '$')
			return Identifier();
		if (current is '@')
			return Annotation();
		if (current is '\'')
			return Str();
		if (current is '`')
			return Char();
		return Punctuator();
	}

	Token HashTag()
	{
		if (tokenThisLine)
			throw new Exception("hashtag not at begging of line");

		pos++;
		int start = pos;
		while (current is not '\n')
			pos++;

		return new Token(TokenKind.HashTag, src[start..pos], line, col);
	}

	Token Num()
	{
		int start = pos;
		TokenKind kind = TokenKind.Int;
		while (char.IsDigit(current) || current is '.')
		{
			if (current is '.')
			{
				if (kind == TokenKind.Int)
					kind = TokenKind.Float;
				else
					throw new Exception("too many dots");
			}

			pos++;
		}

		return new Token(kind, src[start..pos], line, col);
	}

	Token Str()
	{
		Next();
		StringBuilder sb = new StringBuilder();
		while (current is not '\'' and not '\0')
		{
			if (current is '\\')
			{
				Next();
				switch (Next())
				{
				case '0':
					sb.Append('\0');
					break;
				case '\a':
					sb.Append('\a');
					break;
				case 'b':
					sb.Append('\b');
					break;
				case 'f':
					sb.Append('\f');
					break;
				case 'n':
					sb.Append('\n');
					break;
				case 'r':
					sb.Append('\r');
					break;
				case 't':
					sb.Append('\t');
					break;
				case 'v':
					sb.Append('\v');
					break;
				case '\\':
					sb.Append('\\');
					break;
				case '\'':
					sb.Append('\'');
					break;

				default:
					throw new Exception("Unrecognized escape sequence");
				}
			}
			else
				sb.Append(Next());
		}

		Next();
		return new Token(TokenKind.Str, sb.ToString(), line, col);
	}

	Token Char()
	{
		Next();
		string text;

		if (current is '\\')
		{
			Next();
			switch (Next())
			{
			case '0':
				text = "\0";
				break;
			case '\a':
				text = "\a";
				break;
			case 'b':
				text = "\b";
				break;
			case 'f':
				text = "\f";
				break;
			case 'n':
				text = "\n";
				break;
			case 'r':
				text = "\r";
				break;
			case 't':
				text = "\t";
				break;
			case 'v':
				text = "\v";
				break;
			case '\\':
				text = "\\";
				break;
			case '\'':
				text = "\'";
				break;

			default:
				throw new Exception("Unrecognized escape sequence");
			}
		}
		else
			text = Next().ToString();

		if (Next() is not '`')
			throw new Exception("literal can only have one char");
		return new Token(TokenKind.Char, text, line, col);
	}

	Token Identifier()
	{
		int start = pos;
		while (char.IsLetterOrDigit(current) || current is '_' or '$')
			pos++;

		string text = src[start..pos];
		for (int i = 0; i < keywords.Length; i++)
			if (keywords[i] == text)
				return new Token(keywordStart + i, text, line, col);

		return new Token(TokenKind.Identifier, text, line, col);
	}

	Token Annotation()
	{
		Next();
		int start = pos;
		while (char.IsLetterOrDigit(current))
			pos++;

		return new Token(TokenKind.Annotation, src[start..pos], line, col);
	}

	Token Punctuator()
	{
		for (int i = punctuators.Length - 1; i >= 0; i--)
			if (src[pos..].StartsWith(punctuators[i]))
				return new Token(punctuatorStart + i, src[pos..(pos += punctuators[i].Length)], line, col);

		return new Token(TokenKind.Unknown, src[pos..++pos], line, col);
	}

	void SingleComment()
	{
		Next();
		Next();
		while (current is not '\n')
			Next();
	}

	void MultiComment()
	{
		Next();
		Next();
		while (current is not '*' && next is not '/')
			Next();
		Next();
		Next();
	}

	char Next()
	{
		char c = current;
		if (pos < src.Length)
			pos++;
		return c;
	}

	char Peek(int offset = 0)
	{
		int i = pos + offset;
		if (i < src.Length)
			return src[i];
		return '\0';
	}
}
