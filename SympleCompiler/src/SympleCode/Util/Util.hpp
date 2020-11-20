#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <windows.h>

#define Out(...) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED); fprintf(stdout, __VA_ARGS__); }
#define Err(...) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); fprintf(stderr, __VA_ARGS__); }

#define COut(msg) std::cout << msg;
#define CErr(msg) std::cerr << msg;

namespace Symple
{
	std::vector<std::string> Split(std::string string, const std::string& delimiter);
	void Write(const char* path, const char* text);
	void Write(const std::string& path, const char* text);
	void Write(const std::string& path, const std::string& text);

	int powi(int v, int t);
}