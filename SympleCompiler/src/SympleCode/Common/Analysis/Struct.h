#pragma once

#include "SympleCode/Common/Analysis/Type.h"
#include "SympleCode/Common/Node/Statement/StructDeclarationNode.h"

namespace Symple
{
	class Struct : public Type
	{
	private:
		const StructDeclarationNode* mDeclaration;
	public:
		Struct(const StructDeclarationNode* declaration)
			: mDeclaration(declaration), Type(std::string(mDeclaration->GetName()->GetLex()), mDeclaration->GetSize()) {}

		const StructDeclarationNode* GetDeclaration() const
		{
			return mDeclaration;
		}
	};
}