#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <vector>

using namespace std;

#define BUFFSZCSRV 4096

int checkNeg(int var) {
  if (var == -1) {
    throw runtime_error(string("something went wrong: ") + strerror(errno));
  }
  return var;
}

struct WorkerArgs {
  void* server;
  int conn;
};

class ChatServer {
 private:
  int _sock = -1;
  map<int, string> _users;
  vector<int> _conns;
  pthread_t _listenerThread;
  vector<pthread_t> _workerThreads;
  atomic<bool> _running = true;
  mutex _mutex;

  void _sendTo(int conn, const string& msg) {
    auto res = send(conn, msg.data(), msg.size(), 0);
    if (res < 0) {
      cerr << "failed to send a message" << endl;
    }
  }

  void _broadcast(const string& msg) {
    {
      lock_guard<mutex> lock(_mutex);
      for (int i = 0; i < _conns.size(); i++) {
        _sendTo(_conns[i], msg);
      }
    }
  }

  void _processMessage(int conn, const string& msg) {
    if (msg[0] == '/') {
      if (msg.substr(0, 9) == "/setname " && msg.size() > 9) {
        {
          lock_guard<mutex> lock(_mutex);
          _users[conn] = msg.substr(9);
        }
        _broadcast("[Server] " + _users[conn] + " joined the chat");
      } else if (msg == "/list") {
        string list = "";
        for (auto& u : _users) {
          list += u.second + '\n';
        }
        _sendTo(conn, string("[Server] Active users:\n") + list);
      }
    } else {
      _broadcast(string('[' + _users[conn] + "] ") + msg);
    }
  }

  void _onDisconnect(int conn) {
    _broadcast("[Server] " + _users[conn] + " left the chat");
    {
      lock_guard<mutex> lock(_mutex);
      auto iter = find(_conns.begin(), _conns.end(), conn);
      if (iter != _conns.end()) _conns.erase(iter);

      auto iter2 = _users.find(conn);
      if (iter2 != _users.end()) _users.erase(iter2);
    }
    checkNeg(close(conn));
  }

  static void* _worker(void* arg) {
    WorkerArgs* args = (WorkerArgs*)arg;
    ChatServer* server = (ChatServer*)args->server;
    int conn = args->conn;
    delete args;
    char buff[BUFFSZCSRV];
    ssize_t n;
    while (server->_running && (n = recv(conn, buff, sizeof(buff), 0)) > 0) {
      string msg = string(buff, n);
      server->_processMessage(conn, msg);
    }
    server->_onDisconnect(conn);
    return NULL;
  }

  static void* _listener(void* arg) {
    ChatServer* server = (ChatServer*)arg;
    while (server->_running) {
      int conn = accept(server->_sock, NULL, NULL);
      if (conn < 0) {
        if (!server->_running) break;
        if (errno == EINTR) continue;
        cerr << "failed to accept a connection" << endl;
        continue;
      }
      pthread_t thread;
      WorkerArgs* args = new WorkerArgs{server, conn};
      pthread_create(&thread, NULL, _worker, args);
      {
        lock_guard<mutex> lock(server->_mutex);
        server->_conns.push_back(conn);
        server->_workerThreads.push_back(thread);
      }
    }
    return NULL;
  }

 public:
  ChatServer(int port, in_addr_t address, int maxconn) {
    _sock = checkNeg(socket(AF_INET, SOCK_STREAM, 0));
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(address);
    addr.sin_port = htons(port);
    checkNeg(bind(_sock, (struct sockaddr*)&addr, sizeof(addr)));
    checkNeg(listen(_sock, maxconn));
  }

  ~ChatServer() { stop(); }

  void run() { pthread_create(&_listenerThread, NULL, _listener, (void*)this); }
  void stop() {
    _broadcast("/exit");
    _running = false;
    if (_sock != -1) {
      checkNeg(shutdown(_sock, SHUT_RDWR));
      checkNeg(close(_sock));
      _sock = -1;
    }
    {
      lock_guard<mutex> lock(_mutex);
      for (int i = 0; i < _conns.size(); ++i) {
        checkNeg(shutdown(_conns[i], SHUT_RDWR));
      }
      _conns.clear();
    }
    if (_listenerThread > 0) {
      checkNeg(pthread_join(_listenerThread, NULL));
      _listenerThread = -1;
    }
    for (int i = 0; i < _workerThreads.size(); ++i) {
      checkNeg(pthread_join(_workerThreads[i], NULL));
    }
    _workerThreads.clear();

    cout << "Server stopped gracefully" << endl;
  }
};

static ChatServer* srv = nullptr;

void handler(int) {
  srv->stop();
  exit(0);
}

int main() {
  signal(SIGINT, handler);
  srv = new ChatServer(8000, INADDR_LOOPBACK, SOMAXCONN);
  srv->run();
  string cmd;
  while (true) {
    cin >> cmd;
    if (cmd == "/exit") break;
  }
  srv->stop();
  delete srv;

  return 0;
}