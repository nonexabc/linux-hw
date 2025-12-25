#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

using namespace std;

#define BUFFSZCSRV 4096

vector<pthread_t> threads;
vector<int> conns;
map<int, string> users;

int checkNeg(int var) {
  if (var < 0) {
    throw runtime_error(string("something went wrong: ") + strerror(errno));
  }
  return var;
}

void broadcast(string& msg, int conn, bool system = false) {
  if (!system) msg = '[' + users[conn] + "] " + msg;
  for (int i = 0; i < conns.size(); ++i) {
    send(conns[i], msg.data(), msg.size(), 0);
  }
}

void* worker(void* arg) {
  int conn = (int)((int64_t)arg);
  char* buff = new char[BUFFSZCSRV];
  ssize_t n;
  string msg;
  while ((n = recv(conn, buff, BUFFSZCSRV, 0)) > 0) {
    msg = string(buff, n);
    if (msg.substr(0, 9) == "/setname " && msg.size() > 9) {
      users[conn] = msg.substr(9);
      msg = "[Server] " + users[conn] + " joined the chat";
      broadcast(msg, conn, true);
      continue;
    } else if (msg == "/list") {
      string list = "";
      for (auto& u : users) {
        list += u.second + '\n';
      }
      send(conn, list.data(), list.size(), 0);
      continue;
    }
    broadcast(msg, conn);
    memset(buff, 0, BUFFSZCSRV);
  }
  cout << "connection closed" << endl;
  delete[] buff;
  close(conn);
  return NULL;
}

void serve() {
  int sock = checkNeg(socket(AF_INET, SOCK_STREAM, 0));

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  addr.sin_port = htons(8080);

  checkNeg(bind(sock, (struct sockaddr*)&addr, sizeof(addr)));
  checkNeg(listen(sock, SOMAXCONN));

  while (true) {
    int conn = checkNeg(accept(sock, NULL, NULL));
    cout << "new connection" << endl;
    conns.push_back(conn);
    pthread_t thread;
    checkNeg(pthread_create(&thread, NULL, worker, (void*)conn));
    threads.push_back(thread);
  }

  for (int i = 0; i < conns.size(); ++i) {
    pthread_join(threads[i], NULL);
    close(conns[i]);
  }

  close(sock);
}

int main() {
  serve();
  return 0;
}