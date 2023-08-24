//
// PMAprot_ds.cpp
//
// This code is a MultiMemetic algorithm for the maximum contact map overlap
// that uses the operators (crossover and mutation) described in Lancia et.al. paper and various other local searchers. Also a DPX crossover is included.




#include <fstream.h>
#include <utilib/PM_LCG.h>
#include <utilib/errmsg.h>
#include <sgopt/OptProblemDB.h>
#include <sgopt/IPGAint.h>
#include <sgopt/IntOptProblem.h>
#include "PGALocalSearch.h"
#include "PGAprot.h"



//#include "test_problems.h"
//#include "IPGAreal.h"



using namespace std;


void init_problem(char*, char*, int&, int&);
double alignValue(int*, int);




int main(int argc, char* argv[])
{
  /* ARGUMENTS: protein1 protein2 randomSeed innovationRate numberOfLocalSearchIterations xoverRate mutationRate  popSize generationNumber */ 

    
  // number of memes to use
  int numHelpers=7;

  //IntOptimizer* ls_obj = new PGALocalSearch();
    PGALocalSearch* ls_obj = new PGALocalSearch();
  
    errmsg_abort(TRUE);

    IntVector seedList;
    ifstream ifstr("seedfile");
    ifstr >> seedList;

    char  *fileName1 = argv[1];
    char  *fileName2 = argv[2];

    int interval, exponent;
    init_problem(fileName1, fileName2, interval, exponent);
   
    char range[200];
    /* because we count from 0, we have exponent variables, each one of them in the range [0,interval-1] 
       plus an extra variable that specified the identity of the meme-helper used. The identity
       is a number between 0 and numHelper-1 */
    sprintf(range,"[0,%i]^%i [0,%i]^1",interval-1,exponent,numHelpers);
    cout<<range<<endl;

    /*The alignValue function assumes that the shortest protein is the one represented by the
      vector (exponent positions) and that each position in the IntVector can range from [0,interval-1] */
      IntOptProblem* prob = OptProblemConstr(&alignValue,range);
    
    //
    // Setup optimizer
    //
    PGAprot *opt= new PGAprot();
    opt->set_myProblem(prob);
    opt->set_problem(prob);
    int rndSeed;
    sscanf(argv[3],"%i",&rndSeed);
    opt->set_rng(new PM_LCG(rndSeed));

    opt->accuracy = -10000;


    float xoverRate;
    sscanf(argv[6],"%f",&xoverRate);
    float mutationRate;
    sscanf(argv[7],"%f",&mutationRate);
    float lsRate;
    sscanf(argv[8],"%f",&lsRate);
    int popSize;
    sscanf(argv[9],"%i",&popSize);
    int genNum;
    sscanf(argv[10],"%i",&genNum);




    opt->max_iters = genNum;
    opt->max_neval = 35000000;
    opt->debug = 2;
    opt->set_xover_rate(xoverRate);

    //    opt->set_xover_type(1);

    opt->set_xover_type("dpx");

    opt->set_mutation_rate(mutationRate);

    //    opt->set_mutation_type(1);

    opt->set_mutation_type("random_edge");

    opt->init_population(popSize);
    opt->set_output("summary");
   
    /* There are  7 different local searchers to use (1..7). The local searcher number 1 implements RECOMB operator. */
    /* The next 4 are parameterized variations of the same one, namely a sliding window of a fixed size as the       */
    /* elementary move the mutation operator uses. Meme 6 is a scanning local search that takes two alignments and tries */
    /* to relocate them. The seventh one tries to spread contiguos blocks of alignments */
    /* See the file PGALocalSearch.cpp lines 66 to 120 to see the specific definitions */
    /*   of the window size. The direction and delta of the slideMove are still random */
    /* If we want one local searcher to be selected at _random_ then the local search */
    /* type should be put to 8 */
    /* When using a multimeme algorithm, the local search type should be put to -2  and */
    /* the local search strategy will take the meme to use from the individual */
    /* if using a multimeme algorithm the innovation rate should be used */
    float innovationRate;
    sscanf(argv[4],"%f",&innovationRate);
    int itVar;
    sscanf(argv[5],"%i", &itVar);
    // local search type is -2 for multimeme algorithms
    ls_obj->setLocalSearcherType(-2);
    opt->set_helpersNumber(numHelpers);
    ls_obj->setLocalSearcherIterationNumber(itVar);
    cout << "LS="<<-2<<" , IT="<< itVar<<endl;
    opt->setup_ls(ls_obj,lsRate);
    opt->set_innovation_rate(innovationRate);
    cout<< "Innovation Rate: "<<innovationRate<<endl;
    //ifstream infile("PGAprot.in");
    //opt_init(*opt,infile);

    opt->reset();
    cout << (*opt);

    //
    // Perform minimization
    //
    REAL ans;
    ans = opt->minimize();

    cout << "Value: " << ans << endl;
    cout << prob->vars() << endl;

    return 0;
}
 
