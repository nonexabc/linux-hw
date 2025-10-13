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

void apnd(char* fp){
    int file = open(fp, O_RDWR);
    checkNeg(file, "couldn't open the specified file");
    int copy = dup(1);
    checkNeg(copy, "couldn't copy stdout");
    checkNeg(dup2(file, 1), "couldn't override stdout");
    cout << "first line\nsecond line" << endl;
    checkNeg(dup2(copy, 1), "couldn't restore stdout");
}

int main(int argc, char** argv){
    if(argc < 2){
        cerr << "no file path specified" << endl;
        exit(0);
    }
    apnd(argv[1]);
    cout << "appended successfully" << endl;
    return 0;
}
