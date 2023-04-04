#include "TypeSafePrint.hpp"

int main()
{
    int a = 10;
    safePrint::print("int = %d, str = %d, int = %s\n", a, "bebra", 100);
}
