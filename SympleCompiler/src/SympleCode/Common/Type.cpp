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

	static TypeModifierNode* MutModifier = new TypeModifierNode(new Token(Token::Kind::Mutable, "mutable", 1, ""));

	TypeNode* VoidType = new TypeNode(Type::PrimitiveType::Void, new TypeModifiersNode({ MutModifier }), nullptr);
	TypeNode* ByteType = new TypeNode(Type::PrimitiveType::Byte, new TypeModifiersNode({ MutModifier }), nullptr);
	TypeNode* ShortType = new TypeNode(Type::PrimitiveType::Short, new TypeModifiersNode({ MutModifier }), nullptr);
	TypeNode* IntType = new TypeNode(Type::PrimitiveType::Int, new TypeModifiersNode({ MutModifier }), nullptr);

	TypeNode* BoolType = new TypeNode(Type::PrimitiveType::Bool, new TypeModifiersNode({ MutModifier }), nullptr);
	TypeNode* CharType = new TypeNode(Type::PrimitiveType::Char, new TypeModifiersNode({ MutModifier }), nullptr);
	TypeNode* WCharType = new TypeNode(Type::PrimitiveType::WChar, new TypeModifiersNode({ MutModifier }), nullptr);

	TypeNode* PtrType = new TypeNode(Type::PrimitiveType::Void, new TypeModifiersNode({ MutModifier }), new TypeContinueNode(new Token(Token::Kind::Asterisk, "*", 1, ""), new TypeModifiersNode({}), nullptr));
	TypeNode* StringType = new TypeNode(Type::PrimitiveType::Char, new TypeModifiersNode({}), new TypeContinueNode(new Token(Token::Kind::Asterisk, "*", 1, ""), new TypeModifiersNode({}), nullptr));

	TypeNode* ErrorType = new TypeNode(Type::PrimitiveType::Error, new TypeModifiersNode({}), nullptr);
}