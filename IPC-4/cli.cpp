#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

#define BUFFSZCCLI 4096

bool isRunning = true;
string username;

int checkNeg(int var) {
  if (var < 0) {
    throw runtime_error(string("something went wrong: ") + strerror(errno));
  }
  return var;
}

void* receiverThread(void* arg) {
  int conn = (int)((int64_t)arg);
  char buff[BUFFSZCCLI];
  ssize_t n;
  while (isRunning && (n = recv(conn, buff, sizeof(buff), 0)) > 0) {
    cout << buff << endl;
    memset(buff, 0, sizeof(buff));
  }
  isRunning = false;
  cout << "connection closed" << endl;
  return NULL;
}

void client() {
  int sock = checkNeg(socket(AF_INET, SOCK_STREAM, 0));

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  addr.sin_port = htons(8080);

  checkNeg(connect(sock, (struct sockaddr*)&addr, sizeof(addr)));
  cout << "What's your name?" << endl;
  cin >> username;
  string req = "/setname " + username;
  send(sock, req.data(), req.size(), 0);
  cout << "Connected to the server" << endl;

  string message;
  ssize_t n;

  pthread_t thread;
  checkNeg(pthread_create(&thread, NULL, receiverThread, (void*)sock));
  while (isRunning) {
    cout << "> ";
    getline(cin, message);
    if (message == "/exit") break;
    checkNeg(send(sock, message.data(), message.size(), 0));
  }
  isRunning = false;
  close(sock);
  cout << "exiting" << endl;
  pthread_join(thread, NULL);
}

int main() {
  client();
  return 0;
}