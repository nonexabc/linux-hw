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

void initialize(int argc, char** argv){
    if(argc < 2){
        cerr << "no file path specified" << endl;
        exit(0);
    }
    int file = open(argv[1], O_RDONLY);
    checkNeg(file, "couldn't open from the specified file");
    dup2(file, 0);
}

int main(int argc, char** argv){
    initialize(argc, argv);
    string input;
    cin >> input;
    reverse(input.begin(), input.end());
    cout << input << endl;
    
    return 0;
}
