/**
  Mergesort: sort an array of N integer in parallel using the DAC pattern

  Author: Tiziano De Matteis <dematteis@di.unipi.it>
 */
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <cstring>
#include "utils/utimer.hpp"

#define CUTOFF 2000

/* ------------------------------------------------------------------ */
//maximum value for arrays elements
const int MAX_NUM=99999.9;
const int SEED = 1234;

static bool isArraySorted(int *a, int n) {
    for(int i=1;i<n;i++)
        if(a[i]<a[i-1])
            return false;
    return true;
}

static int *generateRandomArray(int n) {
    srand (SEED);
    int *numbers=new int[n];
    for(int i=0;i<n;i++)
        numbers[i]=(int) (rand()) / ((RAND_MAX/MAX_NUM));
    return numbers;
}
/* ------------------------------------------------------------------ */


// Operand and Results share the same format
struct ops{
    int *array=nullptr;		    //array (to sort/sorted)
    int left=0;                     //left index
    int right=0;                    //right index
};

typedef struct ops Operand;
typedef struct ops Result;


/*
 * The divide simply 'split' the array in two: the splitting is only logical.
 * The recursion occur on the left and on the right part
 */
void divide(const Operand &op,std::vector<Operand> &subops)
{
    int mid=(op.left+op.right)/2;
    Operand a;
    a.array=op.array;
    a.left=op.left;
    a.right=mid;
    subops.push_back(a);

    Operand b;
    b.array=op.array;
    b.left=mid+1;
    b.right=op.right;
    subops.push_back(b);
}


/*
 * For the base case we resort to std::sort
 */
void seq(const Operand &op, Result &ret)
{

    std::sort(&(op.array[op.left]),&(op.array[op.right+1]));

    //the result is essentially the same of the operand
    ret=op;
}


/*
 * The Merge (Combine) function start from two ordered sub array and construct the original one
 * It uses additional memory
 */
void mergeMS(std::vector<Result>&ress, Result &ret)
{
    //compute what is needed: array pointer, mid, ...
    int *a=ress[0].array;                 //get the array
    int mid=ress[0].right;                //by construction
    int left=ress[0].left, right=ress[1].right;
    int size=right-left+1;
    int *tmp=new int[size];
    int i=left,j=mid+1;

    //merge in order
    for(int k=0;k<size;k++)
    {
        if(i<=mid && (j>right || a[i]<= a[j]))
        {
            tmp[k]=a[i];
            i++;
        }
        else
        {
            tmp[k]=a[j];
            j++;
        }
    }

    //copy back
    memcpy(a+left,tmp,size*sizeof(int));

    delete[] tmp;

    //build the result
    ret.array=a;
    ret.left=left;
    ret.right=right;
}


/*
 * Base case condition
 */
bool cond(const Operand &op)
{
    return (op.right-op.left<=CUTOFF);
}

int main(int argc, char *argv[]){
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << std::endl;
        std::cout << "n : length of random vector to generate (mandatory)" << std::endl;
        std::cout << "nw : Parallelism degree (mandatory)" << std::endl;
        return -1;
    }
    int n = atoi(argv[1]);
    int nw = atoi(argv[2]);
    int *array = generateRandomArray(n);

    {
        utimer omp(" Sequential execution for "+ std::to_string(fib) + "th number \n");
        fibonacci = dc_seq<int,int>(fib, &cond, &solve, &divide, &combine);
    }
    std::cout << "Fibonacci value: " << fibonacci << std::endl;
    int *array = generateRandomArray(n);
    {
        utimer omp(" Parallel execution for "+ std::to_string(fib) + "th number with "+ std::to_string(nw) + " trheads \n");
        fibonacci = dc_par(fib, &cond, &solve, &divide, &combine,4);
    }
    std::cout << "Fibonacci value: " << fibonacci << std::endl;


}

