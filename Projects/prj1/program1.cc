#include <fstream>
#include <iostream>
#include <map>
#include <string>

int stringToInt(std::string originalString){
  int result;
  try{
    result = std::stoi(originalString);
  } catch(...){
    std::cout << "Error: " << originalString << " is not an integer." << std::endl;
    exit(1);
  }
  return result;
}

std::map<std::string, int> getMarketPricesMap(char* marketFileName){
  std::fstream marketFile;
  //Opens the market-price-file
  marketFile.open(marketFileName);
  if(!marketFile.is_open()){
    std::cout << "Error: " << marketFileName << " does not exist." << std::endl;
    exit(1);
  }
  std::string numMarketPricesStr;
  if(!getline(marketFile, numMarketPricesStr)){
    std::cout << "Error: " << marketFileName << " does not have a correct first line." << std::endl;
  }
  int numMarketPrices;
  numMarketPrices = stringToInt(numMarketPricesStr);
  std::map<std::string, int> marketPriceMap;
  for(int i = 0; i < numMarketPrices; i++){
    std::string marketPriceLine;
    if(!std::getline(marketFile, marketPriceLine)){
      std::cout << "Error: " << marketFileName << "'s line " << i << " does not exist." << std::endl;
      exit(1);
    }
    int spaceIndex = marketPriceLine.find(" ");
    if(spaceIndex == -1){
      std::cout << "Error: Line " << i + 1 << " in " << marketFileName << " is formatted incorrectly." << std::endl;
      exit(1);
    }
    std::string cardName = marketPriceLine.substr(0, spaceIndex);
    std::string cardPriceStr = marketPriceLine.substr(spaceIndex + 1, marketPriceLine.length() - spaceIndex - 1);
    int cardPrice = stringToInt(cardPriceStr);
    marketPriceMap.insert(std::pair<std::string, int>(cardName, cardPrice));
  }
  return marketPriceMap;
}

int main(int argc, char* argv[]){
  if(argc != 5){
    std::cout << "Usage: ./program1 -m <market-price-file> -p <price-list-file>" << std::endl;
    exit(1);
  }

  std::fstream listFile;
  std::map<std::string, int> marketPricesMap = getMarketPricesMap(argv[2]);
  
  listFile.open(argv[4]);
  if(!listFile.is_open()){
    std::cout << "Error: " << argv[4] << " does not exist." << std::endl;
    exit(1);
  }
}
