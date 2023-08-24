
#include <math.h>
#include <utilib/_math.h>
#include <utilib/errmsg.h>
#include "PGALocalSearch.h"


static DUniform urand;

PGALocalSearch::PGALocalSearch(RNG* rng_)
: IntOptimizer("PGALocalSearch")
{
set_rng(rng_);
}


PGALocalSearch::PGALocalSearch(IntOptProblem* problem_, RNG* rng_)
: IntOptimizer("PGALocalSearch")
{
problem=problem_;
set_rng(rng_);
}


void PGALocalSearch::set_rng(RNG* rng_)
{
BaseOptimizer::set_rng(rng_);
urand.generator(rng_);

}


void PGALocalSearch::setLocalSearcherType(int lsType)
{
 _localSearcherType = lsType;

}




void PGALocalSearch::setLocalSearcherIterationNumber(int itNum)
{
 _localSearcherIterationNumber = itNum;

}


int PGALocalSearch::minimize()
{
 

    REAL currValue;
    IntVector tempSol;
    REAL tempSolValue;
    int f;
    bool changed=true;
    REAL  prevValue;
    int  workingLocalSearcher;

    opt_init();
    problem->get_bounds(lower,upper);
    
    curr = problem->vars();
    problem->EvalF(curr,currValue);
    DEBUGPR(100, ucout<< "---------------------------------------------"<<endl;)
    DEBUGPR(100, ucout<<"LOCAL SEARCH- start Val="<<currValue<<endl;)

     
    tempSol= curr;
    prevValue = currValue;
    DEBUGPR(100, ucout<< "in localSearcherType="<<_localSearcherType<<endl;)
    if (_localSearcherType==8)
      {/* we select at random the meme to use */
	urand.low(1);
	urand.high(7);
	workingLocalSearcher=urand();

      }
    else
      if(_localSearcherType==-2)
	{/* we are in a multimeme setting */
	  workingLocalSearcher = curr[problem->nvars()-1];
	  DEBUGPR(100, ucout<<"WLS="<<workingLocalSearcher<<endl;)
	}
      else
	{
	  workingLocalSearcher = _localSearcherType;
	}
    DEBUGPR(100, ucout<<"local search used:"<<    workingLocalSearcher<<endl;)
    switch(workingLocalSearcher)
    {
     case 1:
       {/* Brian's local searcher */
	 for(f=0;changed &&(f<_localSearcherIterationNumber);f++)
	   {
	     firstFillMove(&tempSol);
	     problem->EvalF(tempSol,tempSolValue);
	     changed=( tempSolValue!=prevValue);
	     prevValue= tempSolValue;
	   }
	 break;
       }
    case 2:
      { /* Sliding window of size 2 */
	for(f=0;changed &&(f<_localSearcherIterationNumber);f++)
	  {
	    slidingMove(&tempSol,2);
	     problem->EvalF(tempSol,tempSolValue);
	     changed=( tempSolValue!=prevValue);
	     prevValue= tempSolValue;
	  }
	break;
	
      }
    case 3:
      { /* Sliding window of size 4 */
	for(f=0;changed &&(f<_localSearcherIterationNumber);f++)
	  {
	    slidingMove(&tempSol,4);
	    problem->EvalF(tempSol,tempSolValue);
	    changed=( tempSolValue!=prevValue);
	    prevValue= tempSolValue;
	  }
	break;
	
      }

    case 4:
      { /* Sliding window of size 8 */
	for(f=0;changed &&(f<_localSearcherIterationNumber);f++)
	  {
	    slidingMove(&tempSol,8);
	    problem->EvalF(tempSol,tempSolValue);
	    changed=( tempSolValue!=prevValue);
	    prevValue= tempSolValue;
	  }
	break;
	
      }
    case 5:
      { /* Sliding window of size 16 */
	for(f=0;changed &&(f<_localSearcherIterationNumber);f++)
	  {
	    slidingMove(&tempSol,16);
	    problem->EvalF(tempSol,tempSolValue);
	    changed=( tempSolValue!=prevValue);
	    prevValue= tempSolValue;
	  }
	break;
	
      }
    case 6:
      {/* Scan move */
	for(f=0;changed &&(f<_localSearcherIterationNumber);f++)
	  {
	    scanMove(&tempSol);
	    problem->EvalF(tempSol,tempSolValue);
	    changed=( tempSolValue!=prevValue);
	    prevValue= tempSolValue;
	  }
	break;
      }
    case 7:
      {/* Spread move */
	for(f=0;changed &&(f<_localSearcherIterationNumber);f++)
       	  {
	    spreadMove(&tempSol);
	    problem->EvalF(tempSol,tempSolValue);
	    changed=( tempSolValue!=prevValue);
	    prevValue= tempSolValue;
	  }
	break;
      }


    }

    curr=tempSol;
    problem->EvalF(curr,currValue);
    DEBUGPR(100, ucout<<"LOCAL SEARCH- end Val="<<currValue<<endl;)
    DEBUGPR(100, ucout<< "---------------------------------------------"<<endl;)
    problem->set_vars(curr);
    best_val = currValue;


    return OK;
}

/*This operator tries to spread the alignment in such a way as to promote non contiguos -1 regions in the align */
void PGALocalSearch::spreadMove(IntVector *tempSol)
{
 IntVector  workingSol;
 REAL       workingSolValue;
 REAL       tempSolValue;
 bool       improvementFound;
 int        cursor;
 int        prevCursor;
 int        forwardCursor;
 int        i;
 int        numVars;





 DEBUGPR(100, ucout <<" In spreading move:"<<endl;)
 numVars=problem->nvars() -1 -1;
 workingSol=*tempSol;
 problem->EvalF(*tempSol,tempSolValue);
 DEBUGPR(100, ucout<<"OV="<<tempSolValue<<endl;)
 /* spreading right */
 prevCursor    = -2;
 forwardCursor = -2;
 cursor = 1;
 improvementFound = false;
 while ((cursor<numVars))
   {
    if(workingSol[cursor]==-1)
      {/* move forward to find the next non -1 */
       cursor++;
      }
    else
      {/* we found a non -1, move to the right of it */
       while((workingSol[cursor]!=-1)&&(cursor<numVars))
	 {
	   prevCursor = cursor;
	   cursor++;
	 }
       /* at this point we know where the non -1 run ends */
       /* find next where the -1 run ends now:*/
      while((workingSol[cursor]==-1)&&(cursor<numVars))
	{
	  cursor++;
	}
      /* at this point we know where the -1 run starts, prevCursor, and end, cursor */
      for(i=prevCursor;(i<cursor)&&(!improvementFound)&&(i<numVars);i++)
	{/* we are going to move the alignment in workingSol[prevCursor-1] to i and check whether is better or not */
	  workingSol[i]=workingSol[prevCursor-1];
	  workingSol[prevCursor-1] = -1;
	  DEBUGPR(100, ucout << "----------"<<endl;)
	  DEBUGPR(100, ucout <<"spreading "<<prevCursor<< " to " << i<<endl;)
	  problem->EvalF(workingSol,workingSolValue);
	  if(workingSolValue<tempSolValue)
	    {
	      DEBUGPR(100, ucout << " better "<<endl;)
	      improvementFound=true;
	      *tempSol = workingSol;
	      tempSolValue = workingSolValue;
	    }
	  else
	    {
	      DEBUGPR(100, ucout <<" worst "<<endl;)
	      workingSol[prevCursor-1]=workingSol[i];
	      workingSol[i] = -1;
	    }        
	  DEBUGPR(100, ucout << "----------"<<endl;)
	}

      }
    
   }

       if(improvementFound)
	 {
	   *tempSol =workingSol;
	   DEBUGPR(100, ucout <<"newPointValue="<<tempSolValue<<endl;)
	   DEBUGPR(100, ucout <<"-----------"<<endl;)
	 }

 
}


/* This operator selects two alignments at random, finds the range each one of those alignments can be varied
without interfiering with other alignments and chooses one of those modifications */

void PGALocalSearch::scanMove(IntVector *tempSol)
{

 IntVector availableAligns;
 IntVector workingSol;
 REAL workingSolValue;
 REAL tempSolValue;
 bool improvementFound;
 int i,j;
 int numberOfAvailableAligns=0;
 int numVars=problem->nvars() -1 -1;
 int A1,A2; /* the alignments that are going to be scanned */
 int LA_A1,RA_A1,LA_A2,RA_A2;
 int lA1,rA1,lA2,rA2;
 IntVector lower, upper;

 /* we get the bounds, that is, the length of the longest protein */
 problem->get_bounds(lower,upper); 

 availableAligns= *tempSol; /* the plus 1 is a secury gap :-) */
 workingSol = *tempSol;
 /* we initialize the vector of available alignments */
 for (i=0;i<numVars;i++)
   {
    if(workingSol[i]!=-1)
      {
        availableAligns[numberOfAvailableAligns++]=i;
	DEBUGPR(100, ucout << "AA="<<i<<", NAA="<<numberOfAvailableAligns<<endl;)
      }
   }


if(numberOfAvailableAligns>1)
  {
    /* randomly select two alignments, A1 and A2, from the available alignments */
     urand.low(0);
     urand.high(numberOfAvailableAligns-1);
     A1=availableAligns[urand()];
     A2=A1;
     while(A2==A1)
       {
         urand.low(0);
         urand.high(numberOfAvailableAligns-1);
         A2=availableAligns[urand()];
       }
     if(A1>A2)
       {/* we put in A1 the leftmost residue */
         i=A2;
         A2=A1;
         A1=i;
       }
     DEBUGPR(100, ucout << "A1="<<A1<<", A2=,"<<A2<<endl;)
     /* at this point we have the two alignments to scan for improvements */
     /* we need to find whom are their neighbors */
     LA_A1=-1;
     RA_A1=-1;
     LA_A2=-1;
     RA_A2=-1;



     LA_A1 = findLeft(A1,0,&workingSol);
     if(LA_A1==-1)
       {
	 lA1 = 0;
       }
     else
       {
	 lA1 = workingSol[LA_A1];
       }

     RA_A1 = findRight(A1,numVars,&workingSol);
     if(RA_A1==-1)
       {
	 rA1 = upper[0];
       }
     else
       {
	 rA1 = workingSol[RA_A1];
       }

     LA_A2 = findLeft(A2,0,&workingSol);
     if(LA_A2==-1)
       {
	 lA2 = 0;
       }
     else
       {
	 lA2 = workingSol[LA_A2];
       }

     RA_A2 = findRight(A2,numVars,&workingSol);
     if(RA_A2==-1)
       {
	 rA2= upper[0];
       }
     else
       {
	 rA2 = workingSol[RA_A2];
       }


     problem->EvalF(*tempSol,tempSolValue);
     DEBUGPR(100, ucout <<"-----------"<<endl;)
     DEBUGPR(100, ucout <<"originalPointValue="<<tempSolValue<<endl;)
     improvementFound=false;
       for(i=lA1+1;((i<rA1)&&(!improvementFound));i++)
	 {
         for(j=lA2+1;((j<rA2)&&(!improvementFound));j++)
           {
             if(i<j)  /* they can not cross */
               {
                 workingSol[A1]=i;
                 workingSol[A2]=j;
                 problem->EvalF(workingSol,workingSolValue);
                 if(workingSolValue<tempSolValue)
                   {
		     improvementFound=true;
                   }
                 else
                   {
                     workingSol = *tempSol;
                   }
               }
           }
	 }
       if(improvementFound)
	 {
	   *tempSol =workingSol;
	   DEBUGPR(100, ucout <<"newPointValue="<<workingSolValue<<endl;)
	   DEBUGPR(100, ucout <<"-----------"<<endl;)
	 }

  }/* end of numberOfAvailableAligns>1 */





}/* end of scanMove */



/*
This operator tries to include a new alignment in an unocupied position.
To do this it discards all the previous alignments that conflicts with the one being tested.
It chooses th first improvement insertion.
*/
void PGALocalSearch::firstFillMove(IntVector *tempSol)
{
 int i,j,k;

 REAL tempSolValue;
 IntVector workingSol;
 REAL    workingSolValue;
 int numVars;
 IntVector lower, upper;
 bool improvementFound=false;


 problem->get_bounds(lower,upper); 
 numVars= problem->nvars() -1 -1;
 problem->EvalF(*tempSol,tempSolValue);

 workingSol=*tempSol;
 for (i=0;(i<numVars)&&(!improvementFound);i++)
   {
    if(workingSol[i]==-1)
      { /* we try to complete this position by aligning it to a residue in the other protein */
	for (j=0;(j<=upper[0])&&(!improvementFound);j++)
	  { /* we create de alignment */
	    workingSol[i]=j;
	    /*now we need to delete whatever alignment we cross by the insertion of i->j */
	    /*check left first */
	    for(k=0;k<i;k++)
	      {
		if(workingSol[k]>=workingSol[i])
		  { /* this alignment was invalidated by the previous insertion */
		    workingSol[k]=-1;
		  }
	      }
	    /*check right next */
	    for(k=i+1;k<=numVars;k++)
	      {
		if(workingSol[k]<=workingSol[i])
		  { /* this alignment was invalidated by the previous insertion */
		    workingSol[k]=-1;
		  }
	      }
	    /* now we compute the fitness of this new solution */
	    problem->EvalF(workingSol,workingSolValue);
	    if(workingSolValue<tempSolValue)
	      {
		improvementFound=true;
	      }
	    else
	      {
		workingSol=*tempSol;
	      }
	    
	  }
      }

   }
   if(improvementFound)
     {
       *tempSol=workingSol;
     }
}






/*
This operator tries to improve the current alignment by means of the same move as the sliding
window mutation.
It receives a window size  and randomly selects both a direction to move and a delta that specifieshow much to move.
*/
void PGALocalSearch::slidingMove(IntVector *pt,int windowSize)
{
   int  start; /* is the variable where the sliding window  starts; shold be 0<=start<=nvar-2-windowSize */  
   int  end;   /* is the variable where the sliding window  ends; should be end=start+windowSize+1    */      
   int dir;   /* boolean variable that specifiying the direction of the sliding window.
		   If dir==0 then we move to the left (towards the 0 variable).
		   If dir==1 then we move to the right (towards the (nvars()-1)  variable). */
   int  delta; /* how much are we going to slide the window. 
		    Must be 1<=delta<=vec[start] if dir==true.
		    Must be 1<=delta<=upper[0]-vec[end] if dir==false;*/
   int numVars;
   IntVector lower, upper;
   REAL ptValue;
   IntVector workingSol;
   REAL    workingSolValue;
   bool improvementFound=false;
   bool doIt = false;
   int k;
   
   problem->get_bounds(lower,upper); 
   numVars= problem->nvars()-1 -1;
   problem->EvalF(*pt,ptValue);
   workingSol=*pt;

 for(start=0;(start<(numVars-2-windowSize))&&(!improvementFound);start++)
 {
   end = start+windowSize;

   /* choose a direction and selects a suitable delta */
   urand.low(0);
   urand.high(1);
   dir=urand();
   if (dir==0)
     {/* sliding left*/
       urand.low(1);
       for(k=start;k<=end;k++)
	 {
	   if((*pt)[k]>-1)
	     break;
	 }
       if(k<=end)
	 {/* we can  apply mutation because there is at least one
	     alignament in the range [start,end] to slide left*/
	   doIt= true;
	   urand.high((*pt)[k]);
	   delta=urand();
	 }

     }
   else
     {/* we slide right */
       urand.low(1);
       for(k=end;k>=start;k--)
	 {
	   if((*pt)[k]>-1)
	     break;
	 }
       if(k>=start)
	 {/* we can  apply mutation because there is at least one
	     alignament in the range [start,end] to slide left*/
	   doIt= true;
	   urand.high(upper[0]-(*pt)[k]);
	   delta=urand();
	 }

     }

     if(doIt)
       {
	 DEBUGPR(100, ucout <<"----------------"<<endl;)
	 DEBUGPR(100, ucout <<start<<"->"<<end<<" , "<<dir<<","<<delta<<endl;)
	 slide(&workingSol,start,end,dir,delta);
	 /* now we compute the fitness of this new solution */
	 problem->EvalF(workingSol,workingSolValue);
	 if(workingSolValue<ptValue)
	 {
	  improvementFound=true;
         }
         else
         {
          workingSol=*pt;
         }
	
       }

 }/* end of for(start=0.... */

 if(improvementFound)
     {
       *pt=workingSol;
     }


}/* end of method */





void PGALocalSearch::slide(IntVector *pt, int start, int end, int dir, int delta)
{

 int a,b,i,k;
 IntVector lower, upper;
 problem->get_bounds(lower,upper);
 int leftmost, rightmost;
 bool first=true;
 int numVars;

 numVars=problem->nvars()-1 ;


  /* first we sum to every (*pt)[i] +/- delta but maintaining feasibility WITHIN the sliding window*/

     if(dir==0)
       { /* we move left */
	 leftmost=start;
	 for (i=start;i<=end;i++)
	   {
	     if( ((*pt)[i]>=delta) )
	       {
		 int t=findLeft(i,start,pt);
		 if(t>-1)
		   {
		     if((i>start)&&( ((*pt)[i]-delta)>(*pt)[t]))
		       {
			 (*pt)[i]= (*pt)[i]-delta;
			 if(first)
			   {
			     leftmost=i;
			     first=false;
			   }
		       }
		     else if (i==start)
		       {
			 (*pt)[i]= (*pt)[i]-delta;
			 if(first)
			   {
			     leftmost=i;
			     first=false;
			   }
		       }
		   }
		 else
		   {
		       (*pt)[i]= (*pt)[i]-delta;
		       if(first)
			   {
			     leftmost=i;
			     first=false;
			   }
		   }
	       }
	     else
	       {
		 DEBUGPR(100, ucout<<"WARNING SLIDING LEFT!"<<endl;)
	       }
	   }
       }
     else
       {  /* we move right */
	 rightmost=end;
	 for(i=end;i>=start;i--)
	   {
	     if( ((*pt)[i]>-1) && ((*pt)[i]<=upper[0]-delta) )
	       {
		 int t=findRight(i,end,pt);
		 if(t>-1)
		   {
		     if((i<end)&&((*pt)[i]+delta<(*pt)[t]))
		       {
			 (*pt)[i]=(*pt)[i]+delta;
			 if(first)
			   {
			     rightmost=i;
			     first=false;
			   }
		       }
		     else if(i==end)
		       {
			 (*pt)[i]=(*pt)[i]+delta;
			 if(first)
			   {
			     rightmost=i;
			     first=false;
			   }
		       }
		   }
		 else
		   {
		       (*pt)[i]=(*pt)[i]+delta;
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
	 if ((*pt)[i]>=(*pt)[leftmost])
	   {/* we delete the alignament 'cos it was crossed */
	     (*pt)[i]=-1;
	   }
       }
   }
 else
   {
     for(i=end+1;i<numVars;i++)
       {
	 if ((*pt)[i]<=(*pt)[rightmost])
	   {/* we delete the alignament 'cos it was crossed */
	     (*pt)[i]=-1;
	   }
       }
   }




 /* we need to try to complete whatever free residue we find in child. Supose i is free in child
      then we see to whom i-k and i+k are aligned for the smalles k. Supose i-k is aligned to a and i+k to b then
      i can be aligned to anything between a and b. We need first of course to figure out k */
   for (i=1;i<(numVars-1);i++)
     {
      if((*pt)[i]==-1)
	{
	  /* we initialize a and b in such a way that it spans the whole range of the longer protein*/
	  a=lower[0];/* all the numVars got the same bounds so we just peak the 0th one */
	  b=upper[0]; 
          for( k=i-1;k>=0;k--)
	    {
	      if ((*pt)[k]>-1)
		{
		  a=(*pt)[k];
		  break;		/*get out of the for loop*/
		}
	    }
          for( k=i+1;k<numVars;k++)
	    {
	      if ((*pt)[k]>-1)
		{
		  b=(*pt)[k];
		  break;		/*get out of the for loop*/
		}
	    }
          /* at this point we already know how much we can span in the big protein */
          if((b-a)>1)
	  {/* there is room for completion */
           urand.low(1);
	   urand.high(b-a-1);
           (*pt)[i]= a+urand();
	  }

	}
     }



   /* we need to take special precautions with the ends of the intervals: vec[0] and vec[numVars-1]*/
   if((*pt)[0]==-1)
     {/* we need to try to complete the leftmost alignament*/
       a=upper[0];
       for ( k=1;k<numVars;k++)
	 {
	   if ((*pt)[k]>-1)
	     {
	       a=(*pt)[k];
	       break;/*get out of the for loop*/
	     }
	 }
       if(a>=1)
	 {
	   urand.low(0);
	   urand.high(a-1);
	   (*pt)[0]= urand();
	 }
     }

   if((*pt)[numVars-1]==-1)
     {/* we need to try to complete the rightmos alignament*/
       DEBUGPR(100, ucout<<"pos "<<numVars-1<<" holds:"<<(*pt)[numVars-1]<<endl;)
       b=lower[0];
       for (k=numVars-2;k>=0;k--)
	 {
	   if ((*pt)[k]>-1)
	     {
	       b=(*pt)[k];
	       break;/*get out of the for loop*/
	     }
	 }
        if((upper[0])-b>=1)
	  {
	    urand.low(b+1);
	    urand.high(upper[0]);
	    (*pt)[numVars-1]= urand();
	  }

     }




}


int PGALocalSearch::findLeft(int i,int start, IntVector * apt)
{

 int f;
 int res=-1;
 bool out=false;

 for(f=i-1;(f>start)&&(!out);f--)
   {
     if ((*apt)[f]>-1)
       {
	 res = f;
	 out = true;
	 break;
       }
   }

 return res;
}



int PGALocalSearch::findRight(int i,int end, IntVector * apt)
{

 int f;
 int res=-1;
 bool out=false;

 for(f=i+1;(f<end)&&(!out);f++)
   {
     if ((*apt)[f]>-1)
       {
	 res = f;
	 out = true;
	 break;
       }
   }

 return res;
}



