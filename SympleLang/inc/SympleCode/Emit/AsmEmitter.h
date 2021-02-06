#pragma once

#include <cstdio>

#include "SympleCode/Emit/Emitter.h"
#include "SympleCode/Binding/BoundScope.h"

namespace Symple::Emit
{
	class AsmEmitter : public Emitter
	{
	protected:
		char* mFile;
		FILE* mTextStream;
		FILE* mDataStream;

		shared_ptr<Symbol::FunctionSymbol> mFunction;
		shared_ptr<Binding::BoundScope> mScope;

		void BeginScope();
		void EndScope();

		bool mClosed = false;
		bool mReturning;
	public:
		AsmEmitter(char* file = nullptr);
		virtual ~AsmEmitter() override;

		virtual void Compile() override;
		virtual void Emit(shared_ptr<Binding::BoundCompilationUnit>) override;
		virtual void EmitFunction(shared_ptr<Symbol::FunctionSymbol>, shared_ptr<Binding::BoundStatement>) override;

		virtual void EmitStatement(shared_ptr<Binding::BoundStatement>) override;
		virtual void EmitBlockStatement(shared_ptr<Binding::BoundBlockStatement>) override;
		virtual void EmitReturnStatement(shared_ptr<Binding::BoundReturnStatement>) override;
		virtual void EmitExpressionStatement(shared_ptr<Binding::BoundExpressionStatement>) override;

		virtual void EmitConstant(shared_ptr<Binding::BoundConstant>) override;
		virtual void EmitExpression(shared_ptr<Binding::BoundExpression>) override;
		virtual void EmitCallExpression(shared_ptr<Binding::BoundCallExpression>) override;
		virtual void EmitVariableExpression(shared_ptr<Binding::BoundVariableExpression>) override;
	private:
		void CloseStreams();
	};
}