#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include "omp_tasking.hpp"
#include <omp.h>

#define DEBUG 0

void generateMergeSortData (std::vector<int>& arr, size_t n) {
    for (size_t  i=0; i< n; ++i) {
        arr[i] = rand();
    }
}

void checkMergeSortResult (std::vector<int>& arr, size_t n) {
    bool ok = true;
    for (size_t  i=1; i<n; ++i){
        if (arr[i]< arr[i-1]){
            ok = false;
        }
    }
    if(!ok){
        std::cerr<<"notok"<<std::endl;
    }
}

void merge(int * arr, size_t  l, size_t  mid, size_t r, int* temp) {
    if (l == r) return;
    if (r-l == 1) {
        if (arr[l] > arr[r]) {
            size_t swap_temp = arr[l];
            arr[l] = arr[r];
            arr[r] = swap_temp;
        }
        return;
    }
    
    size_t i, j, k;
    size_t n = mid - l;
    
    for (i=0; i<n; ++i)
        temp[i] = arr[l+i];
    
    i = 0;
    j = mid;
    k = l;
    
    while (i<n && j<=r) {
        if (temp[i] <= arr[j] ) {
            arr[k++] = temp[i++];
        } else {
            arr[k++] = arr[j++];
        }
    }
    
    while (i<n) {
        arr[k++] = temp[i++];
    }
}

void mergesort_serial(int * arr, size_t l, size_t r, int* temp) {
    if (l < r) {
        size_t mid = (l+r)/2;
        mergesort_serial(arr, l, mid, temp);
        mergesort_serial(arr, mid+1, r, temp);
        merge(arr, l, mid+1, r, temp + l);
    }
}

void mergesort_parallel(int * arr, size_t l, size_t r, int* temp, int threshold) {
    
    if (l < r) {
        if ((r - l) < (size_t)threshold) {
            mergesort_serial(arr, l, r, temp);
            return;
        }
        
        size_t mid = (l+r)/2;
        
        tasking::taskstart([=]() {
            mergesort_parallel(arr, l, mid, temp, threshold);
        });
        
        tasking::taskstart([=]() {
            mergesort_parallel(arr, mid+1, r, temp, threshold);
        });
        
        tasking::taskwait();
        
        merge(arr, l, mid+1, r, temp + l);
    }
}

int main (int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> [threshold]"<<std::endl;
        return -1;
    }
    
    size_t n = atol(argv[1]);
    int nbthreads = atoi(argv[2]);
    int threshold = (argc > 3) ? atoi(argv[3]) : 1000;
    
    std::vector<int> arr (n);
    generateMergeSortData (arr, n);
    std::vector<int> temp (n);
    
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
    tasking::doinparallel([&](){
        mergesort_parallel(&(arr[0]), 0, n-1, &(temp[0]), threshold);
    }, nbthreads);
    
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapased_seconds = end-start;
    
    std::cerr<<elapased_seconds.count()<<std::endl;
    checkMergeSortResult (arr, n);
    
    return 0;
}
