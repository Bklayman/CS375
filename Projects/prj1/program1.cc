#include <iostream>

int main(int argc, char* argv[]){
  if(argc != 5){
    std::cout << "Usage: ./program1 -m <market-price-file> -p <price-list-file>" << std::endl;
    exit(1);
  }
}
