/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "SourceFile.h"

namespace syc {
    struct SourceLocation {
		SourceFileId fileId;
        uint64_t line;
		uint64_t column;
    };

    struct SourceRange {
		SourceLocation start;
		SourceLocation end;
    };
}