#include <vector>
#include <iostream>
#include "DCTemplate.cpp"
#include "utils/utimer.hpp"
/*
 * Operand and Result are just integers
 */
/*
 * Divide Function: recursively compute n-1 and n-2
 */
std::vector<int> divide(int n)
{
    std::vector<int> res = {n-1, n-2};
    return res;
}

/*
 * Base Case
 */
int solve( int n)
{
    return n;
}

/*
 * Combine function
 */
int combine(std::vector<int> couples)
{
    int next = 0;
    for (auto& it : couples){
        next += it;
    }
    return next;
}

/*
 * Condition for base case
 */
bool cond(int n)
{
    return (n<=2);
}

int main(int argc, char *argv[]){
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << std::endl;
        std::cout << "fib : Number of Fibonacci series to calculate (mandatory)" << std::endl;
        std::cout << "nw : Parallelism degree (mandatory)" << std::endl;
        return -1;
    }
    int fib = atoi(argv[1]);
    int nw = atoi(argv[2]);
    int fibonacci = 0;

    {
        utimer omp(" Sequential execution for "+ std::to_string(fib) + "th number \n");
        fibonacci = dc_seq<int,int>(fib, &cond, &solve, &divide, &combine);
    }
    std::cout << "Fibonacci value: " << fibonacci << std::endl;

    {
        utimer omp(" Parallel execution for "+ std::to_string(fib) + "th number with "+ std::to_string(nw) + " trheads \n");
        fibonacci = dc_par(fib, &cond, &solve, &divide, &combine,4);
    }
    std::cout << "Fibonacci value: " << fibonacci << std::endl;


}
