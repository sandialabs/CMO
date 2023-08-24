//
// SEMTask.h
//
// This is a generic Task object that will run a remote command of the
// form
//     program <infile> <outfile>
// for a worker.
//

#ifndef __SEMTask_h
#define __SEMTask_h

#ifdef OK
#undef OK
#endif
#include "MWTask.h"

#include <utilib/_generic.h>
#include <utilib/PackBuf.h>
#include <utilib/comments.h>
#include <utilib/CharString.h>

using utilib::CharString;
using utilib::PackBuffer;
using utilib::UnPackBuffer;


#ifdef OK
#undef OK
#endif

template <class CommType>
class SEMWorker;

template <class CommType>
class SEMDriver;


template <class CommType>
class SEMTask : public MWTask<CommType> {

  friend class SEMWorker<CommType>;

  friend class SEMDriver<CommType>;

public:
    
  ///
  SEMTask(SEMDriver<CommType>* master, CommType* comm);

  ///
  SEMTask(SEMDriver<CommType>* master, CommType* comm,  
	CharString& taskcmd, CharString& taskinfile, CharString& taskoutfile);

  ///
  void pack_work( void );
    
  ///
  void unpack_work( void );
    
  ///
  void pack_results( void );
    
  ///
  void unpack_results( void );

protected:

  ///
  SEMDriver<CommType>* sem_master;

  ///
  CharString taskcmd;

  ///
  CharString taskinfile;

  ///
  CharString taskoutfile;

  ///
  PackBuffer pack;

  ///
  UnPackBuffer unpack;
};


template <class CommType>
SEMTask<CommType>::SEMTask(SEMDriver<CommType>* master_, CommType* comm)
  : MWTask<CommType>(master_,comm)
{
master = master_;				// WEH - debugging
sem_master = master_;				// WEH - debugging
RMC = comm;					// WEH - debugging
}


template <class CommType>
SEMTask<CommType>::SEMTask(SEMDriver<CommType>* master_, CommType* comm,  
				CharString& taskcmd_,
				CharString& taskinfile_,
				CharString& taskoutfile_)
  : MWTask<CommType>(master_,comm),
    taskcmd(taskcmd_),
    taskinfile(taskinfile_),
    taskoutfile(taskoutfile_)
{
master = master_;				// WEH - debugging
sem_master = master_;				// WEH - debugging
RMC = comm;					// WEH - debugging
}


template <class CommType>
void SEMTask<CommType>::pack_results( void ) 
{
int len;
//
// Pack taskoutfile
//
len = taskoutfile.size();
RMC->pack(&len);
RMC->pack(taskoutfile.data());
//
// Open taskoutfile
//
ifstream ifstr(taskoutfile.data());
if (!ifstr)
   EXCEPTION_MNGR(runtime_error, "SEMTask::pack_results - task file \"" << taskoutfile << "\" does not exist!");
//
// Pack taskoutfile
//
pack.reset();
pack << ifstr;
len = pack.size();
RMC->pack(&len);
RMC->pack_explicit(pack.buf(), pack.size());
ifstr.close();
}


template <class CommType>
void SEMTask<CommType>::unpack_results( void ) 
{
int tmp;
//
// Unpack taskoutfile name
//
RMC->unpack(&tmp);
taskoutfile.resize(tmp);
RMC->unpack(taskoutfile.data());
//
// Open taskoutfile
//
CharString ofile = sem_master->working_dir;
ofile += "/";
ofile += taskoutfile;
ofstream ofstr(ofile.data());
if (!ofstr)
   EXCEPTION_MNGR(runtime_error, "SEMTask::unpack_results - task file \"" << ofile << "\" cannot be created!");
//
//
RMC->unpack(&tmp);
unpack.reset();
unpack.setup(RMC->unpack_buffer() + RMC->unpack_index(), tmp);
unpack >> ofstr;
ofstr.close();
}


template <class CommType>
void SEMTask<CommType>::pack_work( void )
{
int len;
//
// Pack taskcmd
//
len = taskcmd.size();
RMC->pack(&len);
RMC->pack(taskcmd.data());
//
// Pack taskinfile name
//
len = taskinfile.size();
RMC->pack(&len);
RMC->pack(taskinfile.data());
//
// Pack taskoutfile name
//
len = taskoutfile.size();
RMC->pack(&len);
RMC->pack(taskoutfile.data());
//
// Open taskinfile
//
CharString ifile = sem_master->working_dir;
ifile += "/";
ifile += taskinfile;
ifstream ifstr(ifile.data());
MWprintf(100,"Packing taskinfile: %s\n",ifile.data());
if (!ifstr)
   EXCEPTION_MNGR(runtime_error, "SEMTask::pack_work - task file \"" << ifile << "\" does not exist!");
//
// Pack taskinfile
//
pack.reset();
pack << ifstr;
len = pack.size();
RMC->pack(&len);
RMC->pack_explicit(pack.buf(), pack.size());
ifstr.close();
}



template <class CommType>
void SEMTask<CommType>::unpack_work( void )
{
int tmp;
//
// Unpack taskcmd
//
RMC->unpack(&tmp);
taskcmd.resize(tmp);
RMC->unpack(taskcmd.data());
//
// Unpack taskinfile name
//
RMC->unpack(&tmp);
taskinfile.resize(tmp);
RMC->unpack(taskinfile.data());
//
// Unpack taskoutfile name
//
RMC->unpack(&tmp);
taskoutfile.resize(tmp);
RMC->unpack(taskoutfile.data());
//
// Open taskinfile
//
ofstream ofstr(taskinfile.data());
if (!ofstr)
   EXCEPTION_MNGR(runtime_error, "SEMTask::unpack_work - task file \"" << taskinfile << "\" cannot be created!");
//
//
RMC->unpack(&tmp);
unpack.reset();
unpack.setup(RMC->unpack_buffer() + RMC->unpack_index(), tmp);
unpack >> ofstr;
ofstr.close();
}
    
#endif
