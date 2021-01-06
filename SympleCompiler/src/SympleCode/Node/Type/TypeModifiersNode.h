#pragma once

#include "SympleCode/Node/Type/TypeModifierNode.h"
#include "SympleCode/Common/Token.h"

#include <vector>

namespace Symple
{
	class TypeModifiersNode : public Node, public TypeNodes
	{
	private:
		const std::vector<const TypeModifierNode*> mModifiers;
		bool mMutable, mUnsigned, mSigned;
	public:
		TypeModifiersNode(const std::vector<const TypeModifierNode*>& modifiers)
			: mModifiers(modifiers), mMutable(), mUnsigned(), mSigned()
		{
			for (const TypeModifierNode* modifier : mModifiers)
			{
				mMutable |= modifier->IsMutable();
				mUnsigned |= modifier->IsUnsigned();
				mSigned |= modifier->IsSigned();
			}
		}

		Kind GetKind() const override
		{
			return Kind::TypeModifiers;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Type Modifiers";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const TypeModifierNode* modifier : mModifiers)
				ss << '\n' << modifier->ToString(indent + newIndent, modifier == mModifiers.back());

			return ss.str();
		}

		const std::vector<const TypeModifierNode*>& GetModifiers() const
		{
			return mModifiers;
		}

		bool IsMutable() const
		{
			return mMutable;
		}

		bool IsUnsigned() const
		{
			return mUnsigned;
		}

		bool IsSigned() const
		{
			return mSigned;
		}
	};
}