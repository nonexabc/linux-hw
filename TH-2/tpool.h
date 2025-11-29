#ifndef TH_2_TPOOL
#define TH_2_TPOOL

#include <iostream>
#include <pthread.h>
#include <queue>
#include <stdexcept>
#include <mutex>

struct thread_task{
    void* (*func)(void*);
    void* arg;
};


class parallel_scheduler {
    private:
        size_t _cap;
        pthread_t* _threads;
        std::queue<thread_task> _tasks;
        std::mutex _mutex;

        void _onFreed();
        void _dispatch();

    public:
        parallel_scheduler(size_t capacity);
        ~parallel_scheduler();
        void run(void* (*func)(void*), void* arg);

        friend void* launch_wrapper(void* largs);
};

void* launch_wrapper(void* largs);

struct launch_args{
    thread_task task;
    parallel_scheduler* instance;
};

#endif