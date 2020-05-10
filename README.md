## Divide and Conquer function template 

The <b>sequential</b> function template has been left identical to that given by Professor Danelutto. <br>
For the <b>parallel</b> version, I’ve used the idea which is similar to that one behind the divide and conquer pattern in Fast Flow. <br>
The main thread generates a set of tasks whose cardinality is at least equal to the number of workers nw, required by the user, and their dimension is equal, in this way jobs are more equally distributed among threads.<br>
I made the following assumption: the number of subproblems obtained by divide function is known at run-time and so for simplicity the thread working as Emitter goes on dividing until at least each worker has his job, that is number of subproblems > nw . A more powerful approach would have been investigating the division factor, in this way if all tasks had the same difficulty it would have lead surely to a better solution.<br>
Workers are synchronized through an atomic variable that stores the number of remaining tasks. Each worker executes a loop and gets his job until this variable is set to zero.<br>
Each thread goes on computing the divide and conquer schema sequentially on his job, storing the result of the computation before proceeding on another task. <br>
When all the workers finish their jobs, the thread that acts as emitter executes the conquer function on the results provided by the workers. <br>


The following graph shows the Speedup achieved by the divide and conquer parallel function template in the computation  of the 40-th element of Fibonacci serie. The functions for computing Fibonacci are similar to those presented in lesson 26.
![Fibonacci](https://github.com/eleonoradgr/DivideAndConquer/blob/master/img/fibonacci_su.png)





The following graph shows the Speedup achieved by the divide and conquer parallel function template in the computation  of Merge Sort over a vector of 100000000 elements. The functions for computing MergeSort are similar to those presented in lesson 26.
![Mergesort](https://github.com/eleonoradgr/DivideAndConquer/blob/master/img/merge_su.png)



