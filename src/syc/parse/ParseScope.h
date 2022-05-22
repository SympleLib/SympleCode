/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <unordered_map>
#include <string>

#include "syc/ast/Ast.h"

namespace syc {
	class ParseScope {
	public:
		ParseScope *parent;

		std::unordered_map<std::string_view, TypeKind> types;
		std::unordered_map<std::string_view, VariableStmtAst *> vars;

	public:
		ParseScope(ParseScope *parent = nullptr):
			parent(parent) {}

		void defineType(std::string_view name, TypeKind typeKind) {
			if (types.contains(name))
				printf("type '%.*s' already defined in scope", name.size(), name.data());
			types[name] = typeKind;
		}

		TypeKind getType(std::string_view name) const {
			if (types.contains(name))
				return types.at(name);
			return parent ? parent->getType(name) : TypeKind::None;
		}

		bool hasType(std::string_view name) const {
			return types.contains(name) || (parent && parent->hasType(name));
		}

		void defineVar(VariableStmtAst *var) {
			// we can actually have multiple vars with same name :)
			vars[var->name] = var;
		}

		VariableStmtAst *findVar(std::string_view name) const {
			if (!vars.contains(name))
				return parent ? parent->findVar(name) : nullptr;
			return vars.at(name);
		}
	};
} // syc
