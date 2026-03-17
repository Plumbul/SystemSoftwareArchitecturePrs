#include <iostream>
#include <thread>
#include "modules.hpp"

int main() {
    int initial_data = 100;

    std::cout << "Initial value: " << initial_data << std::endl;

    std::thread thread1(add_func, &initial_data);
    std::thread thread2(sub_func, &initial_data);

    thread1.join();
    thread2.join();

    std::cout << "Result: " << initial_data << std::endl;

    return 0;
}
