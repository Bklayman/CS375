#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool stringToInt(std::string input){
  std::string::iterator it = input.begin();
  for( ; it != input.end() && std::isdigit(*it); ++it){

  }
  return(!input.empty() && it == input.end());
}

std::pair<int, int> getInputPair(std::string input){
  int spaceLocation = input.find(" ");
  if(spaceLocation == -1){
    std::cout << "Error: Input line " << input << " formatted incorrectly." << std::endl;
    exit(1);
  }
  std::string num1 = input.substr(0, spaceLocation);
  std::string num2 = input.substr(spaceLocation + 1, input.size() - spaceLocation - 1);
  if(num2.substr(0,1) == " "){
    num2 = num2.substr(1, num2.size() - 1);
  }
  int num1Int = -1;
  int num2Int = -1;
  if(stringToInt(num1)){
    num1Int = std::stoi(num1);
  } else {
    std::cout << "Error: Input number " << num1 << " is invalid." << std::endl;
    exit(1);
  }
  if(stringToInt(num2)){
    num2Int = std::stoi(num2);
  } else {
    std::cout << "Error: Input number " << num2 << " is invalid." << std::endl;
    exit(1);
  }
  return(std::pair<int, int>(num1Int, num2Int));
}

std::vector<std::pair<int, int>> sortInput(std::vector<std::string> input){
  std::vector<std::pair<int, int>> sortedInput;
  for(int i = 0; i < (int)input.size(); i++){
    sortedInput.push_back(getInputPair(input[i]));
  }
  for(int i = 0; i < (int)sortedInput.size() - 1; i++){
    for(int j = i + 1; j < (int)sortedInput.size(); j++){
      double iUnitCost = sortedInput[i].second / sortedInput[i].first;
      double jUnitCost = sortedInput[j].second / sortedInput[j].first;
      if(jUnitCost > iUnitCost){
        std::pair<int, int> jPair = sortedInput[j];
        sortedInput[j] = sortedInput[i];
        sortedInput[i] = jPair;
      }
    }
  }
  return sortedInput;
}

std::vector<std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>>> splitAndSort(std::vector<std::string> input){
  std::vector<std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>>> result;
  for(int i = 0; i < (int)input.size(); ){
    std::pair<int, int> pairDescript = getInputPair(input[i]);
    int curI = i;
    std::vector<std::string> stringList;
    for(i++ ; i < curI + pairDescript.first + 1; i++){
      stringList.push_back(input[i]);
    }
    std::vector<std::pair<int, int>> sortedStrings = sortInput(stringList);
    std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>> newPair = std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>>(pairDescript, sortedStrings);
    result.push_back(newPair);
  }
  return result;
}

void greedy1(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, long begin, int numInput, int weight){
  int profit = 0;
  for(int i = 0; i < numInput && weight > 0; i++){
    if(input[i].first <= weight){
      weight-= input[i].first;
      profit+= input[i].second;
    }
  }
  long end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  long timeTaken = end - begin;
  (*outputFile) << numInput << " " << profit << " " << timeTaken << std::endl;
}

int greedy1(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, long begin, int numInput, int weight, bool arbitraryVariable){
  int profit = 0;
  for(int i = 0; i < numInput && weight > 0; i++){
    if(input[i].first <= weight){
      weight-= input[i].first;
      profit+= input[i].second;
    }
  }
  return profit;
}

void greedy2(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, long begin, int numInput, int weight){
  int totalProfit = greedy1(input, outputFile, begin, numInput, weight, true);
  for(int i = 0; i < (int)input.size(); i++){
    if(totalProfit < input[i].second && input[i].first <= weight){
      totalProfit = input[i].second;
    }
  }
  long end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  long timeTaken = end - begin;
  (*outputFile) << numInput << " " << totalProfit << " " << timeTaken << std::endl;
}

int greedy2(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, long begin, int numInput, int weight, bool arbitraryVariable){
  int totalProfit = greedy1(input, outputFile, begin, numInput, weight, true);
  for(int i = 0; i < (int)input.size(); i++){
    if(totalProfit < input[i].second && input[i].first <= weight){
      totalProfit = input[i].second;
    }
  }
  return totalProfit;
}

int KWF(std::vector<std::pair<int, int>> input, int index, int weight, int curProfit, int curWeight){
  while(index < (int)input.size() && curWeight < weight){
    if(input[index].first + curWeight < weight){
      curWeight+= input[index].first;
      curProfit+= input[index].second;
    } else {
      curProfit+= input[index].second * (weight - curWeight) / input[index].first;
      curWeight = weight;
    }
    index++;
  }
  return curProfit;
}

int backtrackHelper(std::vector<std::pair<int, int>> input, int weight, int index, int maxProfit, int curProfit, int curWeight){
  if(index == (int)input.size() && curWeight <= weight){
    return curProfit;
  }
  if(curWeight > weight){
    return maxProfit;
  }
  int KWFProfit = KWF(input, index, weight, curProfit, curWeight);
  if(KWFProfit <= maxProfit || KWFProfit == curProfit){
    return maxProfit;
  }
  int possibleProfit = curProfit + input[index].second;
  int possibleWeight = curWeight + input[index].first;
  int leftTree = backtrackHelper(input, weight, index + 1, maxProfit, possibleProfit, possibleWeight);
  if(leftTree > maxProfit){
    maxProfit = leftTree;
  }
  int rightTree = backtrackHelper(input, weight, index + 1, maxProfit, curProfit, curWeight);
  if(rightTree > maxProfit){
    maxProfit = rightTree;
  }
  return maxProfit;
}

void backtrack(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, long begin, int numInput, int weight){
  int maxProfit = greedy2(input, outputFile, begin, numInput, weight, true);
  maxProfit = backtrackHelper(input, weight, 0, maxProfit, 0, 0);
  long end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  long timeTaken = end - begin;
  (*outputFile) << numInput << " " << maxProfit << " " << timeTaken << std::endl;
}

int main(int argc, char** argv){
  if(argc != 4){
    std::cout << "Usage: ./submission <input file name> <output file name> <0/1/2>" << std::endl;
    exit(1);
  }

  long begin = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  std::ifstream inputFile;
  inputFile.open(argv[1]);
  if(!inputFile.is_open()){
    std::cout << "Error: Input file " << argv[1] << " not found." << std::endl;
    exit(1);
  }
  std::vector<std::string> input;
  std::string inputLine;
  while(getline(inputFile, inputLine)){
    input.push_back(inputLine);
  }
  std::vector<std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>>> sortedInput = splitAndSort(input);

  std::ofstream outputFile;
  outputFile.open(argv[2]);
  if(!outputFile.is_open()){
    std::cout << "Error: Output file " << argv[2] << " could not open." << std::endl;
    exit(1);
  }

  int algorithmType = -1;
  if(stringToInt(argv[3])){
    algorithmType = std::stoi(argv[3]);
  } else {
    std::cout << "Error: " << argv[3] << " is not an integer." << std::endl;
    exit(1);
  }
  while(sortedInput.size() != 0){
    switch(algorithmType){
      case 0:
        greedy1(sortedInput[0].second, &outputFile, begin, sortedInput[0].first.first, sortedInput[0].first.second);
        break;
      case 1:
        greedy2(sortedInput[0].second, &outputFile, begin, sortedInput[0].first.first, sortedInput[0].first.second);
        break;
      case 2:
        backtrack(sortedInput[0].second, &outputFile, begin, sortedInput[0].first.first, sortedInput[0].first.second);
        break;
    }
    sortedInput.erase(sortedInput.begin());
  }
}
