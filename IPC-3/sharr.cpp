#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

// template <typename T>
class shared_array {
 private:
  std::string _name;
  std::string _fname;
  size_t _size = 0;
  int _fd = -1;
  int* _ptr = nullptr;
  // const long page_size = sysconf(_SC_PAGESIZE);

  void _set_fname() {
    _fname =
        _name + '-' +
        std::to_string(
            _size);  // технически, условием не запрещено так делать массивы
                     // разными, если у них разные размеры, но одно название)
    if (_fname.length() > 253) {
      std::cerr << "error: name is too long" << std::endl;
      exit(1);
    }
    if (_fname.find("/") != _fname.npos) {
      std::cerr << "error: name cannot contain slashes" << std::endl;
      exit(1);
    }
    _fname = _fname.insert(0, "/");
  }

  void _load_fd() {
    _fd = shm_open(_fname.data(), O_CREAT | O_RDWR | O_EXCL, 0777);
    if (_fd >= 0) {
      chk(ftruncate(_fd, _size * sizeof(int)));
    } else if (errno == EEXIST) {
      _fd = chk(shm_open(_fname.data(), O_RDWR, 0777));
    } else {
      chk(_fd);  // throw
    }
  }

  int chk(int var) const {
    if (var == -1) {
      throw std::runtime_error(std::string("error: ") + strerror(errno));
    }
    return var;
  }

 public:
  shared_array(std::string name, size_t size)
      : _name(std::move(name)), _size(size) {
    _set_fname();
    _load_fd();
    _ptr = (int*)mmap(NULL, _size * sizeof(int), PROT_READ | PROT_WRITE,
                      MAP_SHARED, _fd, 0);
    if (_ptr == MAP_FAILED) {
      std::cerr << "error: " << strerror(errno) << std::endl;
      exit(errno);
    }
  }

  int& operator[](size_t index) {
    if (index >= _size) throw std::out_of_range("index out of range");
    return _ptr[index];
  }

  ~shared_array() {
    if (_ptr != nullptr) {
      munmap(_ptr, _size * sizeof(int));
    }
    if (_fd != -1) {
      close(_fd);
    }
    chk(shm_unlink(_fname.data()));
  }
};
