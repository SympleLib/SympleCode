/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/ast/AstNode.h"
#include "syc/ast/RefTypeKind.h"

namespace syc {
	using TypeFlags = uint8_t;
	namespace TypeFlag {
		enum: TypeFlags {
			None,

			Mutable = 1 << 0,
		};
	}

	enum class TypeKind: uint8_t {
#define TYPEKIND(x) x,
#include "syc/ast/TypeKind.h"

		Count,
	};

	constexpr const char *TypeKindNames[(uint8_t)TypeKind::Count] {
#define TYPEKIND(x) #x,
#include "syc/ast/TypeKind.h"
	};

	ENUM_NAME_FUNC(TypeKind);

	class TypeAst: public AstNode {
	public:
		TypeFlags flags;
		RefTypeKind refKind;

	public:
		TypeAst(TypeFlags flags, RefTypeKind refKind):
				flags(flags), refKind(refKind) {}

		virtual TypeKind getTypeKind() const = 0;
		virtual uint64_t getSize() const = 0;

	protected:
		void printTypeIndent(std::ostream &os, std::string_view indent, std::string_view label, bool last) const {
			printIndent(os, indent, label, last);
			os << (flags & TypeFlag::Mutable ? "mut " : "const ") << getTypeKindName(getTypeKind()) << ':' << getSize() << ' ' << getRefTypeKindName(refKind);
		}
	};

	class PrimitiveTypeAst final: public TypeAst {
	public:
		TypeKind typeKind;
		uint64_t size;

	public:
		PrimitiveTypeAst(TypeFlags flags, RefTypeKind refKind, TypeKind typeKind, uint64_t size):
			TypeAst(flags, refKind), typeKind(typeKind), size(size) {}

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
