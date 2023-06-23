#include "CommonType.hpp"
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
/// That file is made for testing CommonType with compilation errors, so     ///
/// don't try to run it, just look up at the compilations error              ///
////////////////////////////////////////////////////////////////////////////////

template <typename... Ts> struct CommonTypeTest;

struct I {
    int a = 0;
};

struct LL {
    long long a = 0;
};

int main()
{
    CommonTypeTest<MySTL::common_type_t<int, long>>     _;  // long
    CommonTypeTest<MySTL::common_type_t<float, double>> __; // double
    CommonTypeTest<MySTL::common_type_t<decltype(I::a), decltype(LL::a)>>
        ___; // long long
    CommonTypeTest<MySTL::common_type_t<short, char, int16_t, decltype(I::a),
                                        decltype(LL::a)>>
        ____; // long long

    return 0;
}
