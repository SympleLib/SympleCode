#pragma once

#include "SympleCode/Node/Type/TypeNode.h"

#include <map>

namespace Symple
{
	struct CastAction {};

	class CastManager
	{
	private:
		static std::map<const Type*, const Type*> sTypeCastSheet;
		static std::map<Token::Kind, Token::Kind> sModifierCastSheet;
	public:
		static bool CanCast(const TypeModifiersNode* from, const TypeModifiersNode* to);
		static bool CanCast(const TypeModifierNode* from, const TypeModifierNode* to);
		static bool CanCast(Token::Kind from, Token::Kind to);

		static bool CanCast(const Type* from, const Type* to);

		static bool CanCast(const TypeNode* from, const TypeNode* to);
	};
}