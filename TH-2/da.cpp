#include <iostream>
#include "tpool.h"
#include <unistd.h>

using namespace std;

struct test_args{
    int task_id;
};

void* testFunc1(void* arg){
    test_args args = *(test_args*) arg;
    cout << "testFunc1 " << args.task_id << endl;
    sleep(1);
    cout << "testFunc1 " << args.task_id << " done" << endl;
    return nullptr;
}
void* testFunc2(void* arg){
    test_args args = *(test_args*) arg;
    cout << "testFunc2 " << args.task_id << endl;
    sleep(2);
    cout << "testFunc2 " << args.task_id << " done" << endl;
    return nullptr;
}

void* testFunc3(void* arg){
    test_args args = *(test_args*) arg;
    cout << "testFunc3 " << args.task_id << endl;
    sleep(3);
    cout << "testFunc3 " << args.task_id << " done" << endl;
    return nullptr;
}

int main(){
    parallel_scheduler scheduler(5);
    int i = 0;
    test_args* args = new test_args[30];
    for(; i<30; i+=3){
        args[i].task_id = i;
        args[i+1].task_id = i+1;
        args[i+2].task_id = i+2;
        scheduler.run(testFunc1, &args[i]);
        scheduler.run(testFunc2, &args[i+1]);
        scheduler.run(testFunc3, &args[i+2]);
    }
    return 0;
}