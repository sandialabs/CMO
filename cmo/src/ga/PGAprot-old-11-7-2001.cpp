//
// PGAprot.cpp
//

#ifdef __GNUC__
#pragma implementation "PGAprot.h"
#endif

#include "DUniform.h" 
#include "PGAprot.h"


static DUniform urand;

PGAprot::PGAprot(RNG* rng_)  : PGAbase(rng_), PGAint(rng_), EAint_ops()
{
 mutation_type=1;
 crossover_type=1;

 xover_rate = 0.0;
 mutation_rate = 1.0;
}



void PGAprot::set_mutation_type(int type)
{
 mutation_type=type;
}






void PGAprot::set_mutation_rate(double rate)
{
 mutation_rate = rate;
}




void PGAprot::set_xover_type(int type)
{
 crossover_type=type;
}




void PGAprot::set_xover_rate(double rate)
{
 xover_rate = rate;
}





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
}



void PGAprot::set_op_rng(RNG* rng_)
{
  // DUniform urand;
 urand.generator(rng_);

}



/* We implement here two crossovers for the contact map alignaments.
Crossover type 1: like Brian' crossover.
Crossover type 2: DPX crossover.
We assume that an individual that represents an alignament will be made
by a vector of integer values. Because both proteins don't necesary share the
same length we encode the shortest protein and each allele value can go from
0 to (the length of the longest protein - 1)
*/

void PGAprot::apply_xover(GA_individual& parent1_, GA_individual& parent2_,
                        GA_individual& child_)
{

 if (parent1_ == parent2_) {
 /* we do nothing if both parents' genotype are the same */
    child_.copy(parent1_);
    child_.Fstat = 1.0;
    return;
    }

 GApoint_int& p1 = Genotype(parent1_);
 GApoint_int& p2 = Genotype(parent2_);
 GApoint_int& ch = Genotype(child_);
 IntVector lower, upper;
 domain_fn.get_bounds(lower,upper);
 int a,b;

 for (int i=0;i<nvars();i++)
   {/* we initialize the vector */
     ch.vec[i]=-1;

   }

 if(crossover_type==1)
 {/*Brian's paper crossover */

   //  int lambda1 = Discretize(rnd(), 1, nvars()/crossover.info.coord.block_size-1);
   //  int lambda2 = Discretize(rnd(), 1, nvars()/crossover.info.coord.block_size-2);
    urand.low(1);
    urand.high(nvars()-1);
    int lambda1 = urand();
    urand.low(1);
    urand.high(nvars()-2);
    int lambda2 = urand();
    
  if (lambda2 >= lambda1)
   lambda2++;	// This keeps lambda2 unif on nvars()-1 other points
  else
  {
   int tmp;
   tmp = lambda1; lambda1 = lambda2; lambda2 = tmp;
  }
  lambda1 *= crossover.info.coord.block_size;
  lambda2 *= crossover.info.coord.block_size;         

  /* we copy to child the portion starting in lambda1, ending in lambda2 from parent p1 */
  /* ch.vec.set_subvec(dstStartPos,length,src,srcStartPos); */
  ch.vec.set_subvec(lambda1,lambda2-lambda1,p1.vec,lambda1);
 
 /* We complete child with as many alignaments from parent 2 as are possible */
   /*compute first the leftmost and rightmost alignaments in P1*/
   int leftmostResidue=nvars();
   int rightmostResidue=0;
   for(int i=lambda1;i<lambda2;i++)
     {
       if (ch.vec[i]<leftmostResidue)
       {  
        leftmostResidue=ch.vec[i];
       }
       if (ch.vec[i]>rightmostResidue)
       {
        rightmostResidue=ch.vec[i];
       }
     }
   /* we complete the left part by inserting any alignament from p2 that is not greater than the
      leftmost alignament from p1. We asume that both p1 and p2 are VALID alignaments: biyections and
      no crossing lines */
   for (int i=0;i<lambda1;i++)
     {
      if(p2.vec[i]<leftmostResidue)
	{/* we insert the alignament */
	  ch.vec[i]=p2.vec[i];
	}
     }

   /* we complete the right part by inserting any alignament from p2 that is not smaller than the
      rightmost alignament from p1. We asume that both p1 and p2 are VALID alignaments: biyections and
      no crossing lines */
   for (int i=lambda2;i<nvars();i++)
     {
      if(p2.vec[i]>rightmostResidue)
	{/* we insert the alignament */
	  ch.vec[i]=p2.vec[i];
	}
     }
   /* we need to try to complete whatever free residue we find in child. Supose i is free in child
      then we see to whom i-1 and i+1 are aligned. Supose i-k s aligned to a and i+k to b then
      i can be aligned to anything between a and b. We need first of course to figure out k */
   for (int i=1;i<nvars()-1;i++)
     {
      if(ch.vec[i]==-1)
	{
	  /* we initialize a and b in such a way that it spans the whole range of the longer protein*/

	  a=lower[0];/* all the nvars() got the same bounds so we just peak the 0th one */
	  b=upper[0]; 
          for(int k=i-1;k>0;k--)
	    {
	      if (ch.vec[k]>-1)
		{
		  a=ch.vec[k];
		  break;
		}
	    }
          for(int k=i+1;k<nvars();k++)
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

	}
     }
   /* we need to take special precautions with the ends of the intervals: vec[0] and vec[nvars()]*/
   if(ch.vec[0]==-1)
     {/* we need to try to complete the leftmos alignament*/
       a=lower[0];
       for (int k=1;k<nvars();k++)
	 {
	   if (ch.vec[k]>-1)
	     {
	       b=ch.vec[k];
	       break;
	     }
	 }
       urand.low(1);
       urand.high(b-a-1);
       ch.vec[0]= a+urand();

     }
   if(ch.vec[nvars()-1]==-1) 
     {/* we need to try to complete the rightmos alignament*/
       b=upper[0];
       for (int k=nvars()-2;k>=0;k--)
	 {
	   if (ch.vec[k]>-1)
	     {
	       a=ch.vec[k];
	       break;
	     }
	 }
       urand.low(1);
       urand.high(b-a-1);
       ch.vec[nvars()-1]= a+urand();

     }




 if((!(ch==p1))&& (!(ch==p2)) )
   {/* we update the offspring */
    child_.reset();
   }



  DEBUGPR(4,
   cout << "Parent 1    : " << parent1_ << "\n";
   cout << "Parent 2    : " << parent2_ << "\n";
   cout << "Child       : " << child_   << "\n";
   )
 }/* End Brian's crossover */

}




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
 GApoint_int  originalPt= Genotype(point);
 bool flag;


 if(mutation_type==1)
   {/* sliding mutation */
     int flag = FALSE;
     int  start; /* the variable where the sliding window  starts; shold be 0<=start<=nvar-2          */
     int  end;   /* the variable where the sliding window  ends; should be start<end<nvar-1           */
     bool dir;  /* boolean variable that specifiying the direction of the sliding window.
		   If dir==true then we move to the left (towards the 0 variable).
		   If dir==false then we move to the right (towards the nvars() variable). */
     int  delta; /* how much are we going to slide the window. 
		    Must be 1<=delta<=vec[start]-1 if dir==true.
		    Must be 1<=delta<=nvars()-vec[end]-1 if dir==false;*/

     urand.low(0);
     urand.high(nvars()-2);
     start=urand();
     urand.low(start+1);
     urand.high(nvars()-1);
     end=urand();
     urand.low(0);
     urand.high(1);
     if(urand()==0)
       {
	 dir = true;
       }
     else
       {
	 dir = false;
       }

     //     dir = ( (urand()==0)? true : false);
     if (dir)
       {/* sliding left*/
	 urand.low(1);
	 urand.high(pt.vec[start]-1);
	 delta=urand();
       }
     else
       {/* we slide right */
	 urand.low(1);
	 urand.high(nvars()-pt.vec[end]-1);
	 delta=urand();
       }
     cout <<"INDIVIDUAL:"<<endl<<pt<<","<<start<<"->"<<end<<","<<dir<<"+"<<delta<<endl;
     slidingMove(pt,start,end,dir,delta);
     if(!(originalPt==pt))
       {
	 flag = true;
       }

   }/* end of sliding mutation */
 //
 // If the point was modified, then reset ... might do this differently in your code.
 //
if (flag)
  {
    cout<<"The original point:"<<originalPt<<", new point:"<<pt;
   point.reset();
  }



}



void PGAprot::slidingMove(GApoint_int& pt, int start, int end, bool dir, int delta)
{

 int a,b;
 IntVector lower, upper;
 domain_fn.get_bounds(lower,upper);

  /* first we sum to every pt.vec[i] +/- delta*/
 for (int i=start;i<=end;i++)
   {
     if(dir)
       { 
	 /* we move left */
	 if (pt.vec[i]-delta<0) cout<<"WARNING!"<<endl;
	 pt.vec[i]-=delta;
       }
     else
       {
	 /* we move right */
	 if (pt.vec[i]+delta>nvars()-1) cout<<"WARNING!"<<endl;
	 pt.vec[i]+=delta;
       }
   }
 /* we check now wheter we cross any alignament that was in place before the move */
 if(dir)
   {
     for(int i=0;i<start;i++)
       {
	 if (pt.vec[i]>pt.vec[start])
	   {/* we delete the alignament 'cos it was crossed */
	     pt.vec[i]=-1;
	   }
       }
   }
 else
   {
     for(int i=end+1;i<nvars();i++)
       {
	 if (pt.vec[i]<pt.vec[end])
	   {/* we delete the alignament 'cos it was crossed */
	     pt.vec[i]=-1;
	   }
       }
   }

 /* we need to try to complete whatever free residue we find in child. Supose i is free in child
      then we see to whom i-1 and i+1 are aligned. Supose i-k s aligned to a and i+k to b then
      i can be aligned to anything between a and b. We need first of course to figure out k */
   for (int i=1;i<(nvars()-1);i++)
     {
      if(pt.vec[i]==-1)
	{
	  /* we initialize a and b in such a way that it spans the whole range of the longer protein*/
	  IntVector lower, upper;
	  domain_fn.get_bounds(lower,upper);
	  a=lower[0];/* all the nvars() got the same bounds so we just peak the 0th one */
	  b=upper[0]; 
          for(int k=i-1;k>0;k--)
	    {
	      if (pt.vec[k]>-1)
		{
		  a=pt.vec[k];
		  break;		/*get out of the for loop*/
		}
	    }
          for(int k=i+1;k<nvars();k++)
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
   /* we need to take special precautions with the ends of the intervals: vec[0] and vec[nvars()-1]*/
   if(pt.vec[0]==-1)
     {/* we need to try to complete the leftmost alignament*/
       a=lower[0];
       for (int k=1;k<nvars();k++)
	 {
	   if (pt.vec[k]>-1)
	     {
	       b=pt.vec[k];
	       k=-2;/*get out of the for loop*/
	     }
	 }
       urand.low(1);
       urand.high(b-a-1);
       pt.vec[0]= a+urand();

     }
   if(pt.vec[nvars()-1]==-1)
     {/* we need to try to complete the rightmos alignament*/
       b=upper[0];
       for (int k=nvars()-2;k>=0;k--)
	 {
	   if (pt.vec[k]>-1)
	     {
	       a=pt.vec[k];
	       k=-2;/*get out of the for loop*/
	     }
	 }
       urand.low(1);
       urand.high(b-a-1);
       pt.vec[nvars()-1]= a+urand();

     }




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
fe=number of residues in the shortest protein
ts=0 (the 0 residue of the long protein)
te=number of residues in the longest protein

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



if( ((fe-fs)<=1) || ((te-ts)<=1) )
  {/* we need return. The initialization of the subproblem was accomplished */
    return;
  }


/* we need to assign one residue and then recurse */
urand.low(fs);
urand.high(fe);
int randomResidue= urand();

urand.low(ts);
urand.high(fe);
myVec[randomResidue]= urand();

/* recurse the left subproblem */
random_init_alignament(myVec,fs,randomResidue-1,ts,myVec[randomResidue]-1);

/* recurse the right subproblem */
random_init_alignament(myVec,randomResidue+1,fs,myVec[randomResidue]+1,te);

    


}





void PGAprot::randomize()
{ 
 
cout<<"Initializing the population with random VALID alignaments"<<endl;
 //if (!domain_fn)
  //   return;
if (nvars() == 0)
   return;

IntVector lower, upper;
domain_fn.get_bounds(lower,upper);




for (unsigned int i=0; i<popsize(); i++) {
  cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
  random_init_alignament(Genotype(population(i)).vec,0,nvars(),lower[0],upper[0]);
  population(i).reset();
  }

cout<<"Random Initialization finished"<<endl;

}

