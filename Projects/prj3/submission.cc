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

std::vector<std::pair<int, int>> sortInput(std::vector<std::string> input){
  std::vector<std::pair<int, int>> sortedInput;
  for(int i = 0; i < input.size(); i++){
    int spaceLocation = input[i].find(" ");
    if(spaceLocation == -1){
      std::cout << "Error: Input line " << input[i] << " formatted incorrectly." << std::endl;
      exit(1);
    }
    std::string num1 = input[i].substr(0, spaceLocation);
    std::string num2 = input[i].substr(spaceLocation + 1, input[i].size() - spaceLocation - 1);
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
    sortedInput.push_back(std::pair<int, int>(num1Int, num2Int));
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

void greedy1(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, std::chrono::time_point<std::chrono::system_clock> begin){
  //TODO
}

void greedy2(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, std::chrono::time_point<std::chrono::system_clock> begin){
  //TODO
}

void backtrack(std::vector<std::pair<int, int>> input, std::ofstream *outputFile, std::chrono::time_point<std::chrono::system_clock> begin){
  //TODO
}

int main(int argc, char** argv){
  if(argc != 4){
    std::cout << "Usage: ./submission <input file name> <output file name> <0/1/2>" << std::endl;
    exit(1);
  }

  std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

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
  std::vector<std::pair<int, int>> sortedInput = sortInput(input);

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
  switch(algorithmType){
    case 0:
      greedy1(sortedInput, &outputFile, begin);
      break;
    case 1:
      greedy2(sortedInput, &outputFile, begin);
      break;
    case 2:
      backtrack(sortedInput, &outputFile, begin);
      break;
  }
}
