/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <cstddef>

#include "SourceFile.h"

namespace syc {
    struct SourceLocation {
		SourceFileId fileId;
        size_t line;
        size_t column;
    };

    struct SourceRange {
		SourceLocation start;
		SourceLocation end;
    };
}