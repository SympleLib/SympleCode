#include "SympleCode/Common/Type.h"

#include "SympleCode/Node/Type/TypeNode.h"

namespace Symple
{
	const Type* const Type::PrimitiveType::Void = new Type("void", 0);
	const Type* const Type::PrimitiveType::Byte = new Type("byte", 1);
	const Type* const Type::PrimitiveType::Short = new Type("short", 2);
	const Type* const Type::PrimitiveType::Int = new Type("int", 4);

	const Type* const Type::PrimitiveType::Bool = new Type("bool", 1);
	const Type* const Type::PrimitiveType::Char = new Type("char", 1);
	const Type* const Type::PrimitiveType::WChar = new Type("wchar", 2);

	const Type* const Type::PrimitiveType::Error = new Type("error-type", -1);

	const std::vector<const Type*> Type::PrimitiveTypes = {
		Type::PrimitiveType::Void,
		Type::PrimitiveType::Byte,
		Type::PrimitiveType::Short,
		Type::PrimitiveType::Int,

		Type::PrimitiveType::Bool,
		Type::PrimitiveType::Char,
		Type::PrimitiveType::WChar,
	};

	const TypeModifierNode* const MutModifier = new TypeModifierNode(new Token(Token::Kind::Mutable, "mutable", 7, ""));
	const TypeModifiersNode* const MutModifiers = new TypeModifiersNode({ MutModifier });
	const TypeModifiersNode* const EmptyModifiers = new TypeModifiersNode({});

	const Token* const PtrToken = new Token(Token::Kind::Asterisk, "*", 1, "");
	const TypeContinueNode* const PtrContinue = new TypeContinueNode(PtrToken, EmptyModifiers, nullptr);

	const TypeNode* const VoidType = new TypeNode(Type::PrimitiveType::Void, MutModifiers, nullptr);
	const TypeNode* const ByteType = new TypeNode(Type::PrimitiveType::Byte, MutModifiers, nullptr);
	const TypeNode* const ShortType = new TypeNode(Type::PrimitiveType::Short, MutModifiers, nullptr);
	const TypeNode* const IntType = new TypeNode(Type::PrimitiveType::Int, MutModifiers, nullptr);

	const TypeNode* const BoolType = new TypeNode(Type::PrimitiveType::Bool, MutModifiers, nullptr);
	const TypeNode* const CharType = new TypeNode(Type::PrimitiveType::Char, MutModifiers, nullptr);
	const TypeNode* const WCharType = new TypeNode(Type::PrimitiveType::WChar, MutModifiers, nullptr);

	const TypeNode* const PtrType = new TypeNode(Type::PrimitiveType::Void, MutModifiers, PtrContinue);
	const TypeNode* const StringType = new TypeNode(Type::PrimitiveType::Char, EmptyModifiers, PtrContinue);

	const TypeNode* const ErrorType = new TypeNode(Type::PrimitiveType::Error, EmptyModifiers, nullptr);
}