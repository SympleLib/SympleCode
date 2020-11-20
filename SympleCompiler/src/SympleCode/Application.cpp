#include <iomanip>
#include <iostream>

#include "SympleCode/Parser.hpp"

int main() {
    const char* source = R"(
1 * 2 + 3
    )";

    Symple::Parser::Parse(source);
    //Tree mTree;
    //    mTree.PushBranch("Sup", 69i64);
    //    Tree& mSup2 = mTree.PushBranch("Sup2");
    //        mSup2.PushBranch("mSup32", 32i64);
    //        Tree& mSup64 = mSup2.PushBranch("mSup64");
    //            mSup64.PushBranch("mSup128", -1i64);
    //            mSup64.PushBranch("Doesn't Exist!!!", 1i64);
    //            mSup64.PushBranch("mSup256", 1337i64);
    //        mTree.FindBranch("Sup2").PushBranch("Sup3", 420i64);
    //    mTree.PushBranch("Sup-1", -69i64);
    //std::cout << mTree;

    //std::cin.get();
}