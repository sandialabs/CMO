//
// cmo_driver.cpp
//
// The serial/MPI code that runs a driver for CMO.  This drives both
// LP solves as well as meta-heuristic solves.
//
// The executable has the following parameters:
//
// cmo_driver <task_list> [<out_file>]
//

#ifdef USING_MPI
#include "MWExecMpi.h"
#else
#include "MWExecInd.h"
#endif

#include "CMODriver.h"
#include "CMOWorker.h"


int main( int argc, char *argv[] ) 
{
utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);

set_MWprintf_level(99);
MWprintf(10,"Starting CMO Driver\n");

#ifdef USING_MPI
MWExec< CMODriver<MWMpiComm>, CMOWorker<MWMpiComm>, MWMpiComm > driver;
//MWAbstractDriver<MWMpiComm>::master_slowdown=2;
#else
MWExec< CMODriver<MWIndRC>, CMOWorker<MWIndRC>, MWIndRC > driver;
#endif

/*
MPI_Init(&argc,&argv);
int tmp;
MPI_Comm_rank(MPI_COMM_WORLD,&tmp);
cin >> tmp;
*/

try {
  driver.go(argc,argv);
}
catch (std::exception& err) {
  cerr << "Caught Exception: " << err.what() << endl;
}

MWprintf(10,"Finishing CMO Driver\n");
//MPI_Finalize();
return 0;
}
