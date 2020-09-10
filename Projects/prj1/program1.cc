#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <time.h>

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

std::list<std::map<std::string, int>> getPriceLists(char* priceListFileName){
  std::list<std::map<std::string, int>> priceLists;
  std::fstream priceListFile;
  priceListFile.open(priceListFileName);
  if(!priceListFile.is_open()){
    std::cout << "Error: " << priceListFileName << " does not exist." << std::endl;
    exit(1);
  }
  std::string listParameters;
  while(std::getline(priceListFile, listParameters)){
    int spaceIndex = listParameters.find(" ");
    std::string numCardsStr = listParameters.substr(0, spaceIndex);
    std::string numMoneyStr = listParameters.substr(spaceIndex + 1, listParameters.length() - spaceIndex - 1);
    int numCards = stringToInt(numCardsStr);
    int numMoney = stringToInt(numMoneyStr);
    std::map<std::string, int> curPrices;
    curPrices.insert(std::pair<std::string, int>("MoneyAmount", numMoney));
    for(int i = 0; i < numCards; i++){
      std::string priceListLine;
      if(!std::getline(priceListFile, priceListLine)){
        std::cout << "Error: " << priceListFileName << "does not have enough lines per the parameters set." << std::endl;
        exit(1);
      }
      spaceIndex = priceListLine.find(" ");
      std::string cardName = priceListLine.substr(0, spaceIndex);
      std::string cardPriceStr = priceListLine.substr(spaceIndex + 1, priceListLine.length() - spaceIndex - 1);
      int cardPrice = stringToInt(cardPriceStr);
      curPrices.insert(std::pair<std::string, int>(cardName, cardPrice));
    }
    priceLists.push_back(curPrices);
  }
  return priceLists;
}

std::pair<std::list<std::string>, int> profitHelper(std::map<std::string, int> marketPrices, std::map<std::string, int>* priceList, std::map<std::string, int>::iterator it, int curProfit, std::list<std::string> curCards, int moneyLeft){
  if(it == priceList->end()){
    return std::pair<std::list<std::string>, int>(curCards, curProfit);
  }
  if(it->first == "MoneyAmount"){
    return profitHelper(marketPrices, priceList, ++it, curProfit, curCards, moneyLeft);
  }
  std::string cardName = it->first;
  int cardPrice = it->second;
  ++it; //Seg Fault
  std::pair<std::list<std::string>, int> noCardAdd = profitHelper(marketPrices, priceList, it, curProfit, curCards, moneyLeft);
  if(moneyLeft < cardPrice){
    return noCardAdd;
  }
  curProfit+= marketPrices[cardName] - 2 * cardPrice;
  curCards.push_back(cardName);
  moneyLeft-= cardPrice;
  std::pair<std::list<std::string>, int> cardAdd = profitHelper(marketPrices, priceList, it, curProfit, curCards, moneyLeft);
  if(noCardAdd.second > cardAdd.second){
    return noCardAdd;
  }
  return cardAdd;
}

std::pair<std::list<std::string>, int> getMaximumProfit(std::map<std::string, int> marketPrices, std::map<std::string, int>* priceList){
  std::list<std::string> winningCards;
  return profitHelper(marketPrices, priceList, priceList->begin(), (*priceList)["MoneyAmount"], winningCards, (*priceList)["MoneyAmount"]);
}

void writeResultsToOutput(std::list<std::pair<std::list<std::string>, int>> results, std::list<std::map<std::string, int>> priceLists, std::list<time_t> timeTaken){
  std::ofstream outputFile;
  outputFile.open("output.txt");
  std::list<std::map<std::string, int>>::iterator itPriceLists = priceLists.begin();
  std::list<time_t>::iterator itTime = timeTaken.begin();
  for(std::list<std::pair<std::list<std::string>, int>>::iterator itResults = results.begin(); itResults != results.end(); ++itResults){
    outputFile << (*itPriceLists).size() - 1 << " " << (*itResults).second << " " << (*itResults).first.size() << " " << *itTime << std::endl;
    for(std::list<std::string>::iterator itCardResults = (*itResults).first.begin(); itCardResults != (*itResults).first.end(); ++itCardResults){
      outputFile << *itCardResults << std::endl;
    }
    ++itTime;
    ++itPriceLists;
  }
}

int main(int argc, char* argv[]){
  time_t timeStart = time (NULL);
  if(argc != 5){
    std::cout << "Usage: ./program1 -m <market-price-file> -p <price-list-file>" << std::endl;
    exit(1);
  }

  char* marketName;
  char* priceName;
  if(std::strncmp(argv[1], "-m", 3) == 0){
    marketName = argv[2];
  } else if (std::strncmp(argv[3], "-m", 3) == 0) {
    marketName = argv[4];
  } else {
    std::cout << "Error: no -m found in the command line input" << std::endl;
    exit(1);
  }

  if(std::strncmp(argv[1], "-p", 3) == 0){
    priceName = argv[2];
  } else if (std::strncmp(argv[3], "-p", 3) == 0) {
    priceName = argv[4];
  } else {
    std::cout << "Error: no -p found in the command line input" << std::endl;
    exit(1);
  }

  std::map<std::string, int> marketPricesMap = getMarketPricesMap(marketName);
  std::list<std::map<std::string, int>> priceLists = getPriceLists(priceName);
  std::list<std::pair<std::list<std::string>, int>> results;
  std::list<time_t> timeTaken;

  for(std::list<std::map<std::string, int>>::iterator it = priceLists.begin(); it != priceLists.end(); ++it){
    results.push_back(getMaximumProfit(marketPricesMap, &*it));
    time_t curTimeTaken = time(NULL);
    timeTaken.push_back(curTimeTaken - timeStart);
  }

  writeResultsToOutput(results, priceLists, timeTaken);
}
