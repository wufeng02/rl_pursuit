#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <rl_pursuit/gflags/gflags.h>
#include <rl_pursuit/factory/ClassifierFactory.h>
#include <rl_pursuit/common/Util.h>
#include <rl_pursuit/learning/ArffReader.h>

DEFINE_bool(train,true,"Train after adding data");

int main(int argc, char *argv[]) {
  std::string usage = "Usage: runClassifier [options] optionFile testFile numTrainingInstances";
  parseCommandLineArgs(&argc,&argv,usage,3,3);

  std::string optionFile = argv[1];
  std::string testFile   = argv[2];
  unsigned int numTrainingInstances = boost::lexical_cast<unsigned int>(argv[3]);

  Json::Value options;

  if (! readJson(optionFile,options))
    return 1;

  ClassifierPtr classifier = createClassifier(options);
  
  std::ifstream testIn(testFile.c_str());
  ArffReader testReader(testIn);
  for (unsigned int count = 0; count < numTrainingInstances; count++) {
    if (testReader.isDone()) {
      std::cerr << "ERROR: Insufficient training data" << std::endl;
      std::cerr << "Found " << count << " instances, expected " << numTrainingInstances << std::endl;
      exit(2);
    }
    InstancePtr instance = testReader.next();
    classifier->addData(instance);
  }
  double trainingTime = 0.0;
  if (FLAGS_train) {
    double startTime = getTime();
    classifier->train(false);
    trainingTime = getTime() - startTime;
    std::cout << "Training time: " << trainingTime << std::endl;
  }
  
  std::cout << "------------------------------------------" << std::endl;
  //std::cout << *classifier << std::endl;

  double correct = 0.0;
  int correctCount = 0;
  int count;
  for (count = 0; !testReader.isDone(); count++) {
    InstancePtr instance = testReader.next();
    Classification c;
    classifier->classify(instance,c);
    //std::cout << *instance << std::endl;
    //std::cout << "  ";
    //for (unsigned int i = 0; i < c.size(); i++)
      //std::cout << c[i] << " ";
    //std::cout << std::endl;
    // calculate the fraction correct
    correct += c[instance->label];
    // calculate whether most probable was correct
    unsigned int maxInd = vectorMaxInd(c);
    if (maxInd == instance->label)
      correctCount++;
  }
  testIn.close();

  std::cout << "------------------------------------------" << std::endl;
  std::cout << "Target Training Insts: " << numTrainingInstances << std::endl;
  std::cout << "Testing Insts: " << count << std::endl;
  std::cout << "Frac  Correct: " << correct << "(" << correct / count << ")" << std::endl;
  std::cout << "Num   Correct: " << correctCount << "(" << correctCount / (float)count << ")" << std::endl;
  std::cout << "Training time: " << trainingTime << std::endl;

  return 0;
}
