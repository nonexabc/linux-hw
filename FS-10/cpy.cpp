#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <algorithm>

#define BUFFSZ 1024
#define checkNeg(var, msg) if(var == -1){ cerr << msg << endl; exit(errno);}

using namespace std;

void cpy(char* srcfp, char* destfp){
    int src = open(srcfp, O_RDONLY);
    checkNeg(src, "couldn't open the source file");
    int dest = open(srcfp, O_WRONLY);
    checkNeg(dest, "couldn't open/create the destination file");
}

int main(int argc, char** argv){
    if(argc != 3){
        cerr << argv[0] << " requires 2 args: source and destination paths" << endl;
        exit(0);
    }
    cpy(argv[1], argv[2]);
    cout << "copied successfully" << endl;
    return 0;
}
