//
// PGAprot_ds.cpp
//
// This code is a Genetic algorithm for the maximum contact map overlap
// that uses the operators (crossover and mutation) described in Lancia et.al. paper 
//



#include <fstream.h>
#include <utilib/PM_LCG.h>
#include <utilib/errmsg.h>
#include <sgopt/OptProblemDB.h>
#include <sgopt/IntOptProblem.h>
#include <sgopt/IPGAint.h>
#include "PGAprot.h"




//#include "test_problems.h"
//#include "IPGAreal.h"



using namespace std;


void init_problem(char*, char*, int&, int&);
double alignValue(int*, int);

int main(int argc, char* argv[])
{
  

    double ls_freq = 1.0;
    //    IntOptimizer* ls_obj = new PGAlocalsearch();
  
    errmsg_abort(TRUE);

    IntVector seedList;
    ifstream ifstr("seedfile");
    ifstr >> seedList;

    char  *fileName1 = argv[1];
    char  *fileName2 = argv[2];

    int interval, exponent;
    init_problem(fileName1, fileName2, interval, exponent);
   
    char range[200];
    /* because we count from 0, we have exponent variables, each one of them in the range [0,interval-1] */
    sprintf(range,"[0,%i]^%i",interval-1,exponent);
    cout<<range<<endl;

    /*The alignValue function assumes that the shortest protein is the one represented byt the
      vector and that each position in the IntVector can range from [0,interval-1] */
    IntOptProblem* prob = OptProblemConstr(&alignValue,range);
    
    //
    // Setup optimizer
    //
    PGAprot *opt= new PGAprot();
    opt->set_problem(prob);
    opt->set_rng(new PM_LCG(int(argv[3])));

    opt->accuracy = -10000;
    opt->max_iters = 2500;
    opt->max_neval = 100000000;
    opt->debug = 10000;
    opt->set_xover_rate(0.5);
    opt->set_xover_type("original");
    opt->set_mutation_rate(0.15);
    opt->set_mutation_type("sliding");
    opt->init_population(500);
    opt->set_output("summary");
    //    opt->setup_ls(ls_obj,ls_freq);

    //ifstream infile("PGAprot.in");
    //opt_init(*opt,infile);

    opt->reset();
    cout << opt;

    //
    // Perform minimization
    //
    REAL ans;
    ans = opt->minimize();

    cout << "Value: " << ans << endl;
    cout << prob->vars() << endl;

    return 0;
}
