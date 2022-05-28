/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

namespace sy {
	struct File {
		std::string filename;
		std::string source;

		File(std::string &&filename, std::string &&source): filename(std::move(filename)), source(std::move(source)) {}
	};

	class Compiler {
	private:
		std::vector<File> files;
	public:
		void Compile(std::string filename);
	};
} // sy
