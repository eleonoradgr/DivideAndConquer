/**
  Mergesort: sort an array of N integer in parallel using the DAC pattern

  Part of the code was written by Tiziano De Matteis <dematteis@di.unipi.it>
 */
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <cstring>
#include <climits>
#include "utils/utimer.hpp"
#include "DCTemplate.cpp"
#include "assert.h"

#define CUTOFF 5000

/* ------------------------------------------------------------------ */
//maximum value for arrays elements
const int MAX_NUM = 99999.9;
const int SEED = 1234;

static bool isArraySorted(int *a, int n) {
    for (int i = 1; i < n; i++)
        if (a[i] < a[i - 1])
            return false;
    return true;
}

static int *generateRandomArray(int n) {
    srand(SEED);
    int *numbers = new int[n];
    for (int i = 0; i < n; i++)
        numbers[i] = (int) (rand()) / ((RAND_MAX / MAX_NUM));
    return numbers;
}
/* ------------------------------------------------------------------ */


// Operand and Results share the same format
struct ops {
    int *array = nullptr;            //array (to sort/sorted)
    int left = 0;                     //left index
    int right = 0;                    //right index
};

typedef struct ops Operand;
typedef struct ops Result;


/*
 * The divide simply 'split' the array in two: the splitting is only logical.
 * The recursion occur on the left and on the right part
 */
std::vector<Operand> divide(const Operand op) {
    std::vector<Operand> subops;
    subops.reserve(2);
    int mid = (op.left + op.right) / 2;
    Operand a;
    a.array = op.array;
    a.left = op.left;
    a.right = mid;
    subops.push_back(a);

    Operand b;
    b.array = op.array;
    b.left = mid + 1;
    b.right = op.right;
    subops.push_back(b);
    return subops;
}


/*
 * For the base case we resort to std::sort
 */
Result seq(const Operand op) {

    std::sort(&(op.array[op.left]), &(op.array[op.right + 1]));

    //the result is essentially the same of the operand
    return const_cast<Result &>(op);
}


/*
 * The Merge (Combine) function start from two or more ordered sub array and construct the original one
 * It uses additional memory
 */
Result mergeMS(std::vector<Result> ress) {
    //compute what is needed: array pointer, mid, ...
    int *a = ress.front().array;
    int left = ress.front().left;
    int right = ress.back().right;
    int size = right - left + 1;
    int *tmp = new int[size];

    Result ret;

    std::vector<int> indexes(ress.size());
    for (auto i = 0; i < ress.size(); ++i) {
        indexes[i] = ress[i].left;
    }

    //merge in order
    for (int k = 0; k < size; k++) {
        int min = INT_MIN;
        int min_ind = -1;
        for (int i = 0; i < indexes.size(); ++i) {
            if (indexes[i] <= ress[i].right && ress[0].array[indexes[i]] < min) {
                min = ress[0].array[indexes[i]];
                min_ind = i;
            }
        }
        tmp[k] = min;
        indexes[min_ind]++;
    }

    //copy back
    memcpy(a + left, tmp, size * sizeof(int));

    delete[] tmp;

    //build the result
    ret.array = a;
    ret.left = left;
    ret.right = right;
    return ret;
}


/*
 * Base case condition
 */
bool cond(Operand op) {
    return (op.right - op.left <= CUTOFF);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << std::endl;
        std::cout << "n : length of random vector to generate (mandatory)" << std::endl;
        std::cout << "nw : Parallelism degree (mandatory)" << std::endl;
        return -1;
    }
    int n = atoi(argv[1]);
    int nw = atoi(argv[2]);
    int *array = generateRandomArray(n);
    Operand oper;
    oper.array = array;
    oper.left = 0;
    oper.right = n - 1;
    Result res;

    {
        utimer omp(" Sequential execution for " + std::to_string(n) + "th number \n");
        res = dc_seq<Operand, Result>(oper, &cond, &seq, &divide, &mergeMS);
    }
    assert(isArraySorted(res.array, n));
    std::cout << " Sorted: " << isArraySorted(res.array, n) << std::endl;
    free(array);
    array = generateRandomArray(n);
    oper.array = array;
    oper.left = 0;
    oper.right = n - 1;

    {
        utimer omp(" Parallel execution for " + std::to_string(n) + "th number with " + std::to_string(nw) +
                   " trheads \n");
        res = dc_par(oper, &cond, &seq, &divide, &mergeMS, nw);
    }
    std::cout << " Sorted: " << isArraySorted(res.array, n) << std::endl;
    assert(isArraySorted(res.array, n));

}

