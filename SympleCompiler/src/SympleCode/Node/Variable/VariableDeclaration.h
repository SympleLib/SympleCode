#pragma once

#include "SympleCode/Node/Variable/Variable.h"
#include "SympleCode/Node/Type/TypeNode.h"

namespace Symple
{
	class ExpressionNode;
	class VariableModifiersNode;

	class VariableDeclaration : public Variable
	{
	public:
		virtual const TypeNode* GetType() const = 0;
		virtual const Token* GetName() const = 0;
		virtual const VariableModifiersNode* GetModifiers() const = 0;
		virtual const ExpressionNode* GetInitializer() const = 0;
	};
}