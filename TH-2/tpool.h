#ifndef TH_2_TPOOL
#define TH_2_TPOOL

#include <iostream>
#include <pthread.h>
#include <queue>
#include <stdexcept>
#include <mutex>
#include <chrono>
#include <thread>

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
        bool _stop = false;
        unsigned short _sleepMillis;

        void _lock();
        void _unlock();

    public:
        parallel_scheduler(size_t capacity, unsigned short sleepMillis = 10);
        ~parallel_scheduler();
        void run(void* (*func)(void*), void* arg);

        void stop();
        void waitForCompletion();

        friend void* _worker(void* arg);
};

void* _worker(void* arg);

#endif