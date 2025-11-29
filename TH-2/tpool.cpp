#include "tpool.h"

void parallel_scheduler::_onFreed(){
    _mutex.lock();
    int tid = pthread_self();
    for(size_t i=0; i<_cap; ++i){
        if(_threads[i] == tid){
            pthread_join(tid, NULL);
            _threads[i] = -1;
            _dispatch();
            _mutex.unlock();
            return;
        }
    }
    throw std::runtime_error("Thread not found");
}

void parallel_scheduler::_dispatch(){
    std::cout << "starting to dispatch" << std::endl;
    if(_tasks.size() == 0) {
        _mutex.unlock();
        return;
    };
    auto task = _tasks.front();
    for(size_t i=0; i<_cap; ++i){
        if(_threads[i] == -1){ // doesn't exist / free
            launch_args largs{task, this};
            _tasks.pop();
            pthread_create(&_threads[i], NULL, launch_wrapper, &largs);
            if(_threads[i] == -1) throw std::runtime_error("Failed to create a thread");
            return;
        }
    }
}

parallel_scheduler::parallel_scheduler(size_t capacity){
    _threads = new pthread_t[capacity];
    _cap = capacity;
    for(size_t i=0; i<capacity; ++i){
        _threads[i] = -1;
    }
}
parallel_scheduler::~parallel_scheduler() {
    _mutex.lock();
    delete [] _threads;
    for(size_t i=0; i<_cap; ++i){
        if(_threads[i] != -1){
            pthread_join(_threads[i], NULL);
        }
    }
    _mutex.unlock();
}

void parallel_scheduler::run(void* (*func)(void*), void* arg) {
    thread_task task{func, arg};
    _mutex.lock();
    _tasks.push(task);
    std::cout << "scheduling task. current size - " << _tasks.size() << std::endl;
    _dispatch();
    _mutex.unlock();
}

void* launch_wrapper(void* largs) {
    launch_args* args = (launch_args*) largs;
    thread_task task = args->task;
    void* res = task.func(task.arg);
    args->instance->_onFreed();
    return res;
}
