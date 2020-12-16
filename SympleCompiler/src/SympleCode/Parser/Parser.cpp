#include "Parser.h"

#include <xhash>
#include <iostream>

#include "SympleCode/Common/Node/Statement/BreakStatementNode.h"
#include "SympleCode/Common/Node/Statement/ExpressionStatementNode.h"

#include "SympleCode/Common/Node/Expression/BinaryExpressionNode.h"
#include "SympleCode/Common/Node/Expression/VariableExpressionNode.h"
#include "SympleCode/Common/Node/Expression/StringLiteralExpressionNode.h"
#include "SympleCode/Common/Node/Expression/NumberLiteralExpressionNode.h"
#include "SympleCode/Common/Node/Expression/BooleanLiteralExpressionNode.h"

#include "SympleCode/Common/Priority.h"

namespace Symple
{
	Parser::Parser(const char* source)
		: mLexer(source), mPosition(0), mTypes(Type::PrimitiveTypes), mDiagnostics(new Diagnostics)
	{
		const Token* current = Token::Default;
		while (!current->Is(Token::Kind::EndOfFile))
		{
			current = mLexer.Next();
			std::cout << Token::KindString(current->GetKind()) << " | " << current->GetLex() << '\n';
			if (current->Is(Token::Kind::Comment))
				Preprocess(current);
			else
				mTokens.push_back(current);
		}
	}

	void Parser::Preprocess(const Token* cmd)
	{
		std::string scmd(cmd->GetLex());
		Lexer prepoLexer = scmd.c_str();
		Token* rcmd = prepoLexer.Next();
		std::cout << rcmd->GetLex() << '\n';

		if (rcmd->GetLex() == "include")
		{
			Token* includeDir = prepoLexer.Next();

			FILE* file;
			errno_t err;
			if (!(err = fopen_s(&file, std::string(includeDir->GetLex()).c_str(), "rb")) && file)
			{
				fseek(file, 0L, SEEK_END);
				unsigned int size = std::min(ftell(file), 4096L);
				rewind(file);
				char* source = new char[size + 1];
				fread(source, 1, size, file);
				source[size] = 0;
				fclose(file);

				prepoLexer = source;
				const Token* iCurrent;
				while (!(iCurrent = prepoLexer.Next())->Is(Token::Kind::EndOfFile))
				{
					std::cout << Token::KindString(iCurrent->GetKind()) << " | " << iCurrent->GetLex() << '\n';
					if (iCurrent->Is(Token::Kind::Comment))
						Preprocess(iCurrent);
					else
						mTokens.push_back(iCurrent);
				}
			}
		}
	}

	CompilationUnitNode* Parser::ParseCompilationUnit()
	{
		auto members = ParseMembers();
		Match(Token::Kind::EndOfFile);
		return new CompilationUnitNode(members);
	}

	const Token* Parser::Peek(size_t offset)
	{
		size_t i = mPosition + offset;
		if (i >= mTokens.size())
			return mTokens.back();
		return mTokens[i];
	}

	const Token* Parser::Next()
	{
		const Token* current = Peek();
		mPosition++;
		return current;
	}

	const Token* Parser::Match(Token::Kind kind)
	{
		if (Peek()->Is(kind))
			return Next();
		mDiagnostics->ReportError(Peek(), "Unexpected Token '%s' of type <%s>, Expected <%s>", std::string(Peek()->GetLex()).c_str(), Token::KindString(Peek()->GetKind()), Token::KindString(kind));
		Next();
		return new Token(kind);
	}

	Diagnostics* Parser::GetDiagnostics() const
	{
		return mDiagnostics;
	}

	bool Parser::IsType(const Token* token)
	{
		for (const Type* type : mTypes)
			if (token->GetLex() == type->GetName())
				return true;
		return false;
	}

	const Type* Parser::GetType(const Token* token)
	{
		for (const Type* type : mTypes)
			if (token->GetLex() == type->GetName())
				return type;
		mDiagnostics->ReportError(token, "'%s' is not a Type", std::string(token->GetLex()).c_str());
		return mTypes.back();
	}

	const std::vector<const MemberNode*> Parser::ParseMembers()
	{
		std::vector<const MemberNode*> members;
		while (!Peek()->Is(Token::Kind::EndOfFile))
		{
			const Token* start = Peek();
			members.push_back(ParseMember());

			if (start == Peek())
				Next();
		}

		return members;
	}

	MemberNode* Parser::ParseMember()
	{
		if (IsType(Peek()) && Peek(2)->Is(Token::Kind::OpenParenthesis))
			return ParseFunctionDeclaration();
		if (Peek()->Is(Token::Kind::Hint))
			return ParseFunctionHint();
		if (Peek()->Is(Token::Kind::Extern))
			return ParseExternFunction();

		return ParseGlobalStatement();
	}

	FunctionDeclarationNode* Parser::ParseFunctionDeclaration()
	{
		const Type* type = GetType(Next());
		const Token* name = Next();
		FunctionArgumentsNode* arguments = ParseFunctionArguments();
		BlockStatementNode* body = ParseBlockStatement();
		Match(Token::Kind::Semicolon);

		FunctionDeclarationNode* declaration = new FunctionDeclarationNode(type, name, arguments, body);
		mDiagnostics->FunctionDeclaration(declaration);
		return declaration;
	}

	FunctionArgumentsNode* Parser::ParseFunctionArguments()
	{
		const Token* open = Match(Token::Kind::OpenParenthesis);

		std::vector<const FunctionArgumentNode*> arguments;
		while (!Peek()->Is(Token::Kind::CloseParenthesis))
		{
			if (Peek()->Is(Token::Kind::EndOfFile))
			{
				mDiagnostics->ReportError(Next(), "Unexpected End Of File");
				break;
			}

			arguments.push_back(ParseFunctionArgument());

			if (Peek()->Is(Token::Kind::Comma))
				Next();
			else
			{
				if (!Peek()->Is(Token::Kind::CloseParenthesis))
					mDiagnostics->ReportError(Peek(), "Expected Comma");
				break;
			}
		}

		const Token* close = Match(Token::Kind::CloseParenthesis);

		return new FunctionArgumentsNode(open, arguments, close);
	}

	FunctionArgumentNode* Parser::ParseFunctionArgument()
	{
		const Type* type = GetType(Next());
		const Token* name = Next();

		return new FunctionArgumentNode(type, name);
	}

	FunctionHintNode* Parser::ParseFunctionHint()
	{
		Match(Token::Kind::Hint);
		const Type* type = GetType(Next());
		const Token* name = Next();
		FunctionArgumentsNode* arguments = ParseFunctionArguments();
		Match(Token::Kind::Semicolon);

		FunctionHintNode* hint = new FunctionHintNode(type, name, arguments);
		mDiagnostics->FunctionDeclaration(hint);
		return hint;
	}

	ExternFunctionNode* Parser::ParseExternFunction()
	{
		Match(Token::Kind::Extern);
		const Type* type = GetType(Next());
		const Token* name = Next();
		FunctionArgumentsNode* arguments = ParseFunctionArguments();
		Match(Token::Kind::Semicolon);

		ExternFunctionNode* hint = new ExternFunctionNode(type, name, arguments);
		mDiagnostics->FunctionDeclaration(hint);
		return hint;
	}

	StatementNode* Parser::ParseStatement()
	{
		if (Peek()->Is(Token::Kind::Semicolon))
			return new StatementNode; // Empty Statement;
		if (Peek()->Is(Token::Kind::Return))
			return ParseReturnStatement();
		if (Peek()->Is(Token::Kind::While))
			return ParseWhileStatement();
		if (Peek()->Is(Token::Kind::If))
			return ParseIfStatement();
		if (Peek()->Is(Token::Kind::Break))
			return new BreakStatementNode(Next());
		if (IsType(Peek()))
			return ParseVariableDeclaration();
		if (Peek()->Is(Token::Kind::OpenBracket))
		{
			StatementNode* statement = ParseBlockStatement();
			Match(Token::Kind::Semicolon);
			return statement;
		}
		ExpressionNode* expression = ParseExpression();
		Match(Token::Kind::Semicolon);
		return new ExpressionStatementNode(expression);
	}

	IfStatementNode* Parser::ParseIfStatement()
	{
		const Token* open = Match(Token::Kind::If);
		ExpressionNode* condition = ParseExpression();
		BlockStatementNode* then = ParseBlockStatement();
		BlockStatementNode* elze = nullptr;
		if (Peek()->Is(Token::Kind::Else))
		{
			Next();
			elze = ParseBlockStatement();
		}
		Match(Token::Kind::Semicolon);

		return new IfStatementNode(open, condition, then, elze);
	}

	WhileStatementNode* Parser::ParseWhileStatement()
	{
		const Token* open = Match(Token::Kind::While);
		ExpressionNode* condition = ParseExpression();
		BlockStatementNode* body = ParseBlockStatement();
		Match(Token::Kind::Semicolon);

		return new WhileStatementNode(open, condition, body);
	}

	BlockStatementNode* Parser::ParseBlockStatement()
	{
		const Token* open = Match(Token::Kind::OpenBracket);
		std::vector<const StatementNode*> statements;
		while (!Peek()->Is(Token::Kind::CloseBracket))
		{
			if (Peek()->Is(Token::Kind::EndOfFile))
			{
				mDiagnostics->ReportError(Next(), "Unexpected End Of File");
				break;
			}

			const Token* start = Peek();

			statements.push_back(ParseStatement());

			if (start == Peek())
				Next();
		}
		const Token* close = Match(Token::Kind::CloseBracket);

		return new BlockStatementNode(open, statements, close);
	}

	ReturnStatementNode* Parser::ParseReturnStatement()
	{
		Match(Token::Kind::Return);
		ExpressionNode* expression = ParseExpression();
		Match(Token::Kind::Semicolon);
		return new ReturnStatementNode(expression);
	}

	GlobalStatementNode* Parser::ParseGlobalStatement()
	{
		return new GlobalStatementNode(ParseStatement());
	}

	VariableDeclarationNode* Parser::ParseVariableDeclaration()
	{
		const Type* type = GetType(Next());
		const Token* name = Next();
		if (Peek()->Is(Token::Kind::Equal))
		{
			Next();
			ExpressionNode* expression = ParseExpression();
			Match(Token::Kind::Semicolon);

			return new VariableDeclarationNode(name, type, expression);
		}

		Match(Token::Kind::Semicolon);

		return new VariableDeclarationNode(name, type, new ExpressionNode);
	}

	ExpressionNode* Parser::ParseExpression()
	{
		return ParseBinaryExpression();
	}

	ExpressionNode* Parser::ParseBinaryExpression(int parentPriority)
	{
		ExpressionNode* left = ParsePrimaryExpression();

		while (!Peek()->Is(Token::Kind::EndOfFile))
		{
			int priority = Priority::BinaryOperatorPriority(Peek());
			if (priority < 0 || priority <= parentPriority)
				break;
			const Token* oqerator = Next();
			ExpressionNode* right = ParseBinaryExpression(priority);
			left = new BinaryExpressionNode(oqerator, left, right);
		}

		return left;
	}

	ExpressionNode* Parser::ParsePrimaryExpression()
	{
		switch (Peek()->GetKind())
		{
		case Token::Kind::OpenParenthesis:
			return ParseParenthesizedExpression();
		case Token::Kind::True:
		case Token::Kind::False:
			return new BooleanLiteralExpressionNode(Next());
		case Token::Kind::Number:
			return new NumberLiteralExpressionNode(Next());
		case Token::Kind::String:
			return new StringLiteralExpressionNode(Next());
		}

		return ParseNameOrCallExpression();
	}

	ExpressionNode* Parser::ParseNameOrCallExpression()
	{
		if (Peek(1)->Is(Token::Kind::OpenParenthesis))
			return ParseFunctionCallExpression();

		return new VariableExpressionNode(Next());
	}

	FunctionCallExpressionNode* Parser::ParseFunctionCallExpression()
	{
		const Token* name = Match(Token::Kind::Identifier);
		FunctionCallArgumentsNode* arguments = ParseFunctionCallArguments();

		return new FunctionCallExpressionNode(name, arguments);
	}

	FunctionCallArgumentsNode* Parser::ParseFunctionCallArguments()
	{
		const Token* open = Match(Token::Kind::OpenParenthesis);

		std::vector<const ExpressionNode*> arguments;
		while (!Peek()->Is(Token::Kind::CloseParenthesis))
		{
			if (Peek()->Is(Token::Kind::EndOfFile))
			{
				mDiagnostics->ReportError(Next(), "Unexpected End Of File");
				break;
			}

			arguments.push_back(ParseExpression());

			if (Peek()->Is(Token::Kind::Comma))
				Next();
			else
				break;
		}

		const Token* close = Match(Token::Kind::CloseParenthesis);

		return new FunctionCallArgumentsNode(open, arguments, close);
	}

	ParenthesizedExpressionNode* Parser::ParseParenthesizedExpression()
	{
		const Token* open = Match(Token::Kind::OpenParenthesis);
		ExpressionNode* expression = ParseExpression();
		const Token* close = Match(Token::Kind::CloseParenthesis);

		return new ParenthesizedExpressionNode(open, expression, close);
	}
}