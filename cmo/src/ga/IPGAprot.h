/*  _________________________________________________________________________
 *
 *  SGOPT: A C++ library for (stochastic) global optimization.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top SGOPT directory.
 *  _________________________________________________________________________
 */

//
// IPGAprot.h
//

#ifndef __IPGAprot
#define __IPGAprot

#include <sgopt/IIntOptimizer.h>
#include <sgopt/IPGAbase.h>
#include "PGAprot.h"
#include "PGALocalSearch.h"


class SGOPT_API IPGAprot: virtual public IPGAbase, virtual public IIntOptimizer {

private:

  int popsize;
  int debug_pop_distribution;
  IntOptimizer* ls_obj;
  REAL ls_innovation_rate;
  REAL ls_freq;
  int ls_id;
  int lamarck_flag;
  int ls_niters;
  int ls_nhelpers;

public:

  IPGAprot();

  int setup(PGAprot&, PGALocalSearch* );
};


SGOPT_API int opt_init(PGAint& opt, int& argc, char** argv, PGALocalSearch* ls);


template <typename STREAM>
int opt_init(PGAprot& opt, STREAM& is, PGALocalSearch* ls)
{
IPGAprot IO;
int status = IO.process(is);
if (status == ERR) {
   IO.write(cerr);
   return ERR;
   }
return IO.setup(opt, ls);
}

 
#endif
