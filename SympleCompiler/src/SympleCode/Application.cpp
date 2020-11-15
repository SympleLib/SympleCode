#include <iomanip>
#include <iostream>

#include "SympleCode/Parser.hpp"

int main() {
    const char* source = R"(
int a = 22;
int b = 44;
while b != 0
{;
    if a > b
    {
        a = a - b;
    }
    else
    {
        b = b - a;
    }
}
print(a);
    )";

    Parser::Parse(source);

    //std::cin.get();
}