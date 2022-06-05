/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <functional>

template <typename To, typename From>
inline std::unique_ptr<To> cast(std::unique_ptr<From>&& p) {
	return std::unique_ptr<To>(static_cast<To *>(p.release()));
}
