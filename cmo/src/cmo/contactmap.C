/* We need to put the graphics part of the code inside a conditional compilation */


#include "contactmap.H"
#include <cstdio>
#include <iostream>
#include <math.h>


using namespace std;

ContactMap::ContactMap() {
  _Threshold        = 0.0;
  _theProteinName   = NULL;
}



ContactMap::ContactMap(char * aP) {
  _Threshold        = 0.0;
  _theProteinName       = aP;
}

ContactMap::ContactMap(double t) {
  _Threshold        = t;
  _theProteinName   =NULL;
}

ContactMap::~ContactMap() {
}





void ContactMap::doRead(char * FILENAME) 
{
 unsigned   a, b;
 FILE *fp;
 char rubish[4][100];
 char rubish2[4][100];
 unsigned numberOfResidues;
 unsigned numberOfContacts;

 if(!(fp=fopen(FILENAME,"r")))
 {
   cout<<"ERROR - Can't open file %s\n"<<FILENAME;
  exit(1);
 }
  _Contacts.clear();
 fscanf(fp, " %i %s %s %s %s \n",&numberOfResidues,rubish[0],rubish[1],rubish[2],rubish[3]);
   cout<<numberOfResidues<<"  "<<rubish[3]<<endl;
 _numberOfResidues = numberOfResidues;
 fscanf(fp, " %i %s %s %s %s \n",&numberOfContacts,rubish2[0],rubish2[1],rubish2[2],rubish2[3]);
 cout<<numberOfContacts<<"  "<<rubish2[3]<<endl;
 _numberOfContacts = numberOfContacts;
 while (!feof(fp))
 {
  fscanf(fp, "%u%u\n",&a,&b);
  cout<<" Inserted contact("<<a<<","<<b<<")\n";
  _Contacts.insert(Contact(a, b));
 }  
 fclose(fp);

}



#ifdef _graphics
void ContactMap::draw(NKGraphicPanel aGP, int startX, int startY,bool up)
{
 ContactMapIterator it;
 unsigned a,b;
 double scaleX= (aGP.NKGPGetXRange()-2*startX)/(_numberOfResidues);

 
  for (int f=0;f<_numberOfResidues;f++)
  {
   aGP.NKGPCircle(f*scaleX+startX, startY, scaleX/4, RED);
  }
  for (it=this->begin(); !(it==this->end()); it++)
  {
   a = it->left();
   b = it->right();
   cout<<"("<<a<<","<<b<<")"<<endl;
   //aGP.NKGPCircle(a*scaleX+startX, startY, scaleX/4, RED);
   //aGP.NKGPCircle(b*scaleX+startX, startY, scaleX/4, RED);
   //   aGP.NKGPArc( ((a+(b-a)/2)*scaleX + startX) ,startY+0,(b+1-a)/2*scaleX,0,1800,GREEN);
   if(up)
     {
       aGP.NKGPArc(startX+(a*scaleX)+scaleX*(b-a)/2,startY+0,ceil(b-a)/2*scaleX,0,1800,GREEN);
     }
   else
     {
       aGP.NKGPArc(startX+(a*scaleX)+scaleX*(b-a)/2,startY+0,ceil(b-a)/2*scaleX,0,-1800,GREEN);
     }
  }
  aGP.NKGPRefresh(); 


   cout<<_theProteinName<<endl;
}
#endif


double
Distance(const double *A, const double *B) {
  double s, r;

  s = A[0] - B[0];   r  = s*s;
  s = A[1] - B[1];   r += s*s;
  s = A[2] - B[2];   r += s*s;

  return(r);
}


