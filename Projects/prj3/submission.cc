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
  for(int i = 0; i < input.size(); i++){
    sortedInput.push_back(getInputPair(input[i]));
  }
  for(int i = 0; i < sortedInput.size() - 1; i++){
    for(int j = i + 1; j < sortedInput.size(); j++){
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
  for(int i = 0; i < input.size(); i++){
    std::pair<int, int> pairDescript = getInputPair(input[0]);
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
  for(int i = 1; i < numInput + 1 && weight > 0; i++){
    if(input[i].first <= weight){
      weight-= input[i].first;
      profit+= input[i].second;
    }
  }
  long end = std::chrono::system_clock::now().time_since_epoch().count();
  long timeTaken = end - begin;
  (*outputFile) << numInput << " " << profit << " " << timeTaken << std::endl;
}

void greedy2(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, long begin, int numInput, int weight){
  //TODO
}

void backtrack(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, long begin, int numInput, int weight){
  //TODO
}

int main(int argc, char** argv){
  if(argc != 4){
    std::cout << "Usage: ./submission <input file name> <output file name> <0/1/2>" << std::endl;
    exit(1);
  }

  long begin = std::chrono::system_clock::now().time_since_epoch().count();

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
  //Sorting everything. Need to split into different vectors.
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
