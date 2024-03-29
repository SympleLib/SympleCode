﻿using System.IO;
using System.Text;

namespace CommenSense;

partial class Parser
{
	readonly string folder;
	readonly string filename;
	readonly Token[] tokens;
	readonly List<Container> ctnrs = new List<Container>();
	int pos = 0;
	Token prev => Peek(-1);
	Token current => Peek();
	Token next => Peek(1);

	public Parser(string source, string file)
	{
		scope = new Scope(this);
		folder = Path.GetFileName(Path.GetDirectoryName(file)!);
		filename = Path.GetFileName(file);
		parsers.Add(folder + filename, this);

		Preprocessor preprocessor = new Preprocessor(source, folder, filename);
		tokens = preprocessor.PreProcess();

		foreach (Token token in tokens)
			if (token.kind is TokenKind.Unknown)
				BadCode.Report(new SyntaxError("unrecognized token", token));
			else if (token.kind is TokenKind.HashTag)
				throw new Exception("preprocessor directive after preprocessing!!");
	}

	public ModuleAst Parse()
	{
		MaybeEndLine();

		List<StmtAst> members = new List<StmtAst>();
		while (current.kind is not TokenKind.Eof)
			members.Add(Stmt());

		return new ModuleAst(filename, folder + "/" + filename, members.ToArray(), ctnrs.ToArray());
	}

	bool IsType(Token token)
	{
		if (token.kind is not TokenKind.Identifier)
			return false;

		return typeNames.ContainsKey(token.text);
	}

	ParamAst Param()
	{
		string[] metadata = MetaData();

		bool mutable;
		if (mutable = current.kind is TokenKind.MutableKeyword)
			Next();

		TypeAst type = Type();
		Token token;
		string name;
		if (current.kind is TokenKind.Identifier)
		{
			token = Next();
			name = token.text;
		}
		else
		{
			token = type.token;
			name = string.Empty;
		}

		ExprAst defaultExpr = new ExprAst(Token.devault);
		if (current.kind is TokenKind.Eql)
		{
			Next();
			defaultExpr = Expr();
		}

		return new ParamAst(metadata, mutable, type, token, name, defaultExpr);
	}

	EnumValueAst EnumValue(string parent = "")
	{
		string[] metadata = MetaData();
		Token name = Match(TokenKind.Identifier);
		string asmName = $".enum!{parent}:{name.text}";
		if (current.kind is not TokenKind.Eql)
			return new EnumValueAst(metadata, name, asmName, new ExprAst(Token.devault));

		Next();
		return new EnumValueAst(metadata, name, asmName, Expr());
	}

	FieldAst Field(List<FieldAst>? fields = null)
	{
		string[] metadata = MetaData();
		
		Visibility visibility;
		switch (current.kind)
		{
		case TokenKind.PublicKeyword:
			Next();
			visibility = LLVMDefaultVisibility;
			break;
		case TokenKind.PrivateKeyword:
			Next();
			visibility = LLVMHiddenVisibility;
			break;
		case TokenKind.ProtectedKeyword:
			Next();
			visibility = LLVMProtectedVisibility;
			break;
		default:
			visibility = LLVMDefaultVisibility;
			break;
		};

		bool mutable;
		if (mutable = current.kind is TokenKind.MutableKeyword)
			Next();
		
		TypeAst type;
		if (fields is not null && fields.Count > 0 && !IsType(current))
			type = fields[^1].type;
		else
			type = Type();
		Token name = Match(TokenKind.Identifier);
		ExprAst initializer = new ExprAst(Token.devault);
		if (current.kind is TokenKind.Eql)
		{
			Next();
			initializer = Expr();
		}

		return new FieldAst(metadata, visibility, mutable, type, name, initializer);
	}

	string[] MetaData()
	{
		List<string> metadata = new List<string>();
		while (current.kind is TokenKind.Annotation)
			metadata.Add(Next().text);
		return metadata.ToArray();
	}

	Token Peek(int offset = 0)
	{
		int i = pos + offset;
		if (i >= tokens.Length)
			return tokens.Last();
		return tokens[i];
	}

	Token Match(TokenKind kind)
	{
		if (current.kind != kind)
			BadCode.Report(new SyntaxError($"expected {kind}", current));
		return Next();
	}

	Token Next()
	{
		pos++;
		return prev;
	}

	void EndLine()
	{
		Match(TokenKind.Semicol);
		MaybeEndLine();
	}

	void MaybeEndLine()
	{
		while (current.kind is TokenKind.Semicol)
			Next();
	}

	string Name() =>
		Match(TokenKind.Identifier).text;
}