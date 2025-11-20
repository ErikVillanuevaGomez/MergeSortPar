# Parallel Merge Sort




# Build instructions

Run the following commands in the project directory:



    make clean

    make

    

This creates an executable named mergesort.

Note: This requires omp_tasking.hpp to be in the same directory to compile.



# Usage

Next run the following command with the desired parameters:



    ./mergesort <n> <nbthreads> [threshold]


<n>: The size of the array to generate and sort (e.g., 50000000).

<nbthreads>: The number of threads to use for parallel execution (e.g., 8).

[threshold]: (Optional) The array size threshold below which the algorithm switches to sequential sort (default is 1000).


For example, to sort an array of 50 million elements with 8 threads and a threshold of 1000:

    ./mergesort 50000000 8 1000




# Clean

To remove the compiled executable run the following command:



    make clean



# BenchMark

When Benchmarking on Centaurus:



    cd MergeSortPar

    chmod +x queue_main.sh

    sbatch queue_main.sh
    
This executes the batch script, which will compile the code and run the benchmarks defined in queue_main.sh.

It creates the file results.txt, which will contain the output and runtimes for the different benchmark cases (Small, Medium, Large) and thread counts.
