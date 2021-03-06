#ifndef CLASSIFIER_4OFDPALS
#define CLASSIFIER_4OFDPALS

/*
File: Classifier.h
Author: Samuel Barrett
Description: abstract classifier
Created:  2011-11-22
Modified: 2011-12-27
*/

#include <boost/unordered_map.hpp>
#include "Common.h"
#include <rl_pursuit/common/RNG.h>

typedef boost::unordered_map<Instance,Classification> ClassificationCache;
std::size_t hash_value(const Instance &i);

class Classifier {
public:
  Classifier(const std::vector<Feature> &features, bool caching);

  virtual ~Classifier() {}
  void train(bool incremental=true);
  void classify(const InstancePtr &instance, Classification &classification);
  virtual void addData(const InstancePtr &instance) = 0;
  virtual void addSourceData(const InstancePtr &instance) {
    addData(instance); // same unless we're doing transfer
  }

  std::ostream& outputHeader(std::ostream &out) const;
  virtual void outputDescription(std::ostream &out) const = 0;
  void setPredictSingleClass(bool flag);
  void setRNG(boost::shared_ptr<RNG> newRNG) {
    rng = newRNG;
  }

  virtual void save(const std::string &filename) const = 0;
  
  virtual bool load(const std::string &filename) = 0;

  virtual void clearData() = 0;
  std::vector<Feature> getFeatures() const {
    return features;
  }

  virtual Classifier* copyWithWeights(const InstanceSet &) {
    return NULL;
  }

protected:
  virtual void trainInternal(bool incremental) = 0;
  virtual void classifyInternal(const InstancePtr &instance, Classification &classification) = 0;
  std::string getSubFilename(const std::string &baseFilename, const std::string &sub) const;
  std::string getSubFilename(const std::string &baseFilename, unsigned int i) const;
  std::vector<std::string> getSubFilenames(const std::string &baseFilename, unsigned int maxInd) const;


protected:
  std::vector<Feature> features;
  const std::string classFeature;
  unsigned int numClasses;
  bool caching;
  boost::shared_ptr<ClassificationCache> cache;
  bool predictSingleClass;
  boost::shared_ptr<RNG> rng;

  friend std::ostream& operator<<(std::ostream &out, const Classifier &c);
};

typedef boost::shared_ptr<Classifier> ClassifierPtr;

#endif /* end of include guard: CLASSIFIER_4OFDPALS */
