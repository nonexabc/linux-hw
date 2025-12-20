#include <iostream>
#include <unistd.h>

#define checkNeg(var) if(var == -1){ cerr << "something went wrong" << endl; exit(1);}

using namespace std;

void calculatePrime(int inPipe, int outPipe){
    while(true){
        int num;
        int prime;
        checkNeg(read(inPipe, &num, sizeof(int)));
        cout << "[Child] Calculating " << num << "-th prime number..." << endl;
        for(int i=2; ; ++i){
            bool flag = true;
            for(int j=2; j<=i/2; ++j){
                if(i%j == 0){
                    flag=false;
                    break;
                }
            }
            if(flag){
                --num;
                if(num == 0){
                    prime = i;
                    break;
                }
            }
        }
        cout << "[Child] Sending calculation result of prime(" << num << ')' << endl;
        checkNeg(write(outPipe, &prime, sizeof(int)));
    }
}


int main() {
    int toChild[2];
    int fromChild[2];
    checkNeg(pipe(toChild));
    checkNeg(pipe(fromChild));

    pid_t pid = fork();
    checkNeg(pid);

    if(pid == 0){
        close(toChild[1]);
        close(fromChild[0]);
        calculatePrime(toChild[0], fromChild[1]);
    }else{
        close(toChild[0]);
        close(fromChild[1]);
        while(true){
            string input;
            cout << "[Parent] Enter a number: ";
            cin >> input;
            if(input == "exit") break;
            int num = stoi(input);
            if(num < 1){
                cout << "[Parent] Invalid input. Try again." << endl;
                continue;
            }
            cout << "[Parent] Sending " << num << " to the child process..." << endl;
            checkNeg(write(toChild[1], &num, sizeof(int)));
            cout << "[Parent] Waiting for a response from the child process..." << endl;
            int prime;
            checkNeg(read(fromChild[0], &prime, sizeof(int)));
            cout << "[Parent] Received calculation result of prime(" << num << ") = " << prime << endl;


        }
    }

    return 0;
}