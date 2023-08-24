#include "contactmap.h"
#include "NKCMOProblem.h"
#include "fitnessFunction.cpp"



double alignValue(int* x, int n)
{/* This function assumes that the contact map of the shortest protein is in aContactMap1 and it
actually thinks that the values on x are indexed by the residues in the shortest protein for
which the aContactMap1 keeps the contact map.*/

double ans=0;
Contact mappedContact;

/* code to compute the value of an alignament */

ContactMap* aContactMap1= &(aProblem.getContactMap1());
ContactMap* aContactMap2= &(aProblem.getContactMap2());


set<Contact>::iterator  it  = aContactMap1->begin();
set<Contact>::iterator  end = aContactMap2->end();


//for(cm1It->begin();cm1It->end();cm1It++)
for(;it!=end;it++)
  {
    /* we find out which is the contact in the second contactMap that is mapped by x, the alignment, to
       the current contact */
    //  mappedContact=Contac(x[cm1It->left()],x[cm1It->right()]);
    // if(   aContactMap2->isContact(x[cm1It->left()],x[cm1It->right()]) )
 if(   aContactMap2->isContact(x[it->left()],x[it->right()]) )
   {/* do we need to check for the reverse contact as well?? I think not */
    ans+=1;
   }
  }

return ans;
}
