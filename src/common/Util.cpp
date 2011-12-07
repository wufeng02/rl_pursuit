#include "Util.h"

#include <sys/time.h>
#include <fstream>
#include <cmath>
#include <cassert>
#include <gflags/gflags.h>
#include <stdlib.h>

double getTime() {
  struct timeval time;

  gettimeofday(&time,NULL);
  return time.tv_sec + time.tv_usec / 1000000.0;
}

// returns the probability of x1 using a softmax with the given factor
float softmax(float x1, float x2, float factor) {
  x1 = exp(factor * x1);
  x2 = exp(factor * x2);
  return x1 / (x1 + x2);
}

// fills out probs with the probabilities of the vals using a softmax with the given factor
void softmax(const std::vector<unsigned int> &vals, float factor, std::vector<float> &probs) {
  assert(vals.size() >= 1);
  probs.clear();
  std::vector<float> expVals(vals.size());
  float total = 0;

  for (unsigned int i = 0; i < vals.size(); i++) {
    expVals[i] = exp(factor * vals[i]);
    total += expVals[i];
  }
  for (unsigned int i = 0; i < vals.size(); i++) {
    probs.push_back(expVals[i] / total);
  }
}

bool readJson(const std::string &filename, Json::Value &value) {
  Json::Reader reader;
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    std::cerr << "readJson: ERROR opening file: " << filename << std::endl;
    return false;
  }
  Json::Value root;
  bool parsingSuccessful = reader.parse(in,root);
  in.close();
  if (!parsingSuccessful) {
    std::cerr << "readJson: ERROR parsing file: " << filename << std::endl;
    std::cerr << reader.getFormatedErrorMessages();
    return false;
  }
  Json::Value::Members members = root.getMemberNames();
  for (unsigned int i = 0; i < members.size(); i++) {
    value[members[i]] = root[members[i]];
  }

  return true;
}

void jsonReplaceStrings(Json::Value &value, const std::map<std::string,std::string> &replacementMap) {
  // change this value if necessary
  if (value.isString()) {
    std::map<std::string,std::string>::const_iterator it;
    std::string str = value.asString();
    for (it = replacementMap.begin(); it != replacementMap.end(); it++) {
      size_t pos = str.find(it->first);
      if (pos != std::string::npos) {
        str.replace(pos,it->first.length(),it->second);
      }
    }
    value = Json::Value(str);
  }

  // check its children
  Json::Value::iterator child;
  if (value.isObject()) {
    std::vector<std::string> names = value.getMemberNames();
    for (unsigned int i = 0; i < names.size(); i++) {
      jsonReplaceStrings(value[names[i]],replacementMap);
    }
  } else if (value.isArray()) {
    for (unsigned int i = 0; i < value.size(); i++) {
      jsonReplaceStrings(value[i],replacementMap);
    }
  }
  //int i = 0;
  //for (child = value.begin(); child != value.end(); child++) {
    //i++;
    ////std::cout << (child != value.end()) << std::endl;
    //std::cout << value << std::endl << std::flush;
    //std::cout << i << " of " << value.size() << std::endl << std::flush;
    //std::cout << *child << std::endl << std::flush;
    //jsonReplaceStrings(*child,replacementMap);
  //}
}

std::string indent(unsigned int indentation) {
  return std::string(indentation * 2,' ');
}

DEFINE_bool(h,false,"Print help");

void parseCommandLineArgs(int *argc, char ***argv, const std::string &usage, int minArgs, int maxArgs) {
  google::SetUsageMessage(usage);
  google::ParseCommandLineNonHelpFlags(argc, argv, true);
  
  // check if help was set
  std::string helpStr;
  google::GetCommandLineOption("help",&helpStr);
  if ((helpStr == "true") || FLAGS_h)
    printCommandLineHelpAndExit();

  google::HandleCommandLineHelpFlags();
  // check the number of remaining arguments
  if (((minArgs >= 0) && (*argc -1 < minArgs)) || ((maxArgs >= 0) && (*argc - 1 > maxArgs))) {
    std::cerr << "WARNING: Incorrect number of arguments, got " << *argc - 1 << " expected ";
    if (maxArgs == minArgs)
      std::cerr << minArgs;
    else
      std::cerr << minArgs << "-" << maxArgs;
    std::cerr << std::endl;
    printCommandLineHelpAndExit();
  }
}

void printCommandLineHelpAndExit() {
  std::cout << google::GetArgv0() << ": " << google::ProgramUsage() << std::endl;
  std::vector<google::CommandLineFlagInfo> flags;
  google::GetAllFlags(&flags);
  for (unsigned int i = 0; i < flags.size(); i++) {
    if (((flags[i].filename == "src/gflags.cc") || (flags[i].filename == "src/gflags_completions.cc") || (flags[i].filename == "src/gflags_reporting.cc")) && (flags[i].name != "help") && (flags[i].name != "helpfull"))
      continue;
    std::cout << google::DescribeOneFlag(flags[i]);
  }
  exit(1);
}
