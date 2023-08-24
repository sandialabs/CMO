//
// CMOWorker.h
//

#ifndef __CMOWorker_h
#define __CMOWorker_h

#include "MWWorker.h"
#include "CMOTask.h"

template <class CommType>
class CMOTask;


template <class CommType>
class CMOWorker : public MWWorker<CommType>
{

  friend class CMOTask<CommType>;
  
public:

    CMOWorker(CommType* RMC_)
		: MWWorker<CommType>(RMC_)
		{ task = new CMOTask<CommType>(0,RMC_); }

    typename MWBase::MWReturn unpack_init_data( void )
		{ return OK; }
    
    void execute_task( MWTask<CommType> * );


protected:

};


template <class CommType>
void CMOWorker<CommType>::execute_task( MWTask<CommType> *t ) 
{
MWprintf(10,"CMOWorker execute_task begins\n");

CMOTask<CommType>* tf = dynamic_cast<CMOTask<CommType>*> ( t );

CharString cmd;
cmd = "cmo_worker ";
cmd += tf->taskcmd;
cmd += " ";
cmd += tf->taskinfile;
cmd += " ";
cmd += tf->taskoutfile;

MWprintf(20,"CMOWorker executing: %s\n", cmd.data());
system(cmd.data());

MWprintf(10,"CMOWorker execute_task finishes\n");
}


#endif
