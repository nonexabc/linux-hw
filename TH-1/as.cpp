#include <iostream>
#include <pthread.h>
#include <random>
#include <string>
#include <chrono>

using namespace std;

#define checkNeg(var, msg) if(var == -1){ cerr << msg << endl; exit(errno);}
#define _valtype int8_t // you didn't specify that I need to use bigger types :)
#define _sumtype long long
#define _arrtype const _valtype*

_valtype* randArr(int size){
    // random_device rd;
    // mt19937 gen(rd());
    // uniform_int_distribution<_valtype> dis(-128, 127);
    _valtype* arr = new _valtype[size];

    for(int i=0; i<size; ++i){
        arr[i] = i%255 - 128; // faster
    }
    return arr;
}

struct ThreadArgs{
    _arrtype arr = nullptr;
    int start = 0;
    int end = 0;
    _sumtype* sum = nullptr;
};

void* calculate(void* voidArgs){
    ThreadArgs args = *((ThreadArgs*) voidArgs);
    _sumtype sum = 0;
    for(int i=args.start; i<args.end; ++i){
        sum += args.arr[i];
    }
    *args.sum += sum;
    return nullptr;
}

_sumtype timedSum(_arrtype arr, int size, double& duration){
    _sumtype sum = 0;
    auto start = std::chrono::high_resolution_clock::now();

    ThreadArgs* args = new ThreadArgs{arr, 0, size, &sum};
    calculate(args);
    delete args;

    auto stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration<double>(stop - start).count();
    return sum;
}

_sumtype timedSumThreads(_arrtype arr, int size, int threadCount, double& duration){
    _sumtype* sum = new _sumtype(0);
    auto start = std::chrono::high_resolution_clock::now();
    pthread_t* threads = new pthread_t[threadCount];
    ThreadArgs* tArgs = new ThreadArgs[threadCount];
    int chunk = size / threadCount;
    for(int i=0; i<threadCount; ++i){
        tArgs[i] = ThreadArgs{arr, i*chunk, (i == threadCount-1) ? size : (i+1)*chunk, sum};
        pthread_create(&threads[i], NULL, calculate, tArgs+i);
    }
    for(int i=0; i<threadCount; ++i){
        pthread_join(threads[i], NULL);
    }
    delete [] threads;
    delete [] tArgs;

    auto stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration<double>(stop - start).count();

    _sumtype copy = *sum;
    delete sum;
    return copy;
}


int main(int argc, char* args[]){
    if(argc != 3){
        cerr << "2 args required: number of elements and threads" << endl;
        exit(0);
    }
    int elems = stoi(args[1]);
    int threads = stoi(args[2]);
    if(elems < 1000 || threads < 2){
        cerr << "need at least 1000 elements and 2 threads" << endl;
        exit(0);
    }
    if(threads > elems){
        cerr << "each thread needs at least one element to work with" << endl;
        exit(0);
    }

    _arrtype arr = randArr(elems);
    double singleThreadTime = 0;
    double multiThreadTime = 0;
    _sumtype singleThreadSum = timedSum(arr, elems, singleThreadTime);
    _sumtype multiThreadSum = timedSumThreads(arr, elems, threads, multiThreadTime);

    delete [] arr;

    if(singleThreadSum != multiThreadSum){
        cerr << "the code is shit" << endl;
        cout << "singleThreadSum: " << singleThreadSum << endl;
        cout << "multiThreadSum: " << multiThreadSum << endl;
    }else cout << "Sums - " << singleThreadSum << endl;
    cout << "Time spent without threads: " << singleThreadTime << "s" << endl;
    cout << "Time spent with " << threads << " threads: " << multiThreadTime << "s" << endl; 

    return 0;
}