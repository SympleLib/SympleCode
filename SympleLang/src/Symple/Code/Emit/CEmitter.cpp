#include "Symple/Code/Emit/CEmitter.h"

namespace Symple::Code
{
	CEmitter::CEmitter(const GlobalRef<const CompilationUnitAst> &unit)
		: m_Unit(unit), m_File("bin/Out.c", FilePermissions::Write) {}

	void CEmitter::Emit()
	{
		Emit("typedef void Void;");
		Emit("typedef unsigned char Byte;");
		Emit("typedef short Short;");
		Emit("typedef int Int;");

		Emit("typedef float Float;");
	
		Emit("typedef char Char;");
		Emit("typedef unsigned int WChar;");
		Emit("typedef _Bool Bool;");

		Emit("#define false (Bool)(0)");
		Emit("#define true (Bool)(1)");

		Emit(R"(
int main(int nArgs, char **args)
{
	extern int Syc$Main$Int$2Char(int nArgs, char **args);
	return Syc$Main$Int$2Char(nArgs, args);
}
		)");

		for (auto member : m_Unit->Members)
			switch (member->Kind)
			{
			case AstKind::Function:
				Emit(Cast<const FunctionAst>(member));
				break;
			}

		m_File.Close();
	}


	void CEmitter::Emit(const GlobalRef<const FunctionAst> &fn)
	{
		const std::string &name = fn->MangledName;
		
		Emit("%s %s(", fn->Type->Type->Code.c_str(), name.c_str());
		bool first = true;
		for (auto param : fn->Parameters)
		{
			Emit("%c %s %s", first ? ' ' : ',', param->Type->Type->Code.c_str(), param->MangledName.c_str());
			if (first)
				first = false;
		}
		Emit(")");
		Emit("{");

		Emit(fn->Body);

		// If is main function
		if (fn->MangledName == "Syc$Main$Int$2Char")
			Emit("return 0;");

		Emit("}");
	}


	void CEmitter::Emit(const GlobalRef<const StatementAst> &stmt)
	{
		switch (stmt->Kind)
		{
		case AstKind::ExpressionStatement:
			Emit(Cast<const ExpressionStatementAst>(stmt)->Expression);
			break;
		}
	}


	void CEmitter::Emit(const GlobalRef<const ExpressionAst> &expr)
	{
		switch (expr->Kind)
		{
		case AstKind::CallExpression:
			Emit(Cast<const CallExpressionAst>(expr));
			break;
		}
	}

	void CEmitter::Emit(const GlobalRef<const CallExpressionAst> &call)
	{
		
	}
	

	template<typename... Args>
	void CEmitter::Emit(_Printf_format_string_ const char *fmt, Args&&... args)
	{
		std::fprintf(m_File.Stream, fmt, args...);
		std::fputc('\n', m_File.Stream);
	}
}