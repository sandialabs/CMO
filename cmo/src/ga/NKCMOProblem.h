#include "contactmap.h"

class NKCMOProblem{

private:

 ContactMap aContactMap1;
 ContactMap aContactMap2;
 int        interval;
 int        exponent;

public:

 NKCMOProblem()
	: interval(0), exponent(0) {}

 NKCMOProblem(char * fileName1, char * fileName2)
  {
    ContactMap tmpM1;
    ContactMap tmpM2;

    cout<<"Loading ContactMap for:"<<fileName1<<endl;
    tmpM1 = ContactMap(fileName1);
    tmpM1.doRead(fileName1); 

    cout<<"Loading ContactMap for:"<<fileName2<<endl;
    tmpM2 = ContactMap(fileName2);
    tmpM2.doRead(fileName2); 
    
    
    if(tmpM1.getNumberOfResidues()>tmpM2.getNumberOfResidues())
      {
	aContactMap1 = tmpM2;
	aContactMap2 = tmpM1;
      }
    else
      {
	aContactMap1 = tmpM1;
	aContactMap2 = tmpM2;
      }

      interval =aContactMap2.getNumberOfResidues();
      exponent =aContactMap1.getNumberOfResidues();
      
  }

 NKCMOProblem(ContactMap& tmpM1, ContactMap& tmpM2)
	{initialize(tmpM1, tmpM2);}

 void initialize(ContactMap& tmpM1, ContactMap& tmpM2)
    {
    if(tmpM1.getNumberOfResidues()>tmpM2.getNumberOfResidues())
      {
	aContactMap1 = tmpM2;
	aContactMap2 = tmpM1;
      }
    else
      {
	aContactMap1 = tmpM1;
	aContactMap2 = tmpM2;
      }

    interval =aContactMap2.getNumberOfResidues();
    exponent =aContactMap1.getNumberOfResidues();
    }
      

 int getInterval()
  {
    return interval;
  }

 int getExponent()
  { 
    return exponent;
  }

  ContactMap getContactMap1()
  {
    return aContactMap1;
  }


  ContactMap getContactMap2()
  {
    return aContactMap2;
  }

};
 
