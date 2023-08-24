//
// SEMDriver.h
//

#ifndef __SEMDriver_h
#define __SEMDriver_h

#include "SEMTask.h"
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
#include <utilib/ParameterSet.h>

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
class SEMDriver : public MWDriver<CommType>, public utilib::ParameterSet {

public:

  ///
  SEMDriver();

  ///
  void go(int argc, char* argv[]);

  ///
  typename SEMDriver<CommType>::MWReturn get_userinfo( int argc, char *argv[] );

  ///
  typename SEMDriver<CommType>::MWReturn setup_initial_tasks( vector<MWTask<CommType>*>& tasks )
	{return setup_tasks(tasks,true);}

  ///
  typename SEMDriver<CommType>::MWReturn setup_additional_tasks( vector<MWTask<CommType>*>& tasks )
	{return setup_tasks(tasks,false);}

  ///
  typename SEMDriver<CommType>::MWReturn setup_tasks( vector<MWTask<CommType>*>& tasks, bool initial_flag );

  ///
  typename SEMDriver<CommType>::MWReturn act_on_completed_task( MWTask<CommType> * );

  ///
  typename SEMDriver<CommType>::MWReturn pack_worker_init_data( void )
		{return OK;}

  ///
  MWTask<CommType>* allocate_task()
		{return new SEMTask<CommType>(this,RMC);}

  ///
  CharString task_cmd;

  ///
  CharString out_file;

  ///
  CharString working_dir;

  ///
  ostream* ostr;

  ///
  CharString command_setup;

  ///
  CharString command_cleanup;

  ///
  CharString command_tasks;
  
};


//--------------------------------------------------------------------------
//
//
template <class CommType>
void SEMDriver<CommType>::go(int argc, char* argv[])
{
if (command_setup != "") {
   CharString cmd;
   cmd = "(cd ";
   cmd += working_dir;
   cmd += " ; ./";
   cmd += command_setup;
   cmd += " ) > /dev/null 2>&1";
   system(cmd.data());
   }

try {
  MWDriver<CommType>::go(argc,argv);
  }
catch (std::exception& err) {
  cerr << "Termination caught: " << err.what() << endl;
  }

if (command_cleanup != "") {
   CharString cmd;
   cmd = "(cd ";
   cmd += working_dir;
   cmd += " ; ./";
   cmd += command_cleanup;
   cmd += " ) > /dev/null 2>&1";
   system(cmd.data());
   }
}


//--------------------------------------------------------------------------
//
//
template <class CommType>
SEMDriver<CommType>::SEMDriver()
  : MWDriver<CommType>(),
    task_cmd(),
    out_file(),
    working_dir(),
    ostr(0)
{
create_parameter("ofile",out_file,"<str>","<empty>","An optional output file");

create_parameter("command-setup",command_setup,"<str>","<empty>",
	"An optional command to perform setup");

create_parameter("command-cleanup",command_cleanup,"<str>","<empty>",
	"An optional command to perform cleanup");

server_mode=false;
create_parameter("server",server_mode,"<bool>","false",
	"If true, run driver in server mode");

command_tasks = "sem_get_tasks";
create_parameter("command-tasks",command_tasks,"<str>","sem_get_tasks",
	"The name of the script that gets tasks for the sem_driver");
}



//--------------------------------------------------------------------------
//
//
template <class CommType>
typename SEMDriver<CommType>::MWReturn SEMDriver<CommType>::get_userinfo( 
					int argc, char *argv[] ) 
{
//
// Process the command line parameters
//
process_parameters(argc,argv);
if (get_parameter<bool>("help")) {
   write_parameters(cout);
   MWprintf(0,"Commandline Syntax:\n\tsem_driver [<--option> <value>] <working_dir>\n");
   return ABORT;
   }
//
// Setup the working directory
//
if (argc != 2) {
   MWprintf(0,"Commandline Syntax:\n\tsem_driver [<--option> <value>] <working_dir>\n");
   return ABORT;
   }
   
working_dir = argv[1];
//
// Setup the output file
//
if (out_file != "") {
   ostr = new ofstream(out_file);
   if (!(*ostr)) {
      MWprintf(0,"ERROR: Could not open output file \"%s\".\n", out_file.data());
      return ABORT;
      }
   set_MWprintf_ostream(*ostr);
   }
else
   ostr = &cout;

return OK;
}



template <class CommType>
typename SEMDriver<CommType>::MWReturn SEMDriver<CommType>::setup_tasks(vector<MWTask<CommType>*>& init_tasks, bool initial_flag) 
{
if (initial_flag) {
   // Do anything special here?
   }

//
// List the new tasks in the sem_driver.tmp file
//
CharString cmd;
cmd = "(cd ";
cmd += working_dir;
cmd += " ; ./";
cmd += command_tasks;
cmd += " sem_driver.tmp ) > /dev/null 2>&1";
system(cmd.data());
///
/// Setup tasks from the sem_driver.tmp
///
cmd = working_dir;
cmd += "/sem_driver.tmp";
ifstream ifstr(cmd.data());

int line=0;
while (ifstr) {
  ifstr >> cmd;
  if (!ifstr) break;

  MWprintf(20,"Read task \"%s\" at line %d.\n", cmd.data(), line);

  BasicArray<CharString> strs = split(cmd,'.');
  strs.resize(strs.size()-1);
  CharString file = join(strs,'.');

  CharString tag,ofile;
  tag = strs[strs.size()-2];
  ofile << file;
  ofile += ".out";

#if 0
  CharString oldtmp = working_dir;
  oldtmp += "/";
  oldtmp += cmd;
  CharString newtmp = working_dir;
  newtmp += "/";
  newtmp += ifile;
  rename(oldtmp.data(), newtmp.data());
  MWprintf(80,"Rename taskfile \"%s\" to \"%s\".\n", oldtmp.data(), newtmp.data());

#endif

  SEMTask<CommType>* task = new SEMTask<CommType>(this, RMC, tag, cmd, ofile);
  init_tasks.push_back(task);
  MWprintf ( 20, "Added the task %p\n", task );
  line++;
  }
ifstr.close();

return OK;
}



template <class CommType>
typename SEMDriver<CommType>::MWReturn SEMDriver<CommType>::act_on_completed_task( MWTask<CommType> *t ) 
{
SEMTask<CommType>* tf = dynamic_cast<SEMTask<CommType>*> ( t );

MWprintf(0, "-----------------------------------------------------------");
MWprintf(0,"\nCOMPLETED %d ", tf->number);
MWprintf(0,"%s ", tf->taskcmd.data());
MWprintf(0,"%s ", tf->taskinfile.data());
MWprintf(0,"%s ", tf->taskoutfile.data());
	
return OK;
}


#endif
