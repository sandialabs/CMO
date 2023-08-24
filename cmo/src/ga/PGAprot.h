//
// PGAprot.h
//

#ifndef __PGAprot_h
#define __PGAprot_h

#ifdef __GNUC__
#pragma interface
#endif

#include <sgopt/PGAint.h>
#include <utilib/DUniform.h>

/** An evolutionary algorithm with genotype=IntVector and phenotype=IntVector adapted for problem-specific
	mutation/crossover methods. */

class PGAprot : public PGAint {

public:
  int mutation_type;

  int crossover_type;

  float innovation_rate;

  int helpersNum;

  IntOptProblem* myProblem;
        
  PGAprot(RNG* rng_=&default_rng);

  void set_mutation_type(const char* str);

  void set_xover_type(const char* str);

  void set_myProblem(IntOptProblem* problem_);

  void reset();

  int write(ostream& os);

  void set_helpersNumber(int aNum);

  void set_innovation_rate(double rate);

  void randomize();


  void  random_init_alignament(IntVector& myVec,int fs,int fe, int ts, int te);

protected:

  void set_op_rng(RNG* rng_);

  void apply_mutation(GA_individual& point, int parent_ndx);

  void apply_xover(GA_individual& parent1_, GA_individual& parent2_,
                        GA_individual& child_);

  void slidingMove(GApoint_int &pt, int start, int end, int dir, int delta);

  void bestFillMove( GApoint_int& ind,int pos,int start, int end, int forbiden1, int forbiden2);

  DUniform urand;
};

#endif



