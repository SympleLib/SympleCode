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

template <typename To, typename From>
inline std::unique_ptr<To> cast(std::unique_ptr<From>&& p) {
	if (To* cast = dynamic_cast<To*>(p.get()))
	{
		std::unique_ptr<To> result(cast);
		p.release();
		return result;
	}

	return std::unique_ptr<To>(nullptr); // or throw std::bad_cast() if you prefer
}
