/*  _________________________________________________________________________
 *
 *  SGOPT: A C++ library for (stochastic) global optimization.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top SGOPT directory.
 *  _________________________________________________________________________
 */

//
// IPGAprot.cpp
//
// Interface for PGAprot
//
 
#include "IPGAprot.h"


IPGAprot::IPGAprot()
{
popsize=100;
debug_pop_distribution=0;
 
data_optn("popsize",&popsize,"[popsize=100]");
data_optn("debug_pop_distribution",&debug_pop_distribution,"[debug_pop_distribution=0]");

ls_obj = NULL;
ls_freq=1.0;
ls_innovation_rate=0.0;
ls_id=-2;			/// Default is multimeme
lamarck_flag=TRUE;
ls_nhelpers=7;  /// Default for multimeme is 7 (number of currently available local searchers
ls_niters=10;

data_optn("ls_obj",&ls_obj,"[ls_obj = NULL]");
data_optn("ls_freq",&ls_freq,"[ls_freq = 0.0]");
data_optn("ls_id",&ls_id,"[ls_id = 0]");
data_optn("ls_num_helpers",&ls_nhelpers,"[ls_num_helpers = 1]");
data_optn("ls_niters",&ls_niters,"[ls_niters = 1]");
data_optn("ls_irate",&ls_innovation_rate,"[ls_irate = 0.0]");
simple_flag("lamarck_ls",&lamarck_flag);
}


int IPGAprot::setup(PGAprot& ga, PGALocalSearch* ls)
{
int status;
status = IIntOptimizer::setup(&ga);
if (status == ERR) return status;
status = IPGAbase::setup(&ga);
if (status == ERR) return status;

ga.init_population(popsize);
 
if (ls) {
   ls->setLocalSearcherIterationNumber(ls_niters);
   ga.set_helpersNumber(ls_nhelpers);
   ls->setLocalSearcherType(ls_id);
   ga.set_innovation_rate(ls_innovation_rate);
   ga.setup_ls(ls_obj,ls_freq);
   }

if (ga.debug > 4)
   ga.selection_obj.debug = ga.debug;
if (debug_pop_distribution)
   ga.debug_pop_distribution(1);

return OK;
}


SGOPT_API int opt_init(PGAprot& opt, int& argc, char** argv, PGALocalSearch* ls)
{
IPGAprot IO;
int status = IO.process(argc,argv);
if (status == ERR) {
   IO.write(cerr);
   return ERR;
   }
return IO.setup(opt,ls);
}
 
SGOPT_API int opt_init(PGAprot& opt, istream& is, PGALocalSearch* ls)
{
IPGAprot IO;
int status = IO.process(is);
if (status == ERR) {
   IO.write(cerr);
   return ERR;
   }
return IO.setup(opt,ls);
}




