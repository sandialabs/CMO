

#ifndef __PGAlocalsearch
#define __PGAlocalsearch

#include <sgopt/IntOptimizer.h>
#include <utilib/DUniform.h>
#include <utilib/default_rng.h>



class PGALocalSearch : virtual  public IntOptimizer
{

 public:
 
  PGALocalSearch(RNG* rng_=&default_rng);
  PGALocalSearch(IntOptProblem* problem, RNG* rng_=&default_rng);
  void setLocalSearcherType(int lsType);
  void setLocalSearcherIterationNumber(int itNum);
  int minimize();
  void set_rng(RNG* rng_);
  void reset()
  {
   IntOptimizer::reset(); 
   if (problem) curr.resize(problem->nvars());
  }



protected:
  
  DUniform urnd;
  IntVector curr;
  IntVector lower;
  IntVector upper;
  int       _localSearcherType;
  int       _localSearcherIterationNumber;

private:

  int PGALocalSearch::findLeft(int i,int start, IntVector * aPT);
  int PGALocalSearch::findRight(int i,int end, IntVector * aPT);
  void PGALocalSearch::slide(IntVector *pt, int start, int end, int dir, int delta);
  void PGALocalSearch::slidingMove(IntVector *tempSol,int windowSize);
  void firstFillMove(IntVector *tempSol);
  void PGALocalSearch::scanMove(IntVector *tempSol);
  void PGALocalSearch::spreadMove(IntVector *tempSol);
};


#endif
