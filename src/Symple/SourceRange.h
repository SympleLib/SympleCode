/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <cstddef>

namespace Symple {
    struct Position {
        size_t line;
        size_t column;
    };

    struct SourceRange {
        Position start;
        Position end;
    };
}