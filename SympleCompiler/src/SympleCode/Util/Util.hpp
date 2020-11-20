#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <windows.h>

#define COLORED_CONSOLE 1

#if COLORED_CONSOLE
#define Out(...) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); fprintf(stdout, __VA_ARGS__); }
#define Err(...) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED); fprintf(stderr, __VA_ARGS__); }

#define COut(msg) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); std::cout << msg; }
#define CErr(msg) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED);  std::cerr << msg; }
#else
#define Out(...) fprintf(stdout, __VA_ARGS__)
#define Err(...) fprintf(stderr, __VA_ARGS__)

#define COut(msg) std::cout << msg
#define CErr(msg) std::cerr << msg
#endif

namespace Symple
{
	std::vector<std::string> Split(std::string string, const std::string& delimiter);
	void Write(const char* path, const char* text);
	void Write(const std::string& path, const char* text);
	void Write(const std::string& path, const std::string& text);

	int powi(int v, int t);
}