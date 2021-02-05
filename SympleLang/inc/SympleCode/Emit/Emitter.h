#pragma once

#include "SympleCode/Binding/BoundStatement.h"
#include "SympleCode/Binding/BoundCallExpression.h"
#include "SympleCode/Binding/BoundCompilationUnit.h"
#include "SympleCode/Binding/BoundReturnStatement.h"
#include "SympleCode/Binding/BoundExpressionStatement.h"

#include "SympleCode/Symbol/FunctionSymbol.h"

namespace Symple::Emit
{
	class Emitter
	{
	protected:
		char* mFile = nullptr;
		shared_ptr<Binding::BoundCompilationUnit> mCompilationUnit = nullptr;
	public:
		virtual ~Emitter() {}

		virtual void Compile() = 0;
		virtual void Emit(shared_ptr<Binding::BoundCompilationUnit>) = 0;
		virtual void EmitFunction(shared_ptr<Symbol::FunctionSymbol>, shared_ptr<Binding::BoundStatement>) = 0;

		virtual void EmitStatement(shared_ptr<Binding::BoundStatement>) = 0;
		virtual void EmitBlockStatement(shared_ptr<Binding::BoundBlockStatement>) = 0;
		virtual void EmitReturnStatement(shared_ptr<Binding::BoundReturnStatement>) = 0;
		virtual void EmitExpressionStatement(shared_ptr<Binding::BoundExpressionStatement>) = 0;

		virtual void EmitConstant(shared_ptr<Binding::BoundConstant>) = 0;
		virtual void EmitExpression(shared_ptr<Binding::BoundExpression>) = 0;
		virtual void EmitCallExpression(shared_ptr<Binding::BoundCallExpression>) = 0;
	};
}