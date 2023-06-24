#include "SharedPtr.hpp"
#include "UniquePtr.hpp"
#include <chrono>
#include <memory>

int main()
{
    using namespace MySTL;

    auto start = std::chrono::system_clock::now();

    Shared_ptr<int> orig_ptr(new int);

    for (int i = 0; i < 10000; ++i) {
        // int *ptr = new int;
        // delete ptr;

        Shared_ptr<int> ptr(Make_shared<int>());
        std::cout << "old addr = " << ptr.get() << '\n';

        ptr.reset(new float);
        std::cout << "new addr = " << ptr.get() << '\n';
    }

    std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
    std::cout << "duration = " << duration.count() << std::endl;

    std::cout << "final value = " << *orig_ptr << '\n';
}
