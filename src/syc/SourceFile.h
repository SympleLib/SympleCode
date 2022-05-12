/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>

namespace syc {
	using SourceFileId = size_t;

	class SourceFile {
	private:
		static SourceFileId nextFileId;
		static std::unordered_map<SourceFileId, SourceFile> sourceFileMap;

	public:
		bool fileError;
		std::string filename;
		SourceFileId fileId;

		std::string content;

	private:
		SourceFile(const std::string &filename);
		SourceFile(std::string_view filename, std::string_view content);

	public:

		static bool getFileError(SourceFileId fileId) {
			return sourceFileMap.at(fileId).fileError;
		}

		static const std::string &getFilename(SourceFileId fileId) {
			return sourceFileMap.at(fileId).filename;
		}

		static const std::string &getContent(SourceFileId fileId) {
			return sourceFileMap.at(fileId).content;
		}

		static SourceFileId ReadFile(const std::string &filename) {
			sourceFileMap.insert({ nextFileId, SourceFile(filename) });
			return nextFileId++;
		}

		static SourceFileId MakeFile(const std::string &filename, std::string_view content) {
			sourceFileMap.insert({ nextFileId, SourceFile(filename, content) });
			return nextFileId;
		}
	};
} // syc
