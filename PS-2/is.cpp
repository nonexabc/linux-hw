#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <vector>
#include <cstring>

using namespace std;

#define checkNeg(var, msg) if(var == -1){ cerr << msg << endl; exit(errno);}


void do_command(string cmd, bool silent, char* pdir){
    pid_t pid = fork();
    checkNeg(pid, "could not create a child process");
    
    if(pid == 0){
        const char* path = getenv("PATH");
        string newPath = string(pdir) + ":"s + string(path);
        setenv("PATH", newPath.c_str(), 1);
        if(silent){
            int outcpy = dup(1);
            checkNeg(outcpy, "could not redirect stdout");
            int errcpy = dup(2);
            checkNeg(errcpy, "could not redirect stderr");        
            int fd = open((to_string((int) getpid()) + ".log"s).data(), O_WRONLY | O_CREAT | O_TRUNC, 0755);
            checkNeg(fd, "couldn't open the .log file");
            dup2(fd, 1);
            dup2(fd, 2);
            close(fd);
        }
        vector<char*> args;
        char* token = strtok(cmd.data(), " ");
        while(token){
            args.push_back(token);
            token = strtok(nullptr, " ");
        }
        args.push_back(nullptr);
        execvp(args[0], args.data());
        cerr << "error executing the command"  << endl;
        exit(errno);
    }
    if(pid > 0){
        int childStatus = -1;
        pid_t waiting = waitpid(pid, &childStatus, 0);
    }
}

void open_shell(){
    char* cwd = new char[1024];
    getcwd(cwd, 1024);
    while(true){
        cout << "> ";
        string cmd;
        std::getline(cin, cmd);
        if(cmd == "exit"){
            cout << "exited" << endl;
            break;
        }
        bool sil = cmd.starts_with("silent "s);
        do_command((sil) ? cmd.substr(7) : cmd, sil, cwd);
    }
    delete [] cwd;
}


int main(){
    open_shell();    
    return 0;
}
