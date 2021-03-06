#!/usr/bin/env python

import sys, random, os
from common import getUniqueStudents, makeTemp, getFilename, TRAIN, DESC
from createDT import resample, makeTree #, removeTrialStep

def calcProbs(students,numSource,numTarget,studentInd):
  numInstances = (len(students) - 1) * numSource + numTarget
  probs = [numSource / float(numInstances) for s in students]
  probs[studentInd] = numTarget / float(numInstances)
  return probs

def calcBalancedProbs(students,numSource,numTarget,studentInd):
  probs = [0.5 / (len(students) - 1) for s in students]
  probs[studentInd] = 0.5
  return probs

def main(args=sys.argv[1:]):
  usage = 'createTrBagg [--balanced] numTarget numSource jobStart [numJobs]'
  balanced = False
  if '--balanced' in args:
    args.remove('--balanced')
    balanced = True
  if (len(args) < 3) or (len(args) > 4):
    print >>sys.stderr,usage
    sys.exit(1)
  numClassifiers = 1000
  numTarget = int(args[0])
  numSource = int(args[1])
  jobStart = int(args[2])
  if len(args) >= 4:
    numJobs = int(args[3])
  else:
    numJobs = 1
  jobStart *= numJobs
  base = 'studentsNew29-unperturbed-%i'
  outputBase = 'studentsNew29-unperturbed-transfer/target%i-source%i' % (numTarget,numSource)
  if not os.path.exists('data/dt/' + outputBase + '/desc'):
    os.makedirs('data/dt/' + outputBase + '/desc')
  if not os.path.exists('data/dt/' + outputBase + '/weighted'):
    os.makedirs('data/dt/' + outputBase + '/weighted')

  for jobOffset in range(numJobs):
    jobNum = jobStart + jobOffset
    studentInd = jobNum / numClassifiers
    classifierInd = jobNum % numClassifiers
    print 'job,student,classifier:',jobNum,studentInd,classifierInd

    students = getUniqueStudents()
    if studentInd >= len(students):
      return
    if balanced:
      probs = calcBalancedProbs(students,numSource,numTarget,studentInd)
    else:
      probs = calcProbs(students,numSource,numTarget,studentInd)
    print 'probs:',probs,sum(probs)

    counts = [0 for s in students]
    for i in range(numTarget):
      r = random.random()
      total = 0
      for j,p in enumerate(probs):
        total += p
        if r < total:
          ind = j
          break
      else:
        ind = len(probs) - 1
      counts[ind] += 1
    eps = 1e-10
    props = [eps + float(c) / (numTarget if i == studentInd else numSource) for i,c in enumerate(counts)]

    try:
      arffFilename = makeTemp('.arff')
      #arffFilenameFilt = makeTemp('.arff')
      tempFile = makeTemp('.arff')
      with open(arffFilename,'w') as arffFile:
        for i,(student,prop) in enumerate(zip(students,props)):
          inFile = getFilename(base % (numTarget if student == students[studentInd] else numSource),student,TRAIN)
          print 'resampling',student
          resample(inFile,tempFile,prop)
          with open(tempFile,'r') as f:
            if i != 0:
              for line in f:
                if line.strip() == '@data':
                  break
            for line in f:
              arffFile.write(line)
      print 'removing trial step'
      #removeTrialStep(arffFilename,arffFilenameFilt)
      if balanced:
        name = 'trBagg-balanced-%s-%i' % (students[studentInd],classifierInd)
      else:
        name = 'trBagg-%s-%i' % (students[studentInd],classifierInd)
      makeTree(arffFilename,True,None,outputBase,name,[],False,1.0)
      #makeTree(arffFilenameFilt,True,None,outputBase,'trBagg-%s-%i' % (students[studentInd],classifierInd),[],False,1.0)
    finally:
      os.remove(arffFilename)
      #os.remove(arffFilenameFilt)
      os.remove(tempFile)
      os.remove(getFilename(outputBase,name,DESC))


if __name__ == '__main__':
  main()
