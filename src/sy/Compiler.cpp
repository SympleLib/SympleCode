/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "sy/Compiler.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "sy/Lexer.h"

using namespace sy;

void Compiler::Compile(std::string filename) {
	if (!std::filesystem::exists(filename)) {
		std::cout << "file doesn't exist\n";
		return;
	}

	std::ifstream fs(filename);
	if (!fs.is_open()) {
		std::cout << "file failed to open\n";
		return;
	}

	std::stringstream ss;
	ss << fs.rdbuf();

	File &file = files.emplace_back(std::move(filename), ss.str());
//	std::cout << file.source << '\n';

	std::vector<Token> tokens = lex(files.size() - 1, file.source);
	if (tokens.size() > 0) {
		uint64_t idx = 0;
		Token *curr = &tokens[idx];
		std::string str;
		for (uint64_t i = 0; i < file.source.length(); i++) {
			if (i == curr->span.start) {
				std::cout << "|";
				str += "|";
				str += curr->kind + '0';
			}
			else if (i == curr->span.end) {
				std::cout << "|";
				str += "|";
				if (++idx < tokens.size())
					curr = &tokens[idx];

				str += file.source[i];
			} else {
				if (i >= curr->span.start)
					str += '.';
				else
					str += file.source[i];
			}
			std::cout << file.source[i];
		}

		if (curr->span.end == file.source.size()) {
			std::cout << "|";
			str += "|";
		}
		std::cout << "\n\n----------\n\n" << str << "\n";
	}
}