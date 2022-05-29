/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

namespace sy {
	using FileId = uint64_t;

	struct Span {
		FileId fileId = 0;
		uint64_t start = 0, end = 0;

		Span() = default;
		Span(FileId fileId, uint64_t start, uint64_t end): fileId(fileId), start(start), end(end) {}
	};

	struct NumConstant {
		union {
			int64_t signedVal = 0;
			uint64_t unsignedVal;
			double floatVal;
		};

		enum Kind {
			SInt,
			UInt,
			Float,
		} kind = SInt;
	};

	struct Token {
		Span span;

		union {
			char *text = nullptr;
			NumConstant numConstant;
		};

		enum Kind {
			Unknown,

			Eof,

			Identifier,
			Number,

			Plus,
			Minus,
			Star,
			Slash,
		} kind = Unknown;

		Token(Kind kind, Span span): kind(kind), span(span) {}
		Token(Kind kind, NumConstant numConstant, Span span): kind(kind), numConstant(numConstant), span(span) {
			assert(kind == Number && "token kind must be `Number` when passing NumConstant");
		}
		Token(Kind kind, std::string_view textView, Span span): kind(kind), span(span) {
			text = new char[textView.length() + 1];
			memcpy(text, textView.data(), textView.length());
			text[textView.length()] = '\0';
			assert(kind == Identifier && "token kind must be `Identifier` when passing text");
		}
		~Token() {
			if (kind == Identifier)
				delete[] text;
		}
	};
}