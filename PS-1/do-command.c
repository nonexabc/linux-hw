#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void do_command(char* argv){
    time_t start = time(NULL);
    pid_t pid = fork();
    if(pid == -1){
        printf("failed to create a new process\n");
        exit(1);
    }
    if(pid == 0){ //child
        system(argv);
    }
    if(pid > 0){
        int childStatus = -1;
        pid_t waiting = waitpid(pid, &childStatus, 0);
        printf("%s completed with %d exit code and took %ld seconds\n", argv, childStatus, time(NULL)-start); 
    }
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("no command provided, nothing to do\n");
        exit(1);
    }
    size_t i=1;
    size_t len=0;
    while(1){
        auto curr = argv[i];
        if(curr == nullptr){
         break;
        }
        ++i;
        len += strlen(curr)+1;
    }
    char* cmd = (char*)malloc(len);
    size_t offset = 0;
    for(size_t i1=1; i1<i; ++i1){
        auto curr = argv[i1];
        auto l = strlen(curr)+1;
        memcpy(cmd+offset, curr, l);
        cmd[offset+l-1] = ' ';
        offset += l;
    }
    cmd[len-1] = '\0';
    
    do_command(cmd);
    
    free(cmd);
    
    return 0;
}
