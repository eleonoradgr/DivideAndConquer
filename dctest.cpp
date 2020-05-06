#include <vector>
#include <iostream>
#include "DCTemplate.cpp"
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

int main(){

    int fibonacci = dc_seq<int,int>(10, &cond, &solve, &divide, &combine);

    std::cout << fibonacci << "prima";

    fibonacci = dc_par(10, &cond, &solve, &divide, &combine,4);

    std::cout << fibonacci << "dopo";

}
