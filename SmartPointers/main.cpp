#include "Shared_ptr.hpp"
#include "Unique_ptr.hpp"
#include <chrono>
#include <memory>

int main()
{
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < 100000000; ++i)
    {
        // int *ptr = new int;
        // delete ptr;

        Shared_ptr<int> ptr = Make_shared<int>(10);
    }

    std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
    std::cout << "duration = " << duration.count() << std::endl;
}
