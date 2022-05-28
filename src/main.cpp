#include "sy/Compiler.h"

int main() {
	static_assert(sizeof(size_t) == sizeof(uint64_t), "x64 only, buddy");

	sy::Compiler compiler;
	compiler.Compile("../../samples/test.sy");
}
