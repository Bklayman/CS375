#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

void printFinalResults(std::list<std::pair<std::list<std::string>, int>> results){
  for(std::list<std::pair<std::list<std::string>, int>>::iterator it = results.begin(); it != results.end(); ++it){
    std::cout << "Profit: " << (*it).second << std::endl;
    std::list<std::string> cardNames = (*it).first;
    for(std::list<std::string>::iterator it = cardNames.begin(); it != cardNames.end(); ++it){
      std::cout << *it << std::endl;
    }
  }
  std::cout << std::endl;
}

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

std::pair<std::list<std::string>, int> profitHelper(std::map<std::string, int> marketPrices, std::map<std::string, int> priceList, std::map<std::string, int>::iterator it, int curProfit, std::list<std::string> curCards, int moneyLeft){
  if(it == priceList.end()){
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
  curProfit+= marketPrices[cardName] - cardPrice;
  curCards.push_back(cardName);
  moneyLeft-= cardPrice;
  std::pair<std::list<std::string>, int> cardAdd = profitHelper(marketPrices, priceList, it, curProfit, curCards, moneyLeft);
  if(noCardAdd.second > cardAdd.second){
    return noCardAdd;
  }
  return cardAdd;
}

std::pair<std::list<std::string>, int> getMaximumProfit(std::map<std::string, int> marketPrices, std::map<std::string, int> priceList){
  std::list<std::string> winningCards;
  return profitHelper(marketPrices, priceList, priceList.begin(), 0, winningCards, priceList["MoneyAmount"]);
}

int main(int argc, char* argv[]){
  if(argc != 5){
    std::cout << "Usage: ./program1 -m <market-price-file> -p <price-list-file>" << std::endl;
    exit(1);
  }

  std::map<std::string, int> marketPricesMap = getMarketPricesMap(argv[2]);
  std::list<std::map<std::string, int>> priceLists = getPriceLists(argv[4]);
  std::list<std::pair<std::list<std::string>, int>> results;
  for(std::list<std::map<std::string, int>>::iterator it = priceLists.begin(); it != priceLists.end(); ++it){
    std::cout << "Starting" << std::endl;
    results.push_back(getMaximumProfit(marketPricesMap, *it));
  }
  printFinalResults(results);
}
