//
// sem_driver.cpp
//
// The serial/MPI code that runs a driver for SEM evalutions.
//

#include <utilib/PackBuf.h>
#undef OK
#ifdef USING_MPI
#include "MWExecMpi.h"
#else
#include "MWExecInd.h"
#endif

#include "SEMDriver.h"
#include "SEMWorker.h"


int main( int argc, char *argv[] ) 
{
utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);

set_MWprintf_level(99);
MWprintf(10,"Starting SEM Driver\n");

#ifdef USING_MPI
MWExec< SEMDriver<MWMpiComm>, SEMWorker<MWMpiComm>, MWMpiComm > driver;
//MWAbstractDriver<MWMpiComm>::master_slowdown=2;
#else
MWExec< SEMDriver<MWIndRC>, SEMWorker<MWIndRC>, MWIndRC > driver;
#endif

try {
  driver.go(argc,argv);
}
catch (std::exception& err) {
  cerr << "Caught Exception: " << err.what() << endl;
}

MWprintf(10,"Finishing SEM Driver\n");
//MPI_Finalize();
return 0;
}
