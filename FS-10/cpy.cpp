#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <algorithm>

#define BUFFSZ 4096 // 1 block
#define checkNeg(var, msg) if(var == -1){ cerr << msg << endl; exit(errno);}

using namespace std;

void cpy(char* srcfp, char* destfp){
    int src = open(srcfp, O_RDONLY);
    checkNeg(src, "couldn't open the source file");
    int dest = open(destfp, O_WRONLY | O_CREAT | O_TRUNC, 0744);
    checkNeg(dest, "couldn't open/create the destination file");
    
    char* buff = new char[BUFFSZ];
    off_t pos = 0;
    while(true){
        auto dataStart = lseek(src, pos, SEEK_DATA);
        if (dataStart == -1) {
            if (errno == ENXIO) break;
            checkNeg(-1, "seek_data failed");
        }
        if(dataStart > pos){
            auto skip = dataStart - pos;
            lseek(dest, skip, SEEK_CUR);
            pos += skip;
            continue;
        }
        auto holeStart = lseek(src, pos, SEEK_HOLE);
        if (dataStart == -1) {
            if (errno == ENXIO) holeStart = pos + BUFFSZ;
            else checkNeg(-1, "seek_hole failed");
        }
        lseek(src, pos, SEEK_SET);
        auto toWrite = min((long) BUFFSZ, holeStart - pos);
        if(toWrite <= 0) break;
        int bytes = read(src, buff, toWrite);
        if(bytes == 0) break;
        checkNeg(bytes, "couldn't read from the source file");
        checkNeg(write(dest, buff, bytes), "couldn't write to the destination file");
        pos += bytes;
        lseek(src, pos, SEEK_SET);
    }

    checkNeg(close(src), "couldn't close the source file");
    checkNeg(close(dest), "couldn't close the destination file");
    delete [] buff;
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
