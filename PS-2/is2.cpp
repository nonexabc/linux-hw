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

vector<string> normal_fucking_split(string input, string delimiter){
    vector<string> result;
    size_t start = 0;
    size_t end = input.find(delimiter);
    while (end != string::npos) {
        result.push_back(input.substr(start, end - start));
        start = end+delimiter.length();
        end = input.find(delimiter, start);
    }
    result.push_back(input.substr(start));
    return result;
}

int execute(string cmd, char* pdir, bool redirect, string fname, bool erase=false){
    if(cmd.starts_with("silent"s)){
        redirect = true;
        fname = ""s;
    }
    
    pid_t pid = fork();
    checkNeg(pid, "could not create a child process");
    if(pid == 0){
        const char* path = getenv("PATH");
        string newPath = string(pdir) + ":"s + string(path);
        setenv("PATH", newPath.c_str(), 1);
        if(redirect){
            int outcpy = dup(1);
            checkNeg(outcpy, "could not redirect stdout");
            int errcpy = dup(2);
            checkNeg(errcpy, "could not redirect stderr");        
            int fd = open((fname.empty()) ? ((to_string((int) getpid()) + ".log"s).data()) : (fname.data()), O_WRONLY | O_CREAT | ((erase) ? O_TRUNC : O_APPEND), 0755);
            checkNeg(fd, "couldn't open the redirect file");
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
        return childStatus;
    }
}

// it wasn't specified that I need to implement operator mixing
void process(char* pdir, string cmd){ 
    string cmdCpy = cmd;
    if(cmd.find(" && ") != string::npos){
        vector<string> vec = normal_fucking_split(cmd, " && ");
        
        for(auto& c : vec){
            int res = execute(c, pdir, false, "");
            if(res != 0){
                cerr << "finished after " << c << ". resulted in an error" << endl;
                break;
            }
        }
    } else if(cmd.find(" || ") != string::npos){
        vector<string> vec = normal_fucking_split(cmd, " || ");

        for(auto& c : vec){
            int res = execute(c, pdir, false, "");
            if(res == 0){
                cerr << "finished after " << c << ". didn't result in an error" << endl;
                break;
            }
        }
    } else if(cmd.find("; ") != string::npos){
        vector<string> vec = normal_fucking_split(cmd, "; ");

        for(auto& c : vec){
            execute(c, pdir, false, "");
        }
    } else if(cmd.find(" > ") != string::npos){
        vector<string> vec = normal_fucking_split(cmd, " > ");
        execute(vec[0], pdir, true, vec[1], true);
    } else if(cmd.find(" >> ") != string::npos){
        vector<string> vec = normal_fucking_split(cmd, " >> ");

        execute(vec[0], pdir, true, vec[1]);
    } else {
        execute(cmd, pdir, false, "");
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
        process(cwd, cmd);
    }
    delete [] cwd;
}


int main(){
    open_shell();    
    return 0;
}
