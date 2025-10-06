#include <iostream>

void reversePrint(int argc, char* args[]){
  std::cout << args[argc-1] << " ";
  if(argc>1){
    reversePrint(argc-1, args);
  }else{
    std::cout << std::endl;
  }
}

int main(int argc, char* args[]){
  reversePrint(argc, args);
}
