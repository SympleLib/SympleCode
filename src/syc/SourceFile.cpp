/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "SourceFile.h"

#include <cstdio>

using namespace syc;

SourceFileId SourceFile::nextFileId;
std::unordered_map<SourceFileId, SourceFile> SourceFile::sourceFileMap;

SourceFile::SourceFile(const std::string &fname): filename(fname) {
	FILE *fs = fopen(filename.c_str(), "rb");

	if (!fs) {
		printf("file %s failed to open", filename.c_str());
		fileError = true;
		return;
	}

	fseeko(fs, 0, SEEK_END);
	size_t alloc = (size_t) ftello64(fs);
	content.resize(alloc);
	rewind(fs);
	fread(content.data(), 1, alloc, fs);

	fileId = nextFileId;
}

SourceFile::SourceFile(std::string_view fname, std::string_view data): filename(fname), content(data) {
	fileId = nextFileId;
}