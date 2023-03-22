#include "TypeSafePrint.hpp"

struct Xui {};

int main()
{
    int   a   = 10;
    Xui xui;
    safePrint::print("String = %d\n", a);
}
