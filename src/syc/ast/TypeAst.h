/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/ast/AstNode.h"

namespace syc {
	enum class TypeKind: uint8_t {
#define TYPEKIND(x) x,
#include "syc/ast/TypeKind.h"
#undef TYPEKIND

		Count,
	};

	constexpr const char *TypeKindNames[(uint8_t)TypeKind::Count] {
#define TYPEKIND(x) #x,
#include "syc/ast/TypeKind.h"
#undef TYPEKIND
	};

	ENUM_NAME_FUNC(TypeKind);

	class TypeAst: public AstNode {
	public:
		bool isMutable;

	public:
		TypeAst(bool isMutable):
			isMutable(isMutable) {}

		virtual TypeKind getTypeKind() const = 0;
		virtual uint64_t getSize() const = 0;

	protected:
		void printTypeIndent(std::ostream &os, std::string_view indent, std::string_view label, bool last) const {
			printIndent(os, indent, label, last);
			os << (isMutable ? "mut " : "const ") << getTypeKindName(getTypeKind()) << ':' << getSize() << ' ';
		}
	};

	class PrimitiveTypeAst final: public TypeAst {
	public:
		TypeKind typeKind;
		uint64_t size;

	public:
		PrimitiveTypeAst(bool isMutable, TypeKind typeKind, uint64_t size):
			TypeAst(isMutable), typeKind(typeKind), size(size) {}

		AstKind getKind() const override {
			return AstKind::PrimitiveType;
		}

		TypeKind getTypeKind() const override {
			return typeKind;
		}

		uint64_t getSize() const override {
			return size;
		}

		void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const override;
	};
} // syc
