//
// SEMWorker.h
//

#ifndef __SEMWorker_h
#define __SEMWorker_h

#include "MWWorker.h"
#include "SEMTask.h"

template <class CommType>
class SEMTask;


template <class CommType>
class SEMWorker : public MWWorker<CommType>
{

  friend class SEMTask<CommType>;
  
public:

    SEMWorker(CommType* RMC_)
		: MWWorker<CommType>(RMC_)
		{ task = new SEMTask<CommType>(0,RMC_); }

    typename MWBase::MWReturn unpack_init_data( void )
		{ return OK; }
    
    void execute_task( MWTask<CommType> * );


protected:

};


template <class CommType>
void SEMWorker<CommType>::execute_task( MWTask<CommType> *t ) 
{
MWprintf(10,"SEMWorker execute_task begins\n");

SEMTask<CommType>* tf = dynamic_cast<SEMTask<CommType>*> ( t );

CharString cmd;
cmd = "./sem_worker ";
cmd += tf->taskcmd;
cmd += " ";
cmd += tf->taskinfile;
cmd += " ";
cmd += tf->taskoutfile;

MWprintf(20,"SEMWorker executing: %s\n", cmd.data());
system(cmd.data());

MWprintf(10,"SEMWorker execute_task finishes\n");
}


#endif
