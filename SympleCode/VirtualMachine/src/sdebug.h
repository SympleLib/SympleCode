#pragma once

#include <stdio.h>

#include <exception>

#define S_DEBUG _DEBUG

#define SERROR(fmt, ...) { printf("Error: "); printf(fmt, __VA_ARGS__); printf("\n"); abort(); }
#define SEXCEPTION(fmt, ...) { printf("Exception: "); printf(fmt, __VA_ARGS__); printf("\n"); abort(); }

void throwException(const std::exception&);

#if S_DEBUG
#define SASSERT(c, fmt, ...) if (!(c)) { printf("Assertion Failed (%s:%i): ", __FILE__, __LINE__); printf(fmt, __VA_ARGS__); printf("\n"); abort(); }
#else
#define SASSERT(c, fmt, ...)
#endif