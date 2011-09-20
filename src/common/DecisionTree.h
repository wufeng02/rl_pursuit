#ifndef DECISIONTREE_AX8LS8WW
#define DECISIONTREE_AX8LS8WW

/*
File: DecisionTree.h
Author: Samuel Barrett
Description: a decision tree
Created:  2011-09-13
Modified: 2011-09-13
*/

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <limits>

enum ComparisonOperator{
  EQUALS,
  LESS,
  GEQ
};

//typedef std::vector<float> Classification;
typedef unsigned int Classification;
typedef boost::unordered_map<std::string,float> Features;

class DecisionTree {
public:
  struct Node {
    virtual void classify(const Features &features, Classification &classification) = 0;
    virtual std::ostream& genDescription(std::ostream &out) = 0;
  };
  struct InteriorNode: public Node {
    InteriorNode(ComparisonOperator cmp, const std::string &splitKey);
    void addChild(boost::shared_ptr<Node> child, float splitValue);
    void classify(const Features &features, Classification &classification);
    unsigned int getInd(const Features &features);
    std::ostream& genDescription(std::ostream &out);

    ComparisonOperator cmp;
    std::string splitKey;
    std::vector<boost::shared_ptr<Node> > children;
    std::vector<float> splitValues;
    static const float EPS;
  };

  struct LeafNode: public Node {
    LeafNode(const Classification &classification);
    void classify(const Features &features, Classification &classification);
    std::ostream& genDescription(std::ostream &out);

    Classification classification;
  };

  DecisionTree(boost::shared_ptr<Node> root);
  void classify(const Features &features, Classification &classification);

private:
  boost::shared_ptr<Node> root;
};

std::ostream& operator<<(std::ostream &out, boost::shared_ptr<DecisionTree::Node> node);


#endif /* end of include guard: DECISIONTREE_AX8LS8WW */