#include <gtest/gtest.h>
#include "test_helpers.h"
#include <iostream>
#include <cstdarg>

 
int add_nums(int count...)
{
    int result = 0;
    std::va_list args;
    std::cout << count << '\n';
    va_start(args, count);
    for (int i = 0; i < count; ++i)
        result += va_arg(args, int);
    va_end(args);
    return result;
}
 
int main()
{
    std::cout << add_nums(4, 25, 25, 50, 50) << '\n';
}