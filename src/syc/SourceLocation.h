/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

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