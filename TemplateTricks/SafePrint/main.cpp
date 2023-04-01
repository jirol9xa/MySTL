#include "TypeSafePrint.hpp"

int main()
{
    int a = 10;
    safePrint::print("int = %d, str = %s, int = %d\n", a, "bebra", 100);
}
