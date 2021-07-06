#include "Symple/Code/Emit/Emitter.h"

#define DEBUG_SYMBOLS false

#if DEBUG_SYMBOLS
#define NOMINMAX
#include <Windows.h>
#endif

namespace Symple::Code
{
#define Breakpoint Emit("\tint $3")

#define SYMBOL "\"%s\""
#define VAR SYMBOL

#define FUNCTION SYMBOL
#define FUNCTION_RETURN "\"%s.Return\""
#define FUNCTION_STACKSIZE "\"%s.StackSize\""

	Emitter::Emitter(const GlobalRef<const CompilationUnitAst> &unit)
		: m_Unit(unit), m_TextFile("bin/Out.S", FilePermissions::Write), m_DataFile("bin/__OutData.S", FilePermissions::ReadWrite) {}

	void Emitter::Emit()
	{
	#if DEBUG_SYMBOLS
		auto file = m_Unit->EndOfFile->File;
		m_FNum = file->Number;

		std::string fileName = file->Name.substr(file->Name.find_last_of('/') + 1);

		std::string path;
		path.resize(_MAX_PATH);
		GetModuleFileNameA(NULL, path.data(), path.size());
		path.resize(path.find_last_of('\\'));
		for (char &c : path)
			if (c == '\\')
				c = '/';

		EmitDbg(".file \"%s\"", fileName.c_str());
		EmitDbg(".cv_file %u \"%s\"", m_FNum, (path + fileName).c_str());
	#endif

		Emit(".text");
		Emit(".global main");
		Emit("main:");
		Emit("\tjmp Syc$Main$2char$int");


		Emit(EmitKind::Data, ".data");


		for (auto member : m_Unit->Members)
			switch (member->Kind)
			{
			case AstKind::Function:
				Emit(Cast<const FunctionAst>(member));
				m_FId++;
				break;
			}

		char c;
		m_DataFile.Seek(0, SEEK_SET);
		while ((c = fgetc(m_DataFile.Stream)) != EOF)
			fputc(c, m_TextFile.Stream);

		m_DataFile.Close();
		m_TextFile.Close();

		// Temp file...
		std::remove(m_DataFile.Name.c_str());
	}


	void Emitter::Emit(const GlobalRef<const FunctionAst> &fn)
	{
		m_Func = fn;
		m_Stack = m_StackSize = 0;
		decltype(auto) name = fn->MangledName;

		Emit(".global " FUNCTION, name.c_str());
		Emit(FUNCTION ":", name.c_str());

		// Debug Symbols (I guess...)
		EmitDbg(".Lfunc_begin%u:", m_FId);
		EmitDbg(".cv_func_id %u", m_FId);
		Emit(fn->Name);
		EmitDbg(".cv_fpo_proc %s %u", name.c_str(), m_FId);

		Emit("\tpush %s", Reg(RegKind::Bp));
		Emit("\tmov %s, %s", Reg(RegKind::Sp), Reg(RegKind::Bp));
		Emit("\tsub $" FUNCTION_STACKSIZE ", %s", name.c_str(), Reg(RegKind::Sp));
		Emit("");

		// First 4 args are stored in regs
		uint32 paramPos = 8;
		for (uint32 i = 0; i < 4 && i < fn->Parameters.size(); i++)
		{
			paramPos += 8;
			uint32 sz = fn->Parameters[i]->Type->Type->Size;
			decltype(auto) name = fn->Parameters[i]->MangledName;
			Emit(VAR " = %u", name.c_str(), paramPos);
			Emit("\tmov %s, " VAR "(%s)", Reg(ArgRegs[i], sz), name.c_str(), Reg(RegKind::Bp));
		}

		for (uint32 i = 4; i < fn->Parameters.size(); i++)
		{
			paramPos += 8;
			decltype(auto) name = fn->Parameters[i]->MangledName;
			if (!name.empty())
				Emit(VAR " = %u", name.c_str(), paramPos);
		}

		Emit("");
		Emit(fn->Body);

		if (fn->Type->Type->Code != "Void")
		{
			if (fn->IsMain)
				Emit("\txor %s, %s", Reg(RegKind::Ax), Reg(RegKind::Ax));
			else
				Emit("\tud2");
		}

		Emit(FUNCTION_RETURN ":", fn->MangledName.c_str());
		Emit("\tmov %s, %s", Reg(RegKind::Bp), Reg(RegKind::Sp));
		Emit("\tpop %s", Reg(RegKind::Bp));
		Emit("\tret");

		// Debug Symbols (I guess...)
		EmitDbg(".cv_fpo_endproc");
		EmitDbg(".Lfunc_end%u:", m_FId);
		Emit(FUNCTION_STACKSIZE " = %u", name.c_str(), m_StackSize);
		Emit("");
	}


	void Emitter::Emit(const GlobalRef<const StatementAst> &stmt)
	{
		if (!stmt->Token.expired())
			Emit(stmt->Token.lock());

		switch (stmt->Kind)
		{
		case AstKind::BlockStatement:
		{
			uint32 pStack = m_Stack;
			for (auto piece : Cast<const BlockStatementAst>(stmt)->Statements)
				Emit(piece);
			m_Stack = pStack;
			break;
		}
		case AstKind::ReturnStatement:
			Emit(Cast<const ReturnStatementAst>(stmt));
			break;
		case AstKind::ExpressionStatement:
			Emit(Cast<const ExpressionStatementAst>(stmt)->Expression);
			break;
		case AstKind::VariableStatement:
			Emit(Cast<const VariableStatementAst>(stmt));
			break;
		}

		Emit("");
	}

	void Emitter::Emit(const GlobalRef<const ReturnStatementAst> &ret)
	{
		Emit(ret->Value);
		Emit("\tjmp " FUNCTION_RETURN, m_Func->MangledName.c_str());
	}

	void Emitter::Emit(const GlobalRef<const VariableStatementAst> &var)
	{
		decltype(auto) name = var->MangledName;
		auto ty = var->Type->Type;
		uint32 sz = var->Type->Type->Size;
		uint32 pos = Stalloc(sz);
		Emit(VAR " = -%u", name.c_str(), m_Stack);
		if (var->Initializer)
		{
			Emit(var->Initializer);
			if (ty->IsFloat)
				Emit("\tmovs%c %s, " VAR "(%s)", FSuf(ty), Reg(RegKind::Xmm0, sz), name.c_str(), Reg(RegKind::Bp));
			else
				Emit("\tmov %s, " VAR "(%s)", Reg(RegKind::Ax, sz), name.c_str(), Reg(RegKind::Bp));
		}

		if (var->Next)
			Emit(var->Next);
	}


	void Emitter::Emit(const GlobalRef<const ExpressionAst> &expr)
	{
		if (!expr->Token.expired())
			Emit(expr->Token.lock());

		switch (expr->Kind)
		{
		case AstKind::PunExpression:
			Emit(Cast<const PunExpressionAst>(expr));
			break;
		case AstKind::CastExpression:
			Emit(Cast<const CastExpressionAst>(expr));
			break;
		case AstKind::CallExpression:
			Emit(Cast<const CallExpressionAst>(expr));
			break;
		case AstKind::NameExpression:
			Emit(Cast<const NameExpressionAst>(expr));
			break;
		case AstKind::UnaryExpression:
			Emit(Cast<const UnaryExpressionAst>(expr));
			break;
		case AstKind::BinaryExpression:
			Emit(Cast<const BinaryExpressionAst>(expr));
			break;
		case AstKind::BuiltinExpression:
			Emit(Cast<const BuiltinExpressionAst>(expr));
			break;
		case AstKind::LiteralExpression:
			Emit(Cast<const LiteralExpressionAst>(expr));
			break;
		case AstKind::ParenthasizedExpression:
			Emit(Cast<const ParenthasizedExpressionAst>(expr)->Expression);
			break;

		default:
			Emit("\txor %s, %s # Empty expr", Reg(RegKind::Ax), Reg(RegKind::Ax));
			break;
		}
	}

	void Emitter::Emit(const GlobalRef<const PunExpressionAst> &pun)
	{
		Emit(pun->Value);

		auto from = pun->Value->Type;
		auto to = pun->Type;

		if (from->IsFloat && !to->IsFloat)
		{
			uint32 pos = Stalloc();
			Emit("\tmovs%c %s, -%u(%s)", FSuf(from), Reg(RegKind::Xmm0), pos, Reg(RegKind::Bp));
			Emit("\tmov -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Ax));
			Staf();
		}
		else if (!from->IsFloat && to->IsFloat)
		{
			uint32 pos = Stalloc();
			Emit("\tmov %s, -%u(%s)", Reg(RegKind::Ax), pos, Reg(RegKind::Bp));
			Emit("\tmovs%c -%u(%s), %s", FSuf(to), pos, Reg(RegKind::Bp), Reg(RegKind::Xmm0));
			Staf();
		}
	}

	void Emitter::Emit(const GlobalRef<const CastExpressionAst> &cast)
	{
		Emit(cast->Value);
		auto to = cast->Type;
		auto from = cast->Value->Type;

		auto nativeTy = Cast<const NativeType>(to->Base);
		if (!nativeTy)
			throw nullptr; // TODO: Support other casting


		if (to->Is(from));
		else if (nativeTy->Kind == NativeTypeKind::Bool)
		{
			Emit("\ttest %s, %s", Reg(RegKind::Ax), Reg(RegKind::Ax));
			Emit("\tsetne %s", Reg(RegKind::Ax, 1));
			Emit("\tmovz%c%c %s, %s", Suf(1), Suf(), Reg(RegKind::Ax, 1), Reg(RegKind::Ax));
		}
		else if (!from->IsFloat && to->IsFloat)
			Emit("\tcvtsi2s%cq %s, %s", FSuf(to), Reg(RegKind::Ax), Reg(RegKind::Xmm0));
		else if (from->IsFloat && !to->IsFloat)
			Emit("\tcvtts%c2si %s, %s", FSuf(from), Reg(RegKind::Xmm0), Reg(RegKind::Ax));
		else if (from->IsFloat && to->IsFloat)
		{
			if (from->IsF32)
				Emit("\tcvtss2sd %s, %s", Reg(RegKind::Xmm0), Reg(RegKind::Xmm0));
			else
				Emit("\tcvtsd2ss %s, %s", Reg(RegKind::Xmm0), Reg(RegKind::Xmm0));
		}
		else if (from->Size != to->Size)
		{
			uint32 min = std::min(cast->Type->Size, cast->Value->Type->Size);
			uint32 pos = Stalloc(min);
			Emit("\tmov %s, -%u(%s)", Reg(RegKind::Ax, min), pos, Reg(RegKind::Bp));
			Emit("\txor %s, %s", Reg(RegKind::Ax), Reg(RegKind::Ax));
			Emit("\tmov -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Ax, min));
		}
	}

	void Emitter::Emit(const GlobalRef<const CallExpressionAst> &call)
	{
		Emit(call->Function);
		uint32 fnPos = Stalloc();
		Emit("\tmov %s, -%u(%s)", Reg(RegKind::Ax), fnPos, Reg(RegKind::Bp));

		uint32 nArgs = call->Arguments.size();
		for (uint32 i = 0; i < 4 && i < nArgs; i++)
		{
			Emit(call->Arguments[i]);
			Emit("\tmov %s, %s", Reg(RegKind::Ax), Reg(ArgRegs[i]));
		}

		if (nArgs > 4)
		{
			uint32 off = 8 * 4;
			for (uint32 i = 4; i < nArgs; i++)
			{
				Emit(call->Arguments[i]);
				Emit("\tmov %s, %u(%s)", Reg(RegKind::Ax), off, Reg(RegKind::Sp));
				off += 8;
			}

		}

		Emit("\tmov -%u(%s), %s", fnPos, Reg(RegKind::Bp), Reg(RegKind::Ax));
		Staf();
		Emit("\tcall *%s", Reg(RegKind::Ax));
		Reserve(24 + nArgs * 4);
	}

	void Emitter::Emit(const GlobalRef<const NameExpressionAst> &name)
	{
		decltype(auto) sname = name->Symbol->MangledName;
		if (name->Symbol->IsFunction)
			Emit("\tlea " FUNCTION "(%s), %s", sname.c_str(), Reg(RegKind::Ip), Reg(RegKind::Ax));
		else
			if (name->Type->IsFloat)
				Emit("\tmovs%c " VAR "(%s), %s", FSuf(name->Type), sname.c_str(), Reg(RegKind::Bp), Reg(RegKind::Xmm0));
			else
				Emit("\tmov " VAR "(%s), %s", sname.c_str(), Reg(RegKind::Bp), Reg(RegKind::Ax, name->Type->Size));
	}

	void Emitter::Emit(const GlobalRef<const UnaryExpressionAst> &expr)
	{
		if (expr->Operator->Is(TokenKind::At))
		{
			EmitAdr(expr->Operand);
			return;
		}

		Emit(expr->Operand);
		switch (expr->Operator->Kind)
		{
		case TokenKind::Plus:
			Emit("\tand $0x%x, %s", 0x80000000, Reg(RegKind::Ax));
			break;
		case TokenKind::Minus:
			Emit("\tneg %s", Reg(RegKind::Ax));
			break;
		case TokenKind::Star:
			Emit("\tmov (%s), %s", Reg(RegKind::Ax), Reg(RegKind::Ax));
			break;
		}
	}

	void Emitter::Emit(const GlobalRef<const BinaryExpressionAst> &expr)
	{
		bool overrides = OverridesRegs(expr->Left);

		auto ty = expr->Type;
		if (ty->IsFloat)
		{
			char fs = FSuf(ty);
			Emit(expr->Right);
			if (overrides)
			{
				Stalloc(ty->Size);
				uint32 pos = m_Stack;
				Emit("\tmovs%c %s, -%u(%s)", fs, Reg(RegKind::Xmm0), pos, Reg(RegKind::Bp));
				Emit(expr->Left);
				Emit("\tmovs%c -%u(%s), %s", fs, pos, Reg(RegKind::Bp), Reg(RegKind::Xmm1));
				Staf(ty->Size);
			}
			else
			{
				Emit("\tmovs%c %s, %s", fs, Reg(RegKind::Xmm0), Reg(RegKind::Xmm1));
				Emit(expr->Left);
			}

			switch (expr->Operator->Kind)
			{
			case TokenKind::Plus:
				Emit("\tadds%c %s, %s", fs, Reg(RegKind::Xmm1), Reg(RegKind::Xmm0));
				break;
			case TokenKind::Minus:
				Emit("\tsubs%c %s, %s", fs, Reg(RegKind::Xmm1), Reg(RegKind::Xmm0));
				break;
			case TokenKind::Star:
				Emit("\tmuls%c %s, %s", fs, Reg(RegKind::Xmm1), Reg(RegKind::Xmm0));
				break;
			case TokenKind::Slash:
				Emit("\tdivs%c %s, %s", fs, Reg(RegKind::Xmm1), Reg(RegKind::Xmm0));
				break;
			case TokenKind::Percent:
			{
				Stalloc(ty->Size);
				uint32 pos = m_Stack;
				Emit("\tmovs%c %s, %u(%s)", fs, Reg(RegKind::Xmm0), 0, Reg(RegKind::Sp));
				Emit("\tmovs%c %s, %u(%s)", fs, Reg(RegKind::Xmm1), 4, Reg(RegKind::Sp));
				Emit("\tcall %s", ty->IsF32 ? "fmodf" : "fmod");
				Staf(ty->Size);
				break;
			}
			case TokenKind::CarotCarot:
			{
				Stalloc(ty->Size);
				uint32 pos = m_Stack;
				Emit("\tmovs%c %s, %u(%s)", Reg(RegKind::Xmm0), 0, Reg(RegKind::Sp));
				Emit("\tmovs%c %s, %u(%s)", Reg(RegKind::Xmm1), 4, Reg(RegKind::Sp));
				Emit("\tcall %s", ty->IsF32 ? "powf" : "pow");
				Staf(ty->Size);
				break;
			}
			}
		}
		else
		{
			Emit(expr->Right);
			if (overrides)
			{
				Stalloc();
				uint32 pos = m_Stack;
				Emit("\tmov %s, -%u(%s)", Reg(RegKind::Ax), pos, Reg(RegKind::Bp));
				Emit(expr->Left);
				Emit("\tmov -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Bx));
				Staf();
			}
			else
			{
				Emit("\tmov %s, %s", Reg(RegKind::Ax), Reg(RegKind::Bx));
				Emit(expr->Left);
			}


			switch (expr->Operator->Kind)
			{
			case TokenKind::Plus:
				Emit("\tadd %s, %s", Reg(RegKind::Bx), Reg(RegKind::Ax));
				break;
			case TokenKind::Minus:
				Emit("\tsub %s, %s", Reg(RegKind::Bx), Reg(RegKind::Ax));
				break;
			case TokenKind::Star:
				Emit("\timul %s, %s", Reg(RegKind::Bx), Reg(RegKind::Ax));
				break;
			case TokenKind::Slash:
				Emit("\tmov %s, %s", Reg(RegKind::Bx), Reg(RegKind::Cx));
				Emit("\tcltd");
				Emit("\tidiv %s", Reg(RegKind::Cx));
				break;
			case TokenKind::Percent:
				Emit("\tmov %s, %s", Reg(RegKind::Bx), Reg(RegKind::Cx));
				Emit("\tcltd");
				Emit("\tidiv %s", Reg(RegKind::Cx));
				Emit("\tmov %s, %s", Reg(RegKind::Dx), Reg(RegKind::Ax));
				break;
			case TokenKind::Carot:
				Emit("\txor %s, %s", Reg(RegKind::Bx), Reg(RegKind::Ax));
				break;

			case TokenKind::At:
				Emit("\timul $%u, %s", expr->Left->Type->Deref()->Size, Reg(RegKind::Bx));
				Emit("\tadd %s, %s", Reg(RegKind::Bx), Reg(RegKind::Ax));
				break;
			}
		}
	}

	void Emitter::Emit(const GlobalRef<const BuiltinExpressionAst> &macro)
	{
		switch (macro->Macro)
		{
		case TokenKind::SizeofKeyword:
			Emit("\tmov $%u, %s", macro->Arguments[0]->Type->Size, Reg(RegKind::Ax));
			break;
		case TokenKind::LengthofKeyword:
		{
			auto arg = macro->Arguments[0];
			if (arg->Kind != AstKind::NameExpression)
				throw nullptr;
			auto trueArg = Cast<const NameExpressionAst>(arg);
			Emit("\tmov (" VAR " + %u)(%s), %s", trueArg->Symbol->MangledName.c_str(), trueArg->Type->Size, Reg(RegKind::Bp), Reg(RegKind::Ax));
			break;
		}
		}
	}

	void Emitter::Emit(const GlobalRef<const LiteralExpressionAst> &expr)
	{
		auto literal = expr->Literal->Text;

		if (expr->Type->IsFloat)
		{
			union
			{
				float fVal;
				int iVal;
			};
			fVal = strtof(literal.data(), nullptr);
			Stalloc();
			uint32 pos = m_Stack;
			Emit("\tmovl $0x%x, -%u(%s) # Float %g", iVal, pos, Reg(RegKind::Bp), fVal);
			Emit("\tmovss -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Xmm0));
			Staf();
		}
		else if (expr->Literal->Is(TokenKind::Char))
			Emit("\tmov $'%.*s', %s", literal.length(), literal.data(), Reg(RegKind::Ax));
		else if (expr->Literal->Is(TokenKind::String))
		{
			auto txt = expr->Literal->Text;

			Emit(EmitKind::Data, "..L%u:", m_Label);
			Emit(EmitKind::Data, "\t.string \"%.*s\"", txt.length(), txt.data());

			Emit("\tlea ..L%u(%s), %s", m_Label, Reg(RegKind::Ip), Reg(RegKind::Ax));
			m_Label++;
		}
		else
			Emit("\tmov $%.*s, %s", literal.length(), literal.data(), Reg(RegKind::Ax));
	}


	void Emitter::EmitAdr(const GlobalRef<const ExpressionAst> &expr)
	{
		switch (expr->Kind)
		{
		case AstKind::ParenthasizedExpression:
			EmitAdr(Cast<const ParenthasizedExpressionAst>(expr)->Expression);
			break;

		case AstKind::NameExpression:
			EmitAdr(Cast<const NameExpressionAst>(expr));
			break;
		}
	}
	void Emitter::EmitAdr(const GlobalRef<const NameExpressionAst> &name)
	{
		decltype(auto) sname = name->Symbol->MangledName;
		if (name->Symbol->IsFunction)
			Emit("\tlea " FUNCTION "(%s), %s", sname.c_str(), Reg(RegKind::Ip), Reg(RegKind::Ax));
		else
			Emit("\tlea " VAR "(%s), %s", sname.c_str(), Reg(RegKind::Bp), Reg(RegKind::Ax));
	}


	uint32 Emitter::Stalloc(uint32 bytes)
	{
		m_Stack += bytes;
		if (m_Stack > m_StackSize)
			m_StackSize = m_Stack;
		return m_Stack;
	}

	uint32 Emitter::Staf(uint32 bytes)
	{ return m_Stack -= bytes; }

	uint32 Emitter::Reserve(uint32 bytes)
	{
		m_Stack += bytes;
		if (m_Stack > m_StackSize)
			m_StackSize = m_Stack;
		return m_Stack -= bytes;
	}
	
	bool Emitter::OverridesRegs(const GlobalRef<const Ast> node)
	{
		switch (node->Kind)
		{
		case AstKind::ParenthasizedExpression:
			return OverridesRegs(Cast<const ParenthasizedExpressionAst>(node)->Expression);

		case AstKind::BinaryExpression:
		case AstKind::CallExpression:
			return true;

		default:
			return false;
		}
	}


	void Emitter::Emit(const GlobalRef<const Token> &tok)
	{ EmitDbg(".cv_loc %u %u %u %u", m_FId, m_FNum, tok->DisplayLine, tok->Column); }


	template<typename... Args>
	void Emitter::Emit(EmitKind kind, _Printf_format_string_ const char *fmt, Args&&... args)
	{
		FILE *fs;
		switch (kind)
		{
		case EmitKind::Text:
			fs = m_TextFile.Stream;
			break;
		case EmitKind::Data:
			fs = m_DataFile.Stream;
			break;

		default:
			std::abort();
			break;
		}

		std::fprintf(fs, fmt, args...);
		std::fputc('\n', fs);
	}

	template<typename... Args>
	void Emitter::Emit(_Printf_format_string_ const char *fmt, Args&&... args)
	{ Emit(EmitKind::Text, fmt, args...); }


	template<typename... Args>
	void Emitter::EmitDbg(EmitKind kind, _Printf_format_string_ const char *fmt, Args&&... args)
	{
	#if DEBUG_SYMBOLS
		FILE *fs;
		switch (kind)
		{
		case EmitKind::Text:
			fs = m_TextFile.Stream;
			break;
		case EmitKind::Data:
			fs = m_DataFile.Stream;
			break;

		default:
			std::abort();
			break;
		}

		std::fprintf(fs, fmt, args...);
		std::fputc('\n', fs);
	#endif
	}

	template<typename... Args>
	void Emitter::EmitDbg(_Printf_format_string_ const char *fmt, Args&&... args)
	{ EmitDbg(EmitKind::Text, fmt, args...); }

	char Emitter::FSuf(GlobalRef<const Type> ty)
	{
		if (ty->IsF32)
			return 's';
		if (ty->IsFloat)
			return 'd';
		throw nullptr;
	}

	constexpr char Emitter::Suf(uint32 sz)
	{
		if (sz <= 1)
			return 'b';
		if (sz <= 2)
			return 'w';
		if (sz <= 4)
			return 'l';
		if (sz <= 8)
			return 'q';
		throw nullptr;
	}

	constexpr const char *Emitter::Reg(RegKind kind, uint32 sz)
	{
		if (sz)
			switch (kind)
			{
			case RegKind::Xmm0:
				return "%xmm0";
			case RegKind::Xmm1:
				return "%xmm1";
			case RegKind::Xmm2:
				return "%xmm2";
			case RegKind::Xmm3:
				return "%xmm3";
			case RegKind::Xmm4:
				return "%xmm4";
			case RegKind::Xmm5:
				return "%xmm5";
			case RegKind::Xmm6:
				return "%xmm6";
			case RegKind::Xmm7:
				return "%xmm7";

			default:
				if (sz <= 1)
					return Reg8[(uint32)kind];
				if (sz <= 2)
					return Reg16[(uint32)kind];
				if (sz <= 4)
					return Reg32[(uint32)kind];
				if (sz <= 8)
					return Reg64[(uint32)kind];
			}

		throw nullptr;
	}
}