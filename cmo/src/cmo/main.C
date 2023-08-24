

#ifdef USEGRAPHICS
#ifndef _graphics
#include "graphics.H"
#endif
#endif

#include <iostream>
#include "contactmap.H"

using namespace std;

int main()
{
 ContactMap aContactMap1;
 //ContactMap aContactMap2;

 char windowTitle[]="Test 1";
 
char  fileName[200] ="/home/u/nkrasno/PROJECTS/work/CMO/nat1.pdb";
 //char  fileName[200] ="/home/u/nkrasno/PROJECTS/INSTANCES/PDBs/pdb1a15.gr";
 //char  fileName2[200] ="/home/u/nkrasno/PROJECTS/INSTANCES/PDBs/pdb2gda.gr";


 cout<<"Loading ContactMap for:"<<fileName<<endl;
 aContactMap1 = ContactMap(fileName);
 aContactMap1.doRead(fileName); 



 //cout<<"Loading ContactMap for:"<<fileName2<<endl;
 //aContactMap2 = ContactMap(fileName2);
 //aContactMap2.doRead(fileName2); 

 #ifdef _graphics
  NKGraphicPanel aGP1;
  //NKGraphicPanel aGP2;

  /* sizeX,sizeY,posX,posY */
  aGP1= NKGraphicPanel(500,500,500.0,500.0);
  //aGP2= NKGraphicPanel(500,500,500.0,500.0);

  aGP1.NKGPOpenMainWindow(fileName);


  //aGP1.NKGPOpenMainWindow(fileName2);



  // aContactMap2.draw(aGP2,20,50);
  aContactMap1.draw(aGP1,40,250,true);
  aContactMap1.draw(aGP1,40,250,false);

 #endif
 
 for (;;){};


}
