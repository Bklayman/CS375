#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]){
  if(argc != 5){
    std::cout << "Usage: ./program1 -m <market-price-file> -p <price-list-file>" << std::endl;
    exit(1);
  }
  //Creates the files for market-price-file and price-list-file
  std::fstream marketFile, listFile;
  //Opens the market-price-file
  marketFile.open(argv[2]);
  if(!marketFile.is_open()){
    std::cout << "Error: " << argv[2] << " does not exist." << std::endl;
    exit(1);
  }
  //Reads market-price-file and creates a dictionary for all the names (key) and prices (value)
  std::string numMarketPricesStr;
  if(!getline(marketFile, numMarketPricesStr)){
    std::cout << "Error: " << argv[2] << " does not have a correct first line." << std::endl;
  }
  int numMarketPrices;
  try{
    numMarketPrices = std::stoi(numMarketPricesStr);
  } catch(...){
    std::cout << "Error: " << argv[2] << " does not have a correct first line." << std::endl;
    exit(1);
  }
  for(int i = 0; i < numMarketPrices; i++){
    std::string marketPrice;
    if(!getLine(marketFile, marketPrice)){
      std::cout << "Error: " << argv[2] << "'s line " << i << " does not exist." << std::endl;
      exit(1);
    }
    
  }
  //Opens the price-list-file
  listFile.open(argv[4]);
  if(!listFile.is_open()){
    std::cout << "Error: " << argv[4] << " does not exist." << std::endl;
    exit(1);
  }
}
