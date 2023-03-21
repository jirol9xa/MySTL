#include "TypeSafePrint.hpp"

int main()
{
    int   a   = 10;
    char *str = "str";
    safePrint::print("String = %d\n", str);
}
