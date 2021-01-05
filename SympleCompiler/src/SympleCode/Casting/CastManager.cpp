#include "SympleCode/Casting/CastManager.h"

#include <algorithm>

namespace Symple
{
	std::map<const Type*, const Type*> CastManager::sTypeCastSheet = {
		{ Type::PrimitiveType::Byte, Type::PrimitiveType::Char },
		{ Type::PrimitiveType::Bool, Type::PrimitiveType::Char },

		{ Type::PrimitiveType::Char, Type::PrimitiveType::Byte },
		{ Type::PrimitiveType::Bool, Type::PrimitiveType::Byte },

		{ Type::PrimitiveType::Char, Type::PrimitiveType::Bool },
		{ Type::PrimitiveType::Byte, Type::PrimitiveType::Bool },


		{ Type::PrimitiveType::Short, Type::PrimitiveType::Bool },
		{ Type::PrimitiveType::Int,   Type::PrimitiveType::Bool },
	};

	std::map<Token::Kind, Token::Kind> CastManager::sModifierCastSheet = {
		{ Token::Kind::Mutable, Token::Kind::Unknown },
	};

	bool CastManager::CanCast(const TypeModifiersNode* from, const TypeModifiersNode* to)
	{
		Token::Kind fromKind = from->IsMutable() ? Token::Kind::Mutable : Token::Kind::Unknown;
		Token::Kind toKind = to->IsMutable() ? Token::Kind::Mutable : Token::Kind::Unknown;

		return CanCast(fromKind, toKind);
	}

	bool CastManager::CanCast(const TypeModifierNode* from, const TypeModifierNode* to)
	{
		return CanCast(from->GetModifier()->GetKind(), to->GetModifier()->GetKind());
	}

	bool CastManager::CanCast(Token::Kind from, Token::Kind to)
	{
		return sModifierCastSheet.find(from) != sModifierCastSheet.end();
	}

	bool CastManager::CanCast(const Type* from, const Type* to)
	{
		return sTypeCastSheet.find(from) != sTypeCastSheet.end();
	}

	bool CastManager::CanCast(const TypeNode* from, const TypeNode* to)
	{
		return CanCast(from->GetType(), to->GetType());
	}
}