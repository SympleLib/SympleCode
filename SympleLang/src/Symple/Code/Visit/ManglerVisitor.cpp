#include "Symple/Code/Visit/Visitor.h"

namespace Symple::Code
{
	ManglerVisitor::ManglerVisitor(GlobalRef<CompilationUnitAst> unit)
		: m_Unit(unit) {}

	void ManglerVisitor::Visit()
	{
		for (auto member : m_Unit->m_Members)
		{
			switch (member->Kind)
			{
			case AstKind::Function:
			{
				m_Depths.push_back(m_Names.size());
				auto fn = Cast<FunctionAst>(member);
				m_Names.push_back(fn);

				std::stringstream ss;
				ss << "_Sy$" << fn->m_Name->Text << "$Func";
				for (auto param : fn->m_Params)
				{
					auto ty = param->Type->Type;
					ss << "$" << ty->MangledName;

					if (param->Name)
					{
						std::stringstream ss;
						ss << "_Sy$" << param->Name->Text << "$Var$" << m_Depths.size();
						param->m_MangledName = std::move(ss.str());
						m_Names.push_back(param);
					}
				}

				Visit(fn->m_Body);
				m_Depths.pop_back();

				fn->m_MangledName = std::move(ss.str());
				m_Names.push_back(fn);
				break;
			}
			case AstKind::ExternFunction:
			{
				m_Depths.push_back(m_Names.size());
				auto fn = Cast<ExternFunctionAst>(member);
				m_Names.push_back(fn);

				std::stringstream ss;
				ss << "_Sy$" << fn->m_Name->Text << "$Func";
				for (auto param : fn->m_Params)
				{
					auto ty = param->Type->Type;
					ss << "$" << ty->MangledName;

					if (param->Name)
					{
						std::stringstream pss;
						pss << "_Sy$" << param->Name->Text << "$Var$" << m_Depths.size();
						param->m_MangledName = std::move(pss.str());
						m_Names.push_back(param);
					}
				}

				m_Depths.pop_back();
				fn->m_MangledName = std::move(ss.str());
				m_Names.push_back(fn);
				break;
			}
			}
		}
	}

	void ManglerVisitor::Visit(GlobalRef<StatementAst> stmt)
	{
		switch (stmt->Kind)
		{
		case AstKind::BlockStatement:
			m_Depths.push_back(m_Names.size());
			for (auto piece : Cast<BlockStatementAst>(stmt)->m_Stmts)
				Visit(piece);
			m_Depths.pop_back();
			break;
		case AstKind::VariableStatement:
		{
			auto var = Cast<VariableStatementAst>(stmt);
			std::stringstream ss;
			var->m_Depth = m_Depths.size();
			ss << "_Sy$" << var->Name->Text << "$Var$" << var->m_Depth;
			var->m_MangledName = std::move(ss.str());
			m_Names.push_back(var);
			break;
		}
		}
	}
}