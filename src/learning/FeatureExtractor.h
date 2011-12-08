#ifndef FEATUREEXTRACTOR_XSHC1OD2
#define FEATUREEXTRACTOR_XSHC1OD2

/*
File: FeatureExtractor.h
Author: Samuel Barrett
Description: extracts a set of features of the agents
Created:  2011-10-28
Modified: 2011-10-28
*/

#include <model/Common.h>
#include <common/Point2D.h>
#include "Classifier.h"
#include <controller/Agent.h>
#include <boost/unordered_map.hpp>

class FeatureExtractor {
public:
  FeatureExtractor(const Point2D &dims);
  
  void addFeatureAgent(const std::string &key, const std::string &name);
  InstancePtr extract(const Observation &obs);

protected:
  struct FeatureAgent {
    std::string name;
    boost::shared_ptr<Agent> agent;
  };

  void setFeature(InstancePtr &instance, const std::string &key, float val);

protected:
  const Point2D dims;
  std::vector<FeatureAgent> featureAgents;
  std::vector<Feature> featureTypes;
};

#endif /* end of include guard: FEATUREEXTRACTOR_XSHC1OD2 */