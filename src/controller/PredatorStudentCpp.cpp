/*
File: PredatorStudentCpp.cpp
Author: Samuel Barrett
Description: wrapper around the student's c++ predator for assignment 1
Created:  2011-09-14
Modified: 2011-09-14
*/

#include "PredatorStudentCpp.h"

const Point2D PredatorStudentCpp::moves[5] = {Point2D(0,0),Point2D(1,0),Point2D(-1,0),Point2D(0,1),Point2D(0,-1)};

PredatorStudentCpp::PredatorStudentCpp(boost::shared_ptr<RNG> rng, const Point2D &dims, const std::string &name, unsigned int predatorInd):
  Agent(rng,dims),
  name(name)
{
  predator = createPredator(name,predatorInd);
}

PredatorStudentCpp::~PredatorStudentCpp() {
}

ActionProbs PredatorStudentCpp::step(const Observation &obs) {
  int pos[2];
  int preyPositions[1][2];
  int predatorPositions[4][2];
  
  pos[0] = obs.myPos().x;
  pos[1] = obs.myPos().y;
  preyPositions[0][0] = obs.preyPos().x;
  preyPositions[0][1] = obs.preyPos().y;
  int ind = 0;
  for (unsigned int i = 0; i < 4; i++) {
    if (ind == obs.preyInd)
      ind++;
    predatorPositions[i][0] = obs.positions[ind].x;
    predatorPositions[i][1] = obs.positions[ind].y;
    ind++;
  }
  // set the random seed for the agents
  srand(rng->randomUInt());
  int action = predator->step(pos,preyPositions,predatorPositions);
  return ActionProbs(getAction(moves[action]));
}

void PredatorStudentCpp::restart() {
}

std::string PredatorStudentCpp::generateDescription() {
  return "PredatorStudentCpp: wrapper around " + name + "'s c++ predator";
}