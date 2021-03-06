#ifndef MODELUPDATERBAYES_S4U00VNJ
#define MODELUPDATERBAYES_S4U00VNJ

/*
File: ModelUpdaterBayes.h
Author: Samuel Barrett
Description: a model updater using bayesian updates, also handles updating by polynomial function of the loss
Created:  2011-09-21
Modified: 2011-10-02
*/

#include "ModelUpdater.h"
#include <rl_pursuit/gtest/gtest_prod.h>
#include <rl_pursuit/common/Params.h>
#include <rl_pursuit/common/Enum.h>

ENUM(ModelUpdateType,
  bayesian,
  polynomial,
  none
)

//SET_FROM_JSON_ENUM(ModelUpdateType)

class ModelUpdaterBayes: public ModelUpdater {

public:
#define PARAMS(_) \
  _(ModelUpdateType_t,modelUpdateType,update,ModelUpdateType::bayesian) \
  _(bool,allowRemovingModels,allowRemovingModels,true) \
  _(float,MIN_MODEL_PROB,minModelProb,0.0001) \
  _(int,stepsUntilSafetyModel,stepsUntilSafetyModel,-1) \
  _(std::string,safetyModelDesc,safetyModelDesc,"pd") \
  _(float,lossEta,lossEta,0.5) \
  _(bool,addUpdateNoise,addUpdateNoise,false) \
  _(float,updateNoise,updateNoise,0.05)

  Params_STRUCT(PARAMS)
#undef PARAMS

protected:
  Params p;

public:
  ModelUpdaterBayes(boost::shared_ptr<RNG> rng, const std::vector<ModelInfo> &models, const Params &p);
  void updateRealWorldAction(const Observation &prevObs, Action::Type lastAction, const Observation &currentObs);
  void updateSimulationAction(const Action::Type &action, const State_t &state);

protected:
  unsigned int selectModelInd(const State_t &state);
  void getNewModelProbs(const Observation &prevObs, Action::Type lastAction, const Observation &currentObs, std::vector<double> &newModelProbs);
  double calculateModelProb(unsigned int modelInd, const Observation &prevObs, Action::Type lastAction, const Observation &currentObs);
  bool allProbsTooLow(const std::vector<double> &newModelProbs);
  void removeLowProbabilityModels();
  std::string generateSpecificDescription();

protected:
  ModelInfo *safetyModel;
  FRIEND_TEST(ModelUpdaterBayesTest,AdvancedTests);
};

#endif /* end of include guard: MODELUPDATERBAYES_S4U00VNJ */
