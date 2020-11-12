#include <chrono>
#include <fstream>
#include <iostream>
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
  std::vector<int> placements;
  for(int i = 0; i < (int)input.size(); i++){
    sortedInput.push_back(getInputPair(input[i]));
    placements.push_back(i);
  }
  for(int i = 0; i < (int)sortedInput.size() - 1; i++){
    for(int j = i + 1; j < (int)sortedInput.size(); j++){
      double iUnitCost = sortedInput[i].second / sortedInput[i].first;
      double jUnitCost = sortedInput[j].second / sortedInput[j].first;
      if(jUnitCost > iUnitCost){
        std::pair<int, int> jPair = sortedInput[j];
        sortedInput[j] = sortedInput[i];
        sortedInput[i] = jPair;
        int curJ = placements[j];
        placements[j] = placements[i];
        placements[i] = curJ;
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

void computeDynamic(std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>> data, std::ofstream* outputFile, long begin){
  int capacity = data.first.second;
  if(capacity == 0 || data.second.size() == 0){
    long end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long timeTaken = end-begin;
    (*outputFile) << "Dynamic Programming: " << data.second.size() << " 0 " << timeTaken << std::endl;
    return;
  }
  int dynamicStorage[2][capacity + 1];
  std::vector<std::pair<int, int>> items = data.second;
  for(int i = 0; i < data.first.first; i++){
    if(i != 0){
      for(int i = 0; i < capacity + 1; i++){
        dynamicStorage[0][i] = dynamicStorage[1][i];
        dynamicStorage[1][i] = 0;
      }
    }
    for(int j = 0; j < capacity + 1; j++){
      if(items[i].first > capacity){
        dynamicStorage[1][j] = dynamicStorage[0][j];
      } else {
        int notInclude = dynamicStorage[0][j];
        int include = 0;
        if(j >= items[i].first){
          include = dynamicStorage[0][j - items[i].first] + items[i].second;
        }
        if(notInclude > include){
          dynamicStorage[1][j] = notInclude;
        } else {
          dynamicStorage[1][j] = include;
        }
      }
    }
  }
  long end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  long timeTaken = end-begin;
  (*outputFile) << "Dynamic Programming: " << items.size() << " " << dynamicStorage[1][capacity] << " " << timeTaken << std::endl;
}

int main(int argc, char** argv){
  if(argc != 4){
    std::cout << "Usage: ./program4 <input file name> <output file name> 3" << std::endl;
    exit(1);
  }

  if(strncmp(argv[3], "3", 2) != 0){
    std::cout << "Error: This program only computes method 3." << std::endl;
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

  for(int i = 0; i < sortedInput.size(); i++){
    std::pair<std::pair<int, int>, std::vector<std::pair<int, int>>> curProblem = sortedInput[i];
    computeDynamic(curProblem, &outputFile, begin);
  }
}
