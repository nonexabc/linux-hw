#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace std;

#define BUFFSZ_HCLI 4096

int checkNeg(int var) {
  if (var < 0) {
    throw runtime_error(string("something went wrong: ") + strerror(errno));
  }
  return var;
}

void fetch(string& url, int outfd) {
  cout << "Resolving domain..." << endl;
  struct addrinfo hints = {0};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  struct addrinfo* res = nullptr;
  checkNeg(getaddrinfo(url.data(), "80", &hints, &res));

  cout << "Connecting..." << endl;
  int fd = -1;
  struct addrinfo* rp = res;
  for (; rp; rp = rp->ai_next) {
    fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (fd == -1) continue;
    if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) break;
    close(fd);
    fd = -1;
  }

  freeaddrinfo(res);

  checkNeg(fd);

  cout << "Sending request..." << endl;
  char buff[BUFFSZ_HCLI];
  string req = "GET / HTTP/1.1\r\nHost: " + url +
               "\r\nUser-Agent: fhttpcli/1.0\r\nConnection: close\r\n\r\n";
  checkNeg(send(fd, req.data(), req.size(), 0));

  cout << "Receiving response..." << endl;
  int len = 0;

  while ((len = recv(fd, buff, BUFFSZ_HCLI, 0)) > 0) {
    write(outfd, buff, len);
  }

  cout << "Done!" << endl;
}

int main(int argc, char* argv[]) {
  string host = "httpforever.com";
  string fname = "httpforever.html";
  if (argc > 1) {
    host = argv[1];

    if (argc == 2) {
      fname = host + ".html";
    } else {
      fname = argv[2];
    }
  }
  cout << "fetching " << host << ", saving to " << fname << endl;
  int fd = checkNeg(open(fname.data(), O_WRONLY | O_CREAT | O_TRUNC, 0744));

  fetch(host, fd);

  close(fd);

  return 0;
}