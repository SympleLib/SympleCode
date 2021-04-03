#pragma once

#define VISIT_ME friend class Visitor; \
		friend class SymbolVisitor; \
		friend class TypeVisitor; \
		friend class ManglerVisitor;