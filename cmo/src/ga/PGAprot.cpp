//
// PGAprot.cpp
//

#ifdef __GNUC__
#pragma implementation "PGAprot.h"
#endif

//#include "IntVector.h"
#include <utilib/Basic2DArray.h>
#include "PGAprot.h"


int findRight(int i,int end, GApoint_int &aPT);
int findLeft(int i,int end, GApoint_int &aPT);

PGAprot::PGAprot(RNG* rng_)  : PGAbase(rng_), PGAint(rng_), EAint_ops()
{
mutation_type=1;
crossover_type=1;

xover_rate = 0.0;
mutation_rate = 1.0;
innovation_rate = 0.0;
ls_first = true;
elitist_replacement (1,0);
}




void PGAprot::set_myProblem(IntOptProblem* problem_)
{
  myProblem = problem_;
}

void PGAprot::set_mutation_type(const char* name)
{
if (strcmp(name,"sliding") == 0) 
  mutation_type=1;
else if (strcmp(name,"random_edge") == 0)
  mutation_type=2;
else
  ErrAbort(errmsg("PGAprot::set_mutation_type -- unknown mutation type \"%s\": known types are sliding and random_edge.\n",name));
}




void PGAprot::set_xover_type(const char* name)
{
if (strcmp(name,"original") == 0) 
  crossover_type=1;
else if (strcmp(name,"sparse") == 0)
  crossover_type=2;
else if (strcmp(name,"best_parent") == 0)
  crossover_type=2;
else if (strcmp(name,"dpx") == 0)
  crossover_type=4;
else
  ErrAbort(errmsg("PGAprot::set_xover_type -- unknown sover type \"%s\": known types are original, sparse,  best_parent and dpx.\n",name));
}



void PGAprot::set_innovation_rate(double rate)
{ innovation_rate = rate; }




void PGAprot::set_helpersNumber(int aNum)
{ helpersNum=aNum; }



int PGAprot::write(ostream& os)
{
PGAint::write(os);
os << mutation_type << endl;
os << crossover_type << endl;
return OK;
}


void PGAprot::reset() 
{
PGAint::reset(); 
//
// Do more here???
//
set_myProblem(get_problem());
}



void PGAprot::set_op_rng(RNG* rng_)
{
EAint_ops::set_op_rng(rng_);
urand.generator(rng_);
}



/* We implement here two crossovers for the contact map alignaments.
Crossover type 1: like Brian's crossover.
Crossover type 2: like Brian's crossover but without refilling of gaps.
Crossover type 3: no crossover but SIM is enabled .
We assume that an individual that represents an alignament will be made
by a vector of integer values. Because both proteins don't necesary share the
same length we encode the shortest protein and each allele value can go from
0 to (the length of the longest protein - 1)
We introduce here the SIM mechanism as described in "Emergence of profitable search strategies" ,
proceedings of the GECCO2001 conference to inherit the memes
Crossover type 4: is a DPX crossover with steepest ascend completion.
*/

void PGAprot::apply_xover(GA_individual& parent1_, GA_individual& parent2_,
                        GA_individual& child_)
{

 if (parent1_ == parent2_)
   {
     /* we do nothing if both parents' genotype are the same */
     child_.copy(parent1_);
     child_.Fstat = 1.0;
     DEBUGPR(100, ucout<<"identical parents"<<endl;)
     return;
   }

 GApoint_int& p1 = Genotype(parent1_);
 GApoint_int& p2 = Genotype(parent2_);
 GApoint_int& ch = Genotype(child_);
 IntVector lower, upper;
 domain_fn.get_bounds(lower,upper);
 int a,b,i,k;
 int lambda1,lambda2;
 int leftmostResidue;
 int rightmostResidue;


 for (i=0;i<nvars()-1;i++)
   {/* we initialize the vector */
     ch.vec[i]=-1;

   }

 DEBUGPR(100,ucout <<"---------"<<endl;)
 
 /*SIM mechanism: if both parents share the same meme then it is inherited to the offspring
   , if the parent's memes are different then we inherit the meme of the fittest parent.
   If none of those criterias are valied, i.e. memes are different but fitnesses are equivalent
   then a random choice is made */
   if(p1.vec[nvars()-1]==p2.vec[nvars()-1])
     {/* both parents have the same meme so we inehrit it */
       ch.vec[nvars()-1]=p1.vec[nvars()-1];
       DEBUGPR(100,ucout<<"SIM:both memes equal"<<endl;)
     }
   else
    if(parent1_.eval()==parent2_.eval())
      {/* we need to do a random choice */
	DEBUGPR(100, ucout<<"SIM: randomly selecting ";)
	urand.low(0);
	urand.high(1);
	if(urand()==0)
	  {
	    ch.vec[nvars()-1]=p1.vec[nvars()-1];
	    DEBUGPR(100, ucout <<" p1"<<endl;)
	  }
	else
	  {
	    ch.vec[nvars()-1]=p2.vec[nvars()-1];
	    DEBUGPR(100, ucout <<" p2"<<endl;)
	  }
      }
    else
      if(parent1_.eval()<parent2_.eval())
	{ DEBUGPR(100, ucout<<"SIM: p1 better than p2"<<endl;)
	    ch.vec[nvars()-1]=p1.vec[nvars()-1];
	}
      else
	{ DEBUGPR(100, ucout<<"SIM: p2 better than p1"<<endl;)
	    ch.vec[nvars()-1]=p2.vec[nvars()-1];
	}

   DEBUGPR(100, ucout<<"-------"<<endl;)
   DEBUGPR(100, ucout<<" SIM assigned at the beggining:"<<ch.vec[nvars()-1]<<endl;)
   DEBUGPR(100, ucout <<"---------"<<endl;)



 if((crossover_type==1)||(crossover_type==2))
 {/*Brian's paper crossover(type 1)  and the sparse version(type 2)*/

    urand.low(0);
    urand.high(nvars()-1-1);
    lambda1 = urand();
    urand.low(0);
    urand.high(nvars()-1-2);
    lambda2 = urand();
    
  if (lambda2 >= lambda1)
   lambda2++;	// This keeps lambda2 unif on nvars()-1-1 other points
  else
  {
   int tmp;
   tmp = lambda1; lambda1 = lambda2; lambda2 = tmp;
  }
  // lambda1 *= crossover.info.coord.block_size;
  //lambda2 *= crossover.info.coord.block_size;         

  /* we copy to child the portion starting in lambda1, ending in lambda2 from parent p1 */

  for (i=lambda1;i<=lambda2;i++)
    {
      ch.vec[i]=p1.vec[i];
    }


  /* We complete child with as many alignaments from parent 2 as are possible */
   /*compute first the leftmost and rightmost alignaments in P1*/
   leftmostResidue=nvars()-1;
   rightmostResidue=-1;
   for(i=lambda1;i<=lambda2;i++)
     {
       if ((ch.vec[i]<leftmostResidue)&&(ch.vec[i]>-1))
       {  
        leftmostResidue=ch.vec[i];
       }
       if (ch.vec[i]>rightmostResidue)
       {
        rightmostResidue=ch.vec[i];
       }
     }
   /* we complete the left part by inserting any alignament from p2 that is not greater nor equall to the
      leftmost alignament from p1. We asume that both p1 and p2 are VALID alignaments: biyections and
      no crossing lines */
   for (i=0;i<lambda1;i++)
     {
       //      if( ( (p2.vec[i]<leftmostResidue)&&(leftmostResidue<nvars()-1) ) ||  (leftmostResidue>=nvars()-1)   )
       if(p2.vec[i]<leftmostResidue )
	{/* we insert the alignament */
	  ch.vec[i]=p2.vec[i];
	}
     }

   /* we complete the right part by inserting any alignament from p2 that is not smaller nor equall than the
      rightmost alignament from p1. We asume that both p1 and p2 are VALID alignaments: biyections and
      no crossing lines */
   for (i=lambda2+1;i<nvars()-1;i++)
     {
       //      if( ((p2.vec[i]>=rightmostResidue)&&(rightmostResidue>-1)) || (rightmostResidue<0))
      if( p2.vec[i]>rightmostResidue)
	{/* we insert the alignament */
	  ch.vec[i]=p2.vec[i];
	}
     }
   if(crossover_type==1)
     {
       /* we need to try to complete whatever free residue we find in child. Supose i is free in child
	  then we see to whom i-k and i+k are aligned with smallest possible k.
	  Supose i-k is aligned to a and i+k to b then
	  i can be aligned to anything between a and b. We need first of course to figure out k */
       for (i=1;i<=nvars()-1-1;i++)
	 {
	   if(ch.vec[i]==-1)
	     {
	       /* we initialize a and b in such a way that it spans the whole range of the longer protein*/
	       
	       a=lower[0];/* all the nvars()-1 got the same bounds so we just peak the 0th one */
	       b=upper[0]; 
	       for(k=i-1;k>=0;k--)
		 {
		   if (ch.vec[k]>-1)
		     {
		       a=ch.vec[k];
		       break;
		     }
		 }
	       for(k=i+1;k<nvars()-1;k++)
		 {
		   if (ch.vec[k]>-1)
		     {
		       b=ch.vec[k];
		       break;
		     }
		 }/* at this point we already know how much we can span in the big protein */
	       if((b-a)>1)
		 {/* there is room for completion */
		   urand.low(1);
		   urand.high(b-a-1);
		   ch.vec[i]= a+urand();
	  
		 }
	       DEBUGPR(100, ucout<<"COMPLETING("<<i<<") with "<<ch.vec[i]<<", a="<<a<<" b="<<b<<endl;)
	     }
	 }
       DEBUGPR(100, ucout<<endl;)

       /* we need to take special precautions with the ends of the intervals: vec[0] and vec[nvars()-1-1]*/
       if(ch.vec[0]==-1)
	 {/* we need to try to complete the leftmost alignament*/
	   a=upper[0];
	   for ( k=1;k<nvars()-1;k++)
	     {
	       if (ch.vec[k]>-1)
		 {
		   a=ch.vec[k];
		   break;/*get out of the for loop*/
		 }
	     }
	   if(a>=1)
	     {
	       urand.low(0);
	       urand.high(a-1);
	       ch.vec[0]= urand();
	     }
	 }

       if(ch.vec[nvars()-1-1]==-1)
	 {/* we need to try to complete the rightmos alignament*/
	   DEBUGPR(100,ucout<<"pos "<<nvars()-1-1<<" holds:"<<ch.vec[nvars()-1-1]<<endl;)
	   b=lower[0];
	   for (k=nvars()-1-2;k>=0;k--)
	     {
	       if (ch.vec[k]>-1)
		 {
		   b=ch.vec[k];
		   break;/*get out of the for loop*/
		 }
	     }
	   if((upper[0])-b>=1)
	     {
	       urand.low(b+1);
	       urand.high(upper[0]);
	       ch.vec[nvars()-1-1]= urand();
	     }

	 }


     }/* of if(crossover_type==1) */

 }/* End Brian's crossover */
 else
 if(crossover_type==3)
   {/* basically we copy as it is the fittest parent, no effective crossover */
     if(parent1_.eval()<parent2_.eval())
       {
	 for (i=0;i<nvars()-2;i++)
	   {
	     ch.vec[i]=p1.vec[i];
	   }
       }
     else
       {
	 for (i=0;i<nvars()-2;i++)
	   {
	     ch.vec[i]=p2.vec[i];
	   }
       }
	 
   }
else
if (crossover_type==4)
  {
   /* DPX crossover */

   Basic2DArray<int> forbiden(3,nvars()-2);
   /* 
   int forbiden[3][nvars()-2]; 		// This syntax is illegal
      forbiden[0][*] contains the id of the residue.
      forbiden[1][*] contains the mapped id of parent1.
      forbiden[2][*] contains the mapped id of parent2.
   */
   Basic2DArray<int> mandatory(2,nvars()-2);
   /*
   int mandatory[2][nvars()-2];
    mandatory[0][*] contains the id of the residue.
    mandatory[1][*] contains the mandatory (common to both parents) map for residue in pos i 
   */

  int indexF, indexM,j;





  /* we fill forbiden and mandatory */
  indexM=0;
  indexF=0;
  for(i=0;i<nvars()-2;i++)
   {
     if( ((p1.vec[i]==p2.vec[i]) && (p1.vec[i]>-1)) ||  ((p1.vec[i]==-1) && (p2.vec[i]>-1)) || ((p1.vec[i]>-1) && (p2.vec[i]==-1)) )
       {
	 mandatory[0][indexM]=i;
	 if(p1.vec[i]>-1)
         {
	
	   mandatory[1][indexM]=p1.vec[i];
	  
	 }
	 else
         {
	   mandatory[1][indexM]=p2.vec[i];
          }
	 indexM++;
       }
     else
       {
	 forbiden[0][indexF]=i;
	 forbiden[1][indexF]=p1.vec[i];
	 forbiden[2][indexF]=p2.vec[i];
	 indexF++;
       }

   }
  DEBUGPR(100,
  ucout<<"+++++++++++++++++++++++++++++++++++"<<endl;
  ucout <<"iF="<<indexF<<" iM="<<indexM<<endl;
  for (j=0;j<indexF;j++)
    {
      ucout <<"F : r="<<forbiden[0][j]<<" to "<<forbiden[1][j]<<" or "<<forbiden[2][j]<<endl;
    }
  for (j=0;j<indexM;j++)
    {
      ucout <<"M : r="<<mandatory[0][j]<<" to "<<mandatory[1][j]<<endl;
    }
  )
  /* we copy into child the mandatory ones */
  int lIndex, rIndex;

  for(j=0;j<indexM;j++)
    {
      lIndex = findLeft(mandatory[0][j],0,ch);
      rIndex = findRight(mandatory[0][j],nvars()-2,ch);
      if(( (lIndex==-1)||((lIndex>-1)&&(ch.vec[lIndex]<mandatory[1][j])) ) && ( (rIndex==-1)||((rIndex>-1)&&(ch.vec[rIndex]>mandatory[1][j])) ) )
	{
	  ch.vec[mandatory[0][j]]=mandatory[1][j];

	}

    }
  DEBUGPR(100,
  for (j=0;j<nvars()-2;j++)
    {
      ucout <<ch.vec[j]<<",";
    }
  ucout <<endl;
  )
  /* we fill in the gaps trying to avoid the forbiden maps */
  // desde-aqui
  int triesCounter;
  int left;
  int right;
  int lbound;
  int rbound;
  bool assigned;
  for(i=0;i<indexF;i++)
    {
      triesCounter=0;
      assigned = false;
      lbound = 0;
      rbound = 0;

       	  left = findLeft(forbiden[0][i],0,ch);
	  right= findRight(forbiden[0][i],nvars()-2,ch);
	  if(left==-1)
	    {
	      lbound=0;
	    }
	  else
	    {
	      lbound = ch.vec[left]+1;
	    }
			   
	  if(right==-1)
	    {
	      rbound=nvars()-2;
	    }
	  else
	    {
	      rbound = ch.vec[right]-1;
	    }

	  if(rbound-lbound>1)
	    {  //hasta-aqui 
	      /* 
		 urand.low(lbound);
		 urand.high(rbound);
		 newMap= urand();
	      	      
		 if((newMap!=forbiden[1][i])&&(newMap!=forbiden[2][i]))
		 {
		 ch.vec[forbiden[0][i]]=newMap;
		 assigned = true;
		
		 }
	      else
		{
		  triesCounter++;
		  }
	      */
	      //desde-aqui	
	      bestFillMove(ch,forbiden[0][i],lbound,rbound,forbiden[1][i],forbiden[2][i]);
	      triesCounter++;
	      assigned = true;
	    }
	  else
	    {
	      ch.vec[forbiden[0][i]]=-1;
	    }
    
    }// hasta-aqui
  DEBUGPR(100,ucout<<"-----------------------------------"<<endl;)
  }/* end of DPX crossover */

 if((!(ch==p1))&& (!(ch==p2)) )
   {/* we update the offspring */
    child_.reset();
   }


 DEBUGPR(100, ucout<<" SIM assigned at the end:"<<ch.vec[nvars()-1]<<endl;)
 DEBUGPR(100, ucout<<"-------"<<endl;)

  DEBUGPR(4,
   ucout << "Parent 1    : " << parent1_ << "\n";
   ucout << "Parent 2    : " << parent2_ << "\n";
   ucout << "Child       : " << child_   << "\n";
   )
 

    }



/* Need to double check whether rightmost and leftmost always get assigned */
void PGAprot::apply_mutation(GA_individual& point, int  parent_ndx )
{
 //
 // Ignore for now
 //
 //if (mutation.per_bit_rate == 0.0)
 //   return;

 //
 // Initial Information
 //
 IntVector lower, upper;
 domain_fn.get_bounds(lower,upper);
 GApoint_int& pt = Genotype(point);
 GApoint_int  originalPt;
 originalPt = Genotype(point);
 bool flag=false;
 bool doIt=false;
 int k;

 /* we perform the innovation of memes mutation phase */
 urand.low(0);
 urand.high(10000);
 float innoRND=(float)(urand()/10000.0);
 DEBUGPR(100, ucout<<"IR="<<innovation_rate<<" RND="<<innoRND;)
 if(innoRND<innovation_rate)
   {/* we innovate the meme */
    urand.low(0);
    urand.high(helpersNum);
    DEBUGPR(100, ucout <<"innovating meme "<<pt.vec[nvars()-1]<<" with:";)
    pt.vec[nvars()-1]=urand();    
    DEBUGPR(100, ucout <<pt.vec[nvars()-1]<<endl;)

   }
else
  {
    DEBUGPR(100, ucout << "NO innovating"<<endl;)
  }
 DEBUGPR(100, ucout <<"-----------"<<endl;)
 if(mutation_type==1)
   {/* sliding mutation */
     int  start; /* the variable where the sliding window  starts; shold be 0<=start<=nvar-2          */
     int  end;   /* the variable where the sliding window  ends; should be start<end<nvar-1           */
     int dir;  /* boolean variable that specifiying the direction of the sliding window.
		   If dir==0 then we move to the left (towards the 0 variable).
		   If dir==1 then we move to the right (towards the nvars()-1 variable). */
     int  delta; /* how much are we going to slide the window. 
		    Must be 1<=delta<=vec[start] if dir==true.
		    Must be 1<=delta<=upper[0]-vec[end] if dir==false;*/

     urand.low(0);
     urand.high(nvars()-1-2);
     start=urand();
     urand.low(start+1);
     urand.high(nvars()-1-1);
     end=urand();
     urand.low(0);
     urand.high(1);
     dir=urand();


     if (dir==0)
       {/* sliding left*/
	 urand.low(1);
	 for(k=start;k<=end;k++)
	   {
	     if(pt.vec[k]>-1)
	       break;
	   }
	 if(k<=end)
	   {/* we can  apply mutation because there is at least one
               alignament in the range [start,end] to slide left*/
	     doIt= true;
	     urand.high(pt.vec[k]);
	     delta=urand();
	   }

       }
     else
       {/* we slide right */
	 urand.low(1);
	 for(k=end;k>=start;k--)
	   {
	     if(pt.vec[k]>-1)
	       break;
	   }
	 if(k>=start)
	   {/* we can  apply mutation because there is at least one
               alignament in the range [start,end] to slide left*/
	     doIt= true;
	     urand.high(upper[0]-pt.vec[k]);
	     delta=urand();
	   }

       }

     if(doIt)
       {
	 DEBUGPR(100, ucout <<"----------------"<<endl;)
	 /*   start = 0;
	 end   = 4;
	 dir   = 0;
	 delta = 2;*/
	 DEBUGPR(100, ucout <<start<<"->"<<end<<" , "<<dir<<","<<delta<<endl;)
	 slidingMove(pt,start,end,dir,delta);
	 if(!(originalPt==pt))
	   {
	     flag = true;
	   }
       }
 }/* end of sliding mutation */
 else
   if (mutation_type==2)
     {/* we randomly assigned a new edge between an unocupaied vertex an any vertex of the second protein. */
      /* we then restore feasibility by deleting any link that as a result is not feasible any more */
       int start;
       int end;       
       int i;
       int j;
       int tmp;
       BasicArray<int> perm(nvars()-1);
       /* do a random  permutation of the vertices */
     
       for(i=0;i<nvars()-1;i++)
	 {
	   perm[i]=i;
	 }
       for(i=0;i<nvars()/2;i++)
	 {
	  
	   urand.low(0);
	   urand.high(nvars()-2);
	   start=urand();
	   urand.low(0);
	   urand.high(nvars()-2);
	   end=urand();
	   tmp=perm[start];
	   perm[start]=perm[end];
	   perm[end]=tmp;
	 }
       /*we find an empty slot */
       i=0;
       while((pt.vec[perm[i]]>-1)&&(i<nvars()-2))
	 {
	   i++;
	 }
       if(i<nvars()-2)
	 {
	   /* we mutate it */
	   urand.low(0);
	   urand.high(upper[0]);
	   end=urand();
	   pt.vec[perm[i]]=end;
       
	   /* we delete everything that is not compatible */
	   for(j=0;j<perm[i];j++)
	     {
	       if(pt.vec[j]>=end)
		 pt.vec[j]=-1;
	     }
	   for(j=perm[i]+1;j<nvars()-2;j++)
	     {
	       if(pt.vec[j]<=end)
		 pt.vec[j]=-1;
	     }
	   flag=true;
	 }
     }/*end of mutation type =2 */



 //
 // If the point was modified, then reset ... might do this differently in your code.
 //
if (flag)
  {
    DEBUGPR(100, ucout<<"The original point:"<<originalPt<<endl;)
    point.reset();
    DEBUGPR(100, ucout<<"The new      point:"<<pt<<endl;)

  }
DEBUGPR(100, ucout <<"----------------"<<endl;)


}



void PGAprot::slidingMove(GApoint_int& pt, int start, int end, int dir, int delta)
{

 int a,b,i,k;
 IntVector lower, upper;
 domain_fn.get_bounds(lower,upper);
 int leftmost, rightmost;
 bool first=true;


  /* first we sum to every pt.vec[i] +/- delta but maintaining feasibility WITHIN the sliding window*/

     if(dir==0)
       { /* we move left */
	 for (i=start;i<=end;i++)
	   {
	     if( (pt.vec[i]>=delta) )
	       {
		 int t=findLeft(i,start,pt);
		 if(t>-1)
		   {
		     if((i>start)&&( (pt.vec[i]-delta)>pt.vec[t]))
		       {
			 pt.vec[i]= pt.vec[i]-delta;
			 if(first)
			   {
			     leftmost=i;
			     first=false;
			   }
		       }
		     else if (i==start)
		       {
			 pt.vec[i]= pt.vec[i]-delta;
			 if(first)
			   {
			     leftmost=i;
			     first=false;
			   }
		       }
		   }
		 else
		   {
		       pt.vec[i]= pt.vec[i]-delta;
		       if(first)
			   {
			     leftmost=i;
			     first=false;
			   }
		   }
	       }
	     else
	       {
		 DEBUGPR(100,ucout<<"WARNING SLIDING LEFT!"<<endl;)
	       }
	   }
       }
     else
       {  /* we move right */
	 for(i=end;i>=start;i--)
	   {
	     if( (pt.vec[i]>-1) && (pt.vec[i]<=upper[0]-delta) )
	       {
		 int t=findRight(i,end,pt);
		 if(t>-1)
		   {
		     if((i<end)&&(pt.vec[i]+delta<pt.vec[t]))
		       {
			 pt.vec[i]=pt.vec[i]+delta;
			 if(first)
			   {
			     rightmost=i;
			     first=false;
			   }
		       }
		     else if(i==end)
		       {
			 pt.vec[i]=pt.vec[i]+delta;
			 if(first)
			   {
			     rightmost=i;
			     first=false;
			   }
		       }
		   }
		 else
		   {
		       pt.vec[i]=pt.vec[i]+delta;
		       if(first)
			 {
			   rightmost=i;
			   first=false;
			 }
		   }

	       }
	     else
	       {
		 DEBUGPR(100, ucout<<"WARNING SLIDING RIGHT!"<<endl;)
	       }
	   }
       }




 /* we check now whether we cross any alignament that was in place before the move OUTSIDE the sliding window*/
 if(dir==0)
   {
     for(int i=0;i<start;i++)
       {
	 if (pt.vec[i]>=pt.vec[leftmost])
	   {/* we delete the alignament 'cos it was crossed */
	     pt.vec[i]=-1;
	   }
       }
   }
 else
   {
     for(i=end+1;i<nvars()-1;i++)
       {
	 if (pt.vec[i]<=pt.vec[rightmost])
	   {/* we delete the alignament 'cos it was crossed */
	     pt.vec[i]=-1;
	   }
       }
   }




 /* we need to try to complete whatever free residue we find in child. Supose i is free in child
      then we see to whom i-k and i+k are aligned for the smalles k. Supose i-k is aligned to a and i+k to b then
      i can be aligned to anything between a and b. We need first of course to figure out k */
   for (i=1;i<(nvars()-1-1);i++)
     {
      if(pt.vec[i]==-1)
	{
	  /* we initialize a and b in such a way that it spans the whole range of the longer protein*/
	  a=lower[0];/* all the nvars()-1 got the same bounds so we just peak the 0th one */
	  b=upper[0]; 
          for( k=i-1;k>=0;k--)
	    {
	      if (pt.vec[k]>-1)
		{
		  a=pt.vec[k];
		  break;		/*get out of the for loop*/
		}
	    }
          for( k=i+1;k<nvars()-1;k++)
	    {
	      if (pt.vec[k]>-1)
		{
		  b=pt.vec[k];
		  break;		/*get out of the for loop*/
		}
	    }
          /* at this point we already know how much we can span in the big protein */
          if((b-a)>1)
	  {/* there is room for completion */
           urand.low(1);
	   urand.high(b-a-1);
           pt.vec[i]= a+urand();
	  }

	}
     }



   /* we need to take special precautions with the ends of the intervals: vec[0] and vec[nvars()-1-1]*/
   if(pt.vec[0]==-1)
     {/* we need to try to complete the leftmost alignament*/
       a=upper[0];
       for ( k=1;k<nvars()-1;k++)
	 {
	   if (pt.vec[k]>-1)
	     {
	       a=pt.vec[k];
	       break;/*get out of the for loop*/
	     }
	 }
       if(a>=1)
	 {
	   urand.low(0);
	   urand.high(a-1);
	   pt.vec[0]= urand();
	 }
     }

   if(pt.vec[nvars()-1-1]==-1)
     {/* we need to try to complete the rightmos alignament*/
       DEBUGPR(100,ucout<<"pos "<<nvars()-1-1<<" holds:"<<pt.vec[nvars()-1-1]<<endl;)
       b=lower[0];
       for (k=nvars()-1-2;k>=0;k--)
	 {
	   if (pt.vec[k]>-1)
	     {
	       b=pt.vec[k];
	       break;/*get out of the for loop*/
	     }
	 }
        if((upper[0])-b>=1)
	  {
	    urand.low(b+1);
	    urand.high(upper[0]);
	    pt.vec[nvars()-1-1]= urand();
	  }

     }




}


int findLeft(int i,int start, GApoint_int & aPT)
{

 int f;
 int res=-1;
 bool found=0;

 for(f=i-1;(f>=start)&&(!found);f--)
   {
     if (aPT.vec[f]>-1)
       {
	 res = f;
	 found=1;

       }
   }

 return res;
}



int findRight(int i,int end, GApoint_int & aPT)
{

 int f;
 int res=-1;
 bool found=0;

 for(f=i+1;(f<end)&&(!found);f++)
   {
     if (aPT.vec[f]>-1)
       {
	 res = f;
	 found = 1;
       }
   }

 return res;
}






void  PGAprot::random_init_alignament(IntVector& myVec,int fs,int fe, int ts, int te)
{
/* the format for this function is:

myVec: an IntVector that holds the alignamentsfrom the first protein to the second(largest) one.
fs   : the Starting residue of the   subproblem to be randomly initialized between ts and te
fe   : the Ending   residue of the   subproblem to be randomly initialized between ts and te   
ts   : the starting residue in the long protein to whom to align residues between fs an fe
te   : the ending   residue in the long protein to whom to align residues between fs an fe

when the function is called the first time the whole problem is setted as:
fs=0 (we assume that we are counting from 0)
fe=number of residues in the shortest protein-1
ts=0 (the 0 residue of the long protein)
te=number of residues in the longest protein-1

The algorithm then takes an interval of the short protein ( represented by fe-fs) 
and selects at random a residue. This residue is then randomly mapped to the the interval
(represented by te-ts) on the longest protein. This alignament effectively splits the problem into
two , s-left and s-right, subproblems. We call the function recursively on s-left and s-right.
A subproblem is a base case (no more recursive calls are done) if one or more of the following are
true:

fe-fs<=1 meaning that there are no more residues of the short protein to align in this subproblem
te-ts<=1 meaning that there are no more residues on the long protein with whom a residue of the
         short protein can align.

*/



//
// WEH - is this correct?
//
//if( ((fe-fs)<=1) || ((te-ts)<=1) )
//
if( ((fe-fs)<0) || ((te-ts)<0) )
  {/* we need return. The initialization of the subproblem was accomplished */
    return;
  }


/* we need to assign one residue and then recurse */
urand.low(fs);
urand.high(fe);
int randomResidue= urand();

urand.low(ts);
urand.high(te);
myVec[randomResidue]= urand();

DEBUGPR(100,
    ucout<<endl;
    ucout<<"R["<<fs<<","<<fe<<"] FROM:"<<randomResidue<<endl;
    ucout<<"R["<<ts<<","<<te<<"]   TO:"<<myVec[randomResidue]<<endl;
    ucout<<endl;
)

/* recurse the left subproblem */
random_init_alignament(myVec,fs,randomResidue-1,ts,myVec[randomResidue]-1);

/* recurse the right subproblem */
random_init_alignament(myVec,randomResidue+1,fe,myVec[randomResidue]+1,te);

    


}





void PGAprot::randomize()
{ 
if (nvars() == 0)
   return;

IntVector lower, upper;
domain_fn.get_bounds(lower,upper);
int meme;

DEBUGPR(100, 
ucout<<"INITIALIZING"<<endl;
ucout<<"nvars="<<nvars()<<" , interval["<<lower[0]<<","<<upper[0]<<"],["<<lower[nvars()-1]<<","<<upper[nvars()-1]<<endl;
)

for (unsigned int i=0; i<popsize(); i++) {
  DEBUGPR(100,ucout<<"+++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;)
  for(int j=0;j<nvars();j++)
    {/* we disassociate residue j from any residue in the second(the longest) protein and we put the meme to -1*/
      Genotype(population(i)).vec[j]=-1;
    }
  //
  // WEH
  //
  //random_init_alignament(Genotype(population(i)).vec,0,nvars()-1-1,lower[0],upper[0]);
  random_init_alignament(Genotype(population(i)).vec,0,nvars()-1,lower[0],upper[0]);
  /* we randomly select a meme */
  urand.low(0);
  urand.high(upper[nvars()-1]);
  meme = urand();
  Genotype(population(i)).vec[nvars()-1]=meme;
  DEBUGPR(100,ucout << "In Randomize: "<<"meme range 0-"<<upper[nvars()-1]<<" meme="<<  Genotype(population(i)).vec[nvars()-1];)
  
  population(i).reset();
  }

DEBUGPR(100, ucout<<"Random Initialization finished"<<endl;)

}






/*
This operator tries to include a new alignment in an unocupied position.
To do this it discards all the previous alignments that conflicts with the one being tested.
It chooses th first improvement insertion.
*/
void PGAprot::bestFillMove( GApoint_int& ind,int pos,int start, int end, int forbiden1, int forbiden2)
{
 int j,i;

 IntVector& tempSol=ind.vec;
 REAL tempSolValue;
 REAL    workingSolValue;
 REAL      bestWorkingSolValue;
 int numVars;
 IntVector lower, upper;
 bool improvementFound=false;


 myProblem->get_bounds(lower,upper); 
 numVars= myProblem->nvars() -1 -1;
 myProblem->EvalF(tempSol,tempSolValue);
 

 IntVector workingSol; 
 workingSol.resize(numVars+2);
 IntVector bestWorkingSol;
 bestWorkingSol.resize(numVars+2);

 workingSol<<tempSol;
 bestWorkingSol<<tempSol;

 bestWorkingSolValue = tempSolValue;

 /* we try to complete this position by aligning it to a residue in the other protein
    in such a way as to maximize the alignments but preserving the distance to the 
    parents */
 for (j=start;j<=end;j++)
   { /* we create the alignment */
     if((j!=forbiden1)&&(j!=forbiden2))
      {
	 workingSol[pos]=j;
	 /* now we compute the fitness of this new solution */
	 myProblem->EvalF(workingSol,workingSolValue);
	 if(workingSolValue<bestWorkingSolValue)
	   {
	     improvementFound=true;
	     bestWorkingSolValue = workingSolValue;
	 
	     bestWorkingSol<<workingSol;

	   }

	 
	 workingSol << tempSol;
	 workingSolValue = tempSolValue;
	 
	    
      }
   }

 
 if(improvementFound)
   {
     for(j=0;j<nvars();j++)
       {
	 tempSol[j]=bestWorkingSol[j];
       }
   }
}






