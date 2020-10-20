#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

void printHandles(std::map<int, int> *indexHandler, std::ofstream *outputFile, int maxHeapSize){
  int usedNums[maxHeapSize];
  for(int i = 0; i < maxHeapSize; i++){
    usedNums[i] = -1;
  }
  for(std::map<int, int>::iterator it = indexHandler->begin(); it != indexHandler->end(); ++it){
    usedNums[it->first - 1] = it->second;
  }
  for(int i = 0; i < maxHeapSize; i++){
    if(usedNums[i] == -1){
      *outputFile << "There is no Contestant <" << i + 1 << "> in the extended heap: handle[<" << i + 1 << ">] = -1." << std::endl;
    } else {
      *outputFile << "Contestant <" << i + 1 << "> stored in extended heap location <" << usedNums[i] + 1 << ">." << std::endl;
    }
  }
}

void printExtendedHeap(std::vector<int> *pointsMinHeap, std::map<int, int> *indexHandler, std::ofstream *outputFile){
  int places[pointsMinHeap->size()];
  for(std::map<int, int>::iterator it = indexHandler->begin(); it != indexHandler->end(); ++it){
    places[it->second] = it->first;
  }
  for(int i = 0; i < (int) pointsMinHeap->size(); i++){
    *outputFile << "Contestant <" << places[i] << "> in extended heap location <" << i + 1 << "> with score <" << (*pointsMinHeap)[i] << ">." << std::endl;
  }
}

void swapNodes(std::vector<int> *pointsMinHeap, std::map<int, int> *indexHandler, int index1, int index2){ //Duplicates an item in indexHandler
  if(index1 >= (int) indexHandler->size() || index2 >= (int) indexHandler->size()){
    std::cout << "Error: swap has bad indexes" << std::endl;
    exit(1);
  }
  int location1 = -1; //Where in indexHandler index1 is found
  int location2 = -1;
  for(std::map<int, int>::iterator it = indexHandler->begin(); it != indexHandler->end(); ++it){
    if(it->second == index1){
      location1 = it->first;
    }
    if(it->second == index2){
      location2 = it->first;
    }
  }
  int points1 = (*pointsMinHeap)[index1];
  (*pointsMinHeap)[index1] = (*pointsMinHeap)[index2];
  (*indexHandler)[location1] = index2;
  (*indexHandler)[location2] = index1;
  (*pointsMinHeap)[index2] = points1;
}

void minHeapify(std::vector<int> *pointsMinHeap, std::map<int, int> *indexHandler){
  for(int i = (int) pointsMinHeap->size() / 2 - 1; i >= 0 && (int) pointsMinHeap->size() != 1; i--){
    int curI = i;
    bool finished = false;
    while(!finished){
      if(i == 0){
        finished = true;
      }
      int minFound = 0;
      int minPoints = (*pointsMinHeap)[i];
      if((*pointsMinHeap)[(int)i] > (*pointsMinHeap)[(int)2.0*i + 1]){
        minFound = 1;
        minPoints = (*pointsMinHeap)[(int)2.0*i + 1];
      }
      if((int) pointsMinHeap->size() > (int)2.0*i + 2 && minPoints > (*pointsMinHeap)[(int)2.0*i + 2]){
        minFound = 2;
        minPoints = (*pointsMinHeap)[(int)2.0*i + 2];
      }
      switch(minFound){
        case 1:
          swapNodes(pointsMinHeap, indexHandler, (int)i, (int)2.0*i + 1);
          break;
        case 2:
          swapNodes(pointsMinHeap, indexHandler, (int)i, (int)2.0*i+2);
          break;
      }
      i /= 2;
    }
    i = curI;
  }
}

std::string eliminateWeakest(std::vector<int> *pointsMinHeap, std::map<int, int> *indexHandler){
  std::string result = "";
  if((int)pointsMinHeap->size() == 0){
    result += "No contestant can be eliminated since the extended heap is empty.";
  } else {
    int weakIndex = -1;
    for(std::map<int, int>::iterator it = indexHandler->begin(); it != indexHandler->end(); ++it){
      if(it->second == 0){
        weakIndex = it->first;
        break;
      }
    }
    int weakPoints = (*pointsMinHeap)[0];
    result += "Contestant <" + std::to_string(weakIndex) + "> with current lowest score <" + std::to_string(weakPoints) + "> eliminated.";
    pointsMinHeap->erase(pointsMinHeap->begin(), pointsMinHeap->begin() + 1);
    indexHandler->erase(weakIndex);
    int nextCopy = (*pointsMinHeap)[(int) pointsMinHeap->size() - 1];
    for(int i = 0; i < (int) pointsMinHeap->size(); i++){
      int curCopy = nextCopy;
      nextCopy = (*pointsMinHeap)[i];
      (*pointsMinHeap)[i] = curCopy;
    }
    for(std::map<int, int>::iterator it = indexHandler->begin(); it != indexHandler->end(); ++it){
      if(it->second == (int) pointsMinHeap->size()){
        it->second = 0;
        break;
      }
    }
    minHeapify(pointsMinHeap, indexHandler); //Adds to indexHandler for some reason
  }
  return result;
}

void crownWinner(std::vector<int> *pointsMinHeap, std::map<int, int> *indexHandler, std::ofstream *outputFile){
  while((int) pointsMinHeap->size() > 1){
    eliminateWeakest(pointsMinHeap, indexHandler);
  }
  std::map<int, int>::iterator it = indexHandler->begin();
  *outputFile << "Contestant <" << it->first << "> wins with score <" << (*pointsMinHeap)[it->second] << ">!" << std::endl;
}

void insertContestant(std::vector<int> *pointsMinHeap, std::map<int, int> *indexHandler, int index, int points){
  pointsMinHeap->push_back(points);
  indexHandler->insert(std::pair<int, int>(index, (int) pointsMinHeap->size() - 1));
  minHeapify(pointsMinHeap, indexHandler);
}

int findContestant(std::vector<int> *pointsMinHeap, std::map<int, int> *indexHandler, int index){
  std::map<int, int>::iterator it = indexHandler->find(index);
  int heapIndex = -1;
  if(it != indexHandler->end()){
    heapIndex = it->second;
  } else {
    return -1;
  }
  return pointsMinHeap->at(heapIndex);
}

bool isInt(std::string num){
  std::string::iterator it = num.begin();
  while(it != num.end() && std::isdigit(*it)){
    ++it;
  }
  return(!num.empty() && it == num.end());
}

int getFirstInput(std::string inputLine){
  int numStart = inputLine.find("<") + 1;
  int numEnd = inputLine.find(">");
  std::string num = inputLine.substr(numStart, numEnd - numStart);
  if(!isInt(num)){
    std::cout << "Error: " << num << " is not an integer" << std::endl;
    exit(1);
  }
  return std::stoi(num);
}

int getSecondInput(std::string inputLine){
  int numStart = inputLine.find("<", inputLine.find("<") + 1) + 1;
  int numEnd = inputLine.find(">", numStart);
  std::string num = inputLine.substr(numStart, numEnd - numStart);
  if(!isInt(num)){
    std::cout << "Error: " << num << " is not an integer" << std::endl;
  }
  return std::stoi(num);
}

int main(int argc, char* argv[]){
  if(argc != 3){
    std::cout << "Usage: ./program2 <input file name> <output file name>" << std::endl;
    exit(1);
  }

  std::fstream inputFile;
  inputFile.open(argv[1]);
  if(!inputFile.is_open()){
    std::cout << "Error: Input file does not exist" << std::endl;
    exit(1);
  }
  std::ofstream outputFile;
  outputFile.open(argv[2]);
  if(!inputFile.is_open()){
    std::cout << "Error: Output file could not open" << std::endl;
    exit(1);
  }

  std::vector<int> pointsMinHeap;
  std::map<int, int> indexHandler;
  int maxHeapSize = -1;

  std::string inputLine;
  while(std::getline(inputFile, inputLine)){
    if(maxHeapSize == -1){
      //Sets max heap size
      if(isInt(inputLine)){
        maxHeapSize = std::stoi(inputLine);
      } else {
        std::cout << "Error: " << inputLine << " is not an integer" << std::endl;
      }
    } else if(inputLine.length() >= 18 && inputLine.substr(0, 14) == "findContestant") {
      //Find Contestant
      outputFile << inputLine << std::endl;
      int inputNum = getFirstInput(inputLine);
      int contestantPoints = findContestant(&pointsMinHeap, &indexHandler, inputNum);
      if(contestantPoints == -1){
        outputFile << "Contestant <" << inputNum << "> is not in the extended heap." << std::endl;
      } else {
        outputFile << "Contestant <" << inputNum << "> is in the extended heap with score <" << contestantPoints << ">." << std::endl;
      }
    } else if(inputLine.length() >= 24 && inputLine.substr(0, 16) == "insertContestant"){
      //Insert Contestant
      outputFile << inputLine << std::endl;
      int inputNum1 = getFirstInput(inputLine);
      int inputNum2 = getSecondInput(inputLine);
      if(findContestant(&pointsMinHeap, &indexHandler, inputNum1) != -1){
        outputFile << "Contestant <" << inputNum1 << "> is already in the extended heap: cannot insert." << std::endl;
      } else {
        if((int)pointsMinHeap.size() >= maxHeapSize){
          outputFile << "Contestant <" << inputNum1 << "> could not be inserted because the extended heap is full." << std::endl;
        } else {
          insertContestant(&pointsMinHeap, &indexHandler, inputNum1, inputNum2);
          outputFile << "Contestant <" << inputNum1 << "> inserted with initial score <" << inputNum2 << ">." << std::endl;
        }
      }
    } else if(inputLine == "eliminateWeakest"){
      //Eliminate Weakest
      outputFile << inputLine << std::endl;
      std::string result = eliminateWeakest(&pointsMinHeap, &indexHandler);
      outputFile << result << std::endl;
    } else if(inputLine.length() >= 18 && inputLine.substr(0, 10) == "earnPoints") {
      //Earn Points
      outputFile << inputLine << std::endl;
      int inputNum1 = getFirstInput(inputLine);
      int inputNum2 = getSecondInput(inputLine);
      if(findContestant(&pointsMinHeap, &indexHandler, inputNum1) == -1){
        outputFile << "Contestant <" << inputNum1 << "> is not in the extended heap." << std::endl;
      } else {
        int pointsIndex = indexHandler[inputNum1];
        pointsMinHeap[pointsIndex] += inputNum2;
        minHeapify(&pointsMinHeap, &indexHandler);
        pointsIndex = indexHandler[inputNum1];
        outputFile << "Contestant <" << inputNum1 << ">'s score increased by <" << inputNum2 << "> points to <" << pointsMinHeap[pointsIndex] << ">." << std::endl;
      }
    } else if(inputLine.length() >= 18 && inputLine.substr(0, 10) == "losePoints") {
      //Lose Points
      outputFile << inputLine << std::endl;
      int inputNum1 = getFirstInput(inputLine);
      int inputNum2 = getSecondInput(inputLine);
      if(findContestant(&pointsMinHeap, &indexHandler, inputNum1) == -1){
        outputFile << "Contestant <" << inputNum1 << "> is not in the extended heap." << std::endl;
      } else {
        int pointsIndex = indexHandler[inputNum1];
        pointsMinHeap[pointsIndex] -= inputNum2;
        minHeapify(&pointsMinHeap, &indexHandler);
        outputFile << "Contestant <" << inputNum1 << ">'s score decreased by <" << inputNum2 << "> points to <" << pointsMinHeap[pointsIndex] << ">." << std::endl;
      }
    } else if(inputLine == "showContestants") {
      //Show Contestnats
      outputFile << inputLine << std::endl;
      printExtendedHeap(&pointsMinHeap, &indexHandler, &outputFile);
    } else if(inputLine == "showHandles"){
      //Show Handles
      outputFile << inputLine << std::endl;
      printHandles(&indexHandler, &outputFile, maxHeapSize);
    } else if(inputLine.length() >= 16 && inputLine.substr(0, 12) == "showLocation"){
      //Show Location
      outputFile << inputLine << std::endl;
      int inputNum1 = getFirstInput(inputLine);
      if(findContestant(&pointsMinHeap, &indexHandler, inputNum1) == -1){
        outputFile << "There is no Contestant <" << inputNum1 << "> in the extended heap: handle[<" << inputNum1 << ">] = -1." << std::endl;
      } else {
        int heapLocation = indexHandler[inputNum1];
        outputFile << "Contestant <" << inputNum1 << "> stored in extended heap location <" << heapLocation << ">." << std::endl;
      }
    } else if(inputLine == "crownWinner") {
      outputFile << inputLine << std::endl;
      crownWinner(&pointsMinHeap, &indexHandler, &outputFile);
    } else {
      //Invaled Instruction
      outputFile << inputLine << std::endl;
      std::cout << "Instruction " << inputLine << " is invalid." << std::endl;
    }
  }
}
