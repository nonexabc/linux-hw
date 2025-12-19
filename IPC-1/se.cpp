#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>
#include <ucontext.h>

#define checkNeg(var) if(var == -1){ cerr << "something went wrong" << endl; exit(1);}

using namespace std;

void handler(int sig, siginfo_t* info, void* context) {
    struct passwd* pwd;
    string username;
    if((pwd = getpwuid(info->si_uid)) == NULL){
        username = to_string(info->si_uid);
    }else{
        username = pwd->pw_name;
    }
    cout << "Received a SIGUSR1 signal from process " << info->si_pid << " executed by " << info->si_uid <<  " (" << username << ")" << endl;
    
    #ifdef __x86_64__
        ucontext_t* ctx = (ucontext_t*)context;
        cout << "State of the context: EIP = " << ctx->uc_mcontext.gregs[REG_RIP] << ", EAX = " << ctx->uc_mcontext.gregs[REG_RAX]  << ", EBX = " << ctx->uc_mcontext.gregs[REG_RBX] << endl;
    #endif
}



int main() {
    struct sigaction sa = {0};
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;
    pid_t pid = fork();
    checkNeg(pid);


    if(pid == 0){
        checkNeg(sigaction(SIGUSR1, &sa, NULL));
        while(true){
            sleep(5);
        }
    }else{
        sleep(1);
        while(true){
            checkNeg(kill(pid, SIGUSR1));
            sleep(5);
        }
    }

    return 0;
}