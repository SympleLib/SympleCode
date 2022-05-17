/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#define ENUM_NAME_FUNC(enumName) inline constexpr const char *get##enumName##Name(enumName x) { return enumName##Names[(uint64_t) x]; }