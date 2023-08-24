#include "contactmap.h"
#include "NKCMOProblem.h"




NKCMOProblem* aProblem;

void init_problem(char* filename1, char* filename2, int& interval, int& exponent)
{
aProblem = new NKCMOProblem(filename1, filename2);
interval = aProblem->getInterval();
exponent = aProblem->getExponent();
}



double alignValue(int* x, int n)
{/* This function assumes that the contact map of the shortest protein is in aContactMap1 and it
actually thinks that the values on x are indexed by the residues in the shortest protein for
which the aContactMap1 keeps the contact map.*/

double ans=0;
Contact mappedContact;
int lR,rR;

/* code to compute the value of an alignament */

ContactMap aContactMap1= aProblem->getContactMap1();
ContactMap aContactMap2= aProblem->getContactMap2();


set<Contact>::iterator  it  = aContactMap1.begin();
set<Contact>::iterator  end = aContactMap1.end();


//for(cm1It->begin();cm1It->end();cm1It++)
for(;it!=end;it++)
  {
    /* we find out which is the contact in the second contactMap that is mapped by x, the alignment, to
       the current contact */
    lR=x[it->left()];
    rR=x[it->right()];
    if((lR>-1)&&(rR>-1))
       {
	if(   aContactMap2.isContact(lR,rR) )
	  {/* do we need to check for the reverse contact as well?? I think not */
	    ans+=1;
	  }
      }
  }
return - ans;
}



