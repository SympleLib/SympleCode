#include "Symple/Code/Emit/Emitter.h"

namespace Symple::Code
{
	Emitter::Emitter(const GlobalRef<const CompilationUnitAst> &unit)
		: m_Unit(unit), m_File("sy/Out.S", FilePermissions::Write) {}

	void Emitter::Emit()
	{
		for (auto member : m_Unit->Members)
			switch (member->Kind)
			{
			case AstKind::Function:
				Emit(Cast<const FunctionAst>(member));
				break;
			}

		m_File.Close();
	}


	void Emitter::Emit(const GlobalRef<const FunctionAst> &fn)
	{
		std::string name(fn->Name->Text);

		Emit(".global %s", name.c_str());
		Emit("%s:", name.c_str());
		Emit("\tpush %s", Reg(RegKind::Bp));
		Emit("\tmov %s, %s", Reg(RegKind::Sp), Reg(RegKind::Bp));
		Emit("# Function body...");
		Emit("\tmov %s, %s", Reg(RegKind::Bp), Reg(RegKind::Sp));
		Emit("\tpop %s", Reg(RegKind::Bp));
		Emit("\tret");
	}


	template<typename... Args>
	void Emitter::Emit(const char *fmt, Args&&... args)
	{
		std::fprintf(m_File.Stream, fmt, args...);
		std::fputc('\n', m_File.Stream);
	}

	const char *Emitter::Reg(RegKind kind, uint32 sz)
	{
		switch (kind)
		{
		case RegKind::Ax:
			if (sz <= 1)
				return "%al";
			if (sz <= 2)
				return "%ax";
			if (sz <= 4)
				return "%eax";
			break;
		case RegKind::Bx:
			if (sz <= 1)
				return "%bl";
			if (sz <= 2)
				return "%bx";
			if (sz <= 4)
				return "%ebx";
			break;
		case RegKind::Cx:
			if (sz <= 1)
				return "%cl";
			if (sz <= 2)
				return "%cx";
			if (sz <= 4)
				return "%ecx";
			break;
		case RegKind::Dx:
			if (sz <= 1)
				return "%dl";
			if (sz <= 2)
				return "%dx";
			if (sz <= 4)
				return "%edx";
			break;

		case RegKind::Sp:
			return "%esp";
		case RegKind::Bp:
			return "%ebp";
		}

		return nullptr;
	}
}