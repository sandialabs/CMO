

#ifdef USEGRAPHICS
#ifndef _graphics
#include "graphics.H"
#endif
#endif

#include <iostream>
#include "contactmap.H"

using namespace std;

int main(int argc, char **argv)
{
 ContactMap aContactMap1;
  
 char  *proteinName= argv[1];
 char  *fileName1 =argv[2];
 char  *fileName2 =argv[3];


 cout<<"Loading ContactMap for:"<<proteinName<<endl;
 aContactMap1 = ContactMap(proteinName);
 aContactMap1.doRead(proteinName); 
 aContactMap1.doWriteAmpleFile(fileName1,fileName2);

}
