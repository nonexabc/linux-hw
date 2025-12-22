#include <signal.h>

#include <iostream>

#include "sharr.cpp"

using namespace std;

bool running = true;

void stop(int _) { running = false; }

int main() {
  shared_array arr("testarr", 10);
  struct sigaction act;
  act.sa_handler = &stop;
  sigaction(SIGINT, &act, NULL);
  while (running) {
    cout << "input index and number" << endl;
    int index, num;
    cin >> index >> num;
    try {
      arr[index] = num;
    } catch (...) {
      cout << "index out of range" << endl;
    }
    cout << "{ ";
    for (int i = 0; i < 10; i++) {
      cout << arr[i] << " ";
    }
    cout << "}" << endl;
  }
  return 0;
}