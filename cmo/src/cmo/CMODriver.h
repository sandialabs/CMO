//
// CMODriver.h
//

#ifndef __CMODriver_h
#define __CMODriver_h

#include "CMOTask.h"
#ifdef OK
#undef OK
#endif
#include "MWDriver.h"

#include <iostream>
#include <cmath>
#include <utilib/string_ops.h>
#include <utilib/comments.h>
#include <utilib/PackBuf.h>
#include <utilib/CharString.h>

#ifdef OK
#undef OK
#endif

using utilib::PackBuffer;
using utilib::BasicArray;
using utilib::CharString;
using utilib::comment_lines;

/** The master class derived from the MWDriver class for this application.

    In particular, this application is a very simple one that 
    reads in a list of tasks from a file, each of which specify 
    a method and a data file.  The following syntax is used to indicate
    how tasks are specified:

    <tag> <task-name> <file-name> ;

*/

template <class CommType>
class CMODriver : public MWDriver<CommType> {

public:

  ///
  CMODriver();

  ///
  typename CMODriver<CommType>::MWReturn get_userinfo( int argc, char *argv[] );

  ///
  typename CMODriver<CommType>::MWReturn setup_initial_tasks( vector<MWTask<CommType>*>& tasks )
	{return setup_tasks(tasks,true);}

  ///
  typename CMODriver<CommType>::MWReturn setup_additional_tasks( vector<MWTask<CommType>*>& tasks )
	{return setup_tasks(tasks,false);}

  ///
  typename CMODriver<CommType>::MWReturn setup_tasks( vector<MWTask<CommType>*>& tasks, bool initial_flag );

  ///
  typename CMODriver<CommType>::MWReturn act_on_completed_task( MWTask<CommType> * );

  ///
  typename CMODriver<CommType>::MWReturn pack_worker_init_data( void )
		{return OK;}

  ///
  MWTask<CommType>* allocate_task()
		{return new CMOTask<CommType>(this,RMC);}

  ///
  CharString task_cmd;

  ///
  CharString out_file;

  ///
  CharString working_dir;

  ///
  ostream* ostr;
};



template <class CommType>
CMODriver<CommType>::CMODriver()
	: MWDriver<CommType>(),
	  task_cmd(), out_file(), working_dir(), ostr(0)
{
server_mode=true;
}


template <class CommType>
typename CMODriver<CommType>::MWReturn CMODriver<CommType>::get_userinfo( 
					int argc, char *argv[] ) 
{
if (argc == 1) {
   MWprintf(0,"ERROR: Missing required task filename.\n\tcmo_driver <working_dir> [<out_fname>]\n");
   return ABORT;
   }

int argn=1;

working_dir = argv[argn];

if ((argc-argn) > 1) {
   cout << argc << endl;
   cout << argv[0] << " " << argv[argn] << " " << argv[argn+1] << endl;
   ostr = new ofstream(argv[argn+1]);
   if (!(*ostr)) {
      MWprintf(0,"ERROR: Could not open output file \"%s\".\n", argv[argn+1]);
      return ABORT;
      }
   set_MWprintf_ostream(*ostr);
   out_file = argv[argn+1];
   }
else
   ostr = &cout;

return OK;
}



template <class CommType>
typename CMODriver<CommType>::MWReturn CMODriver<CommType>::setup_tasks(vector<MWTask<CommType>*>& init_tasks, bool initial_flag) 
{
if (initial_flag) {
   //
   // TODO: Cleanup tasks that were not completed
   // 
   }

//
// List the new tasks in the cmo_driver.tmp file
//
CharString cmd;
cmd = "(cd ";
cmd += working_dir;
cmd += " ; ls *.cmo > cmo_driver.tmp ) > /dev/null 2>&1";
system(cmd.data());
///
/// Setup tasks from the cmo_driver.tmp
///
cmd = working_dir;
cmd += "/cmo_driver.tmp";
ifstream ifstr(cmd.data());

int line=0;
while (ifstr) {
  ifstr >> cmd;
  if (!ifstr) break;

  MWprintf(20,"Read task \"%s\" at line %d.\n", cmd.data(), line);

  BasicArray<CharString> strs = split(cmd,'.');
  strs.resize(strs.size()-1);
  CharString file = join(strs,'.');

  CharString tag,ifile,ofile;
  tag = strs[strs.size()-2];
  ifile << file;
  ifile += ".in";
  ofile << file;
  ofile += ".out";

  CharString oldtmp = working_dir;
  oldtmp += "/";
  oldtmp += cmd;
  CharString newtmp = working_dir;
  newtmp += "/";
  newtmp += ifile;
  rename(oldtmp.data(), newtmp.data());
  MWprintf(80,"Rename taskfile \"%s\" to \"%s\".\n", oldtmp.data(), newtmp.data());

  CMOTask<CommType>* task = new CMOTask<CommType>(this, RMC, tag, ifile, ofile);
  init_tasks.push_back(task);
  MWprintf ( 20, "Added the task %p\n", task );
  line++;
  }

return OK;
}



template <class CommType>
typename CMODriver<CommType>::MWReturn CMODriver<CommType>::act_on_completed_task( MWTask<CommType> *t ) 
{
CMOTask<CommType>* tf = dynamic_cast<CMOTask<CommType>*> ( t );

MWprintf(0, "-----------------------------------------------------------");
MWprintf(0,"\nCOMPLETED %d ", tf->number);
MWprintf(0,"%s ", tf->taskcmd.data());
MWprintf(0,"%s ", tf->taskinfile.data());
MWprintf(0,"%s ", tf->taskoutfile.data());
	
return OK;
}


#endif
