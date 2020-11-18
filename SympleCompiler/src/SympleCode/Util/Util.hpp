#pragma once

#include <vector>
#include <string>
#include <iostream>

#define Out(...) fprintf(stdout, __VA_ARGS__)
#define Err(...) fprintf(stderr, __VA_ARGS__)

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