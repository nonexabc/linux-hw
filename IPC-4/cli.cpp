#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <vector>

using namespace std;

#define BUFFSZCCLI 4096

int checkNeg(int var) {
  if (var == -1) {
    throw runtime_error(string("something went wrong: ") + strerror(errno));
  }
  return var;
}

class ChatClient {
 private:
  int _sock = -1;
  atomic<bool> _running = true;
  string _username;
  pthread_t _receiverThread;
  void (*_onServerStop)() = nullptr;
  bool _serverStopped = false;

  void _sendToServer(const string& msg) {
    checkNeg(send(_sock, msg.data(), msg.size(), 0));
  }

  void _onDisconnect() { stop(false); }

  void _processMessage(const string& msg) {
    if (msg[0] == '/') {
      if (msg == "/exit") {
        _running = false;
        _serverStopped = true;
        return;
      }
      cout << "Command: " << msg << endl;
    } else {
      cout << msg << endl;
    }
  }

  static void* _receiver(void* arg) {
    ChatClient* client = (ChatClient*)arg;
    char buff[BUFFSZCCLI];
    ssize_t n;
    string msg;
    while (client->_running &&
           (n = recv(client->_sock, buff, sizeof(buff), 0)) > 0) {
      msg = string(buff, n);
      client->_processMessage(msg);
    }
    client->_onDisconnect();
    return NULL;
  }

 public:
  ChatClient(int port, in_addr_t address) {
    _sock = checkNeg(socket(AF_INET, SOCK_STREAM, 0));
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(address);
    addr.sin_port = htons(port);
    checkNeg(connect(_sock, (struct sockaddr*)&addr, sizeof(addr)));
  }

  ~ChatClient() { stop(); }

  void run() { pthread_create(&_receiverThread, NULL, _receiver, (void*)this); }

  void stop(bool stopReceiver = true) {
    _running = false;
    if (_serverStopped && _onServerStop) {
      cout << "The server stopped" << endl;
      _onServerStop();
    }
    if (_sock != -1) {
      checkNeg(shutdown(_sock, SHUT_RDWR));
      checkNeg(close(_sock));
      _sock = -1;
    }
    if (stopReceiver && _receiverThread > 0) {
      checkNeg(pthread_join(_receiverThread, NULL));
      _receiverThread = -1;
    }
    cout << "Client stopped gracefully" << endl;
  }

  void onServerStop(void (*func)()) { _onServerStop = func; }

  void setName(const string& name) {
    _username = name;
    _sendToServer("/setname " + name);
  }

  void sendMessage(const string& msg) { _sendToServer(msg); }

  bool isRunning() const { return _running; }
};

static ChatClient* cli = nullptr;

void handler(int) {
  cli->stop();
  exit(0);
}

void killStdin() {
  close(STDIN_FILENO);
  kill(getpid(), SIGSTOP);
}

int main() {
  cli = new ChatClient(8000, INADDR_LOOPBACK);
  signal(SIGINT, handler);
  cli->onServerStop(killStdin);
  cli->run();
  string msg;
  cout << "What's your name?" << endl;
  getline(cin, msg);
  cli->setName(msg);
  while (cli->isRunning()) {
    getline(cin, msg);
    cli->sendMessage(msg);
    if (msg == "/exit") {
      cli->stop();
      break;
    }
  }
  return 0;
}