#include "Committee.h"
Committee::Committee(const std::vector<Feature> &features, bool caching, const std::vector<SubClassifier> &classifiers, const Params &p):
  Classifier(features,caching),
  classifiers(classifiers),
  p(p)
{
  normalizeWeights();
}

void Committee::addData(const InstancePtr &instance) {
  for (unsigned int i = 0; i < classifiers.size(); i++)
    classifiers[i].classifier->addData(instance);
}

void Committee::outputDescription(std::ostream &out) const {
  out << "Committee: " << std::endl;
  for (unsigned int i = 0; i < classifiers.size(); i++)
    out << *(classifiers[i].classifier) << std::endl;
}

void Committee::trainInternal(bool incremental) {
  for (unsigned int i = 0; i < classifiers.size(); i++)
    classifiers[i].classifier->train(incremental);
}

void Committee::classifyInternal(const InstancePtr &instance, Classification &classification) {
  Classification temp;
  for (unsigned int i = 0; i < classifiers.size(); i++) {
    classifiers[i].classifier->classify(instance,temp);
    for (unsigned int j = 0; j < numClasses; j++)
      classification[j] += classifiers[i].weight * temp[j];
  }
}

void Committee::normalizeWeights() {
  float total = 0;
  for (unsigned int i = 0; i < classifiers.size(); i++)
    total += classifiers[i].weight;
  float frac = 1.0 / total;
  for (unsigned int i = 0; i < classifiers.size(); i++)
    classifiers[i].weight *= frac;
}
