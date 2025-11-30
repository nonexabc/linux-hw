#include "tpool.h"

void parallel_scheduler::_lock(){
    _mutex.lock();
}

void parallel_scheduler::_unlock(){
    _mutex.unlock();
}

parallel_scheduler::parallel_scheduler(size_t capacity, unsigned short sleepMillis) {
    _threads = new pthread_t[capacity];
    _sleepMillis = sleepMillis;
    _cap = capacity;
    for(size_t i=0; i<capacity; ++i){
        pthread_create(&_threads[i], NULL, _worker, this);
    }
}
parallel_scheduler::~parallel_scheduler() {
    _lock();
    _stop = true;
    for(size_t i=0; i<_cap; ++i){
        if(_threads[i] != -1){
            pthread_join(_threads[i], NULL);
        }
    }
    delete [] _threads;
    _unlock();
}

void parallel_scheduler::run(void* (*func)(void*), void* arg) {
    thread_task task{func, arg};
    _lock();
    _tasks.push(task);
    std::cout << "scheduling task. current size - " << _tasks.size() << std::endl;
    _unlock();
}

void parallel_scheduler::stop(){
    _stop = true;
}

void parallel_scheduler::waitForCompletion(){
    _lock();
    while(!_tasks.empty()){
        _unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(_sleepMillis*5));
        _lock();
    }
    _unlock();
}

void* _worker(void* arg){
    parallel_scheduler* scheduler = (parallel_scheduler*) arg;
    while(!(scheduler->_stop)){
        scheduler->_lock();
        if(!(scheduler->_tasks.empty())){
            thread_task task = scheduler->_tasks.front();
            scheduler->_tasks.pop();
            scheduler->_unlock();
            task.func(task.arg);
        }else{
            scheduler->_unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(scheduler->_sleepMillis));
    }
    return nullptr;
}
