//g++ omp.cc -fopenmp

#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  srand((unsigned int)time(NULL));
  int nIteration = random() % 91 + 10;
  int nThreads = random() % 13 + 3;
  int nChunk = random() % 13 + 3;
  // record that each number is allocated to which thread
  int arr[nIteration];
  // display 3 random imformation
  printf("Iterations: %d\n", nIteration);
  printf("Threads: %d\n", nThreads);
  printf("Chunk: %d\n", nChunk);
  // schedule thread
#pragma omp parallel for num_threads(nThreads) schedule(static, nChunk)
  for (int i = 0; i < nIteration; i++)
  {
  	int id = omp_get_thread_num();
  	arr[i] = id;
  }
  // show the allocate information
  for (int id = 0; id < nThreads; id++)
  {
  	printf("Thread %d	", id);
  	for (int i = 0; i < nIteration; i++)
  	{
      	if (arr[i] == id)
      	{
          	printf("%2d, ", i);
      	}
  	}
  	printf("\n");
  }
  return 0;
}


---------------------


#include <stdio.h>
​
#include <math.h>
​
#define nMin 16
​
#define nMax 32
​
#define pMin 1
​
#define pMax 128
​
void printHorizontalLine(int min, int max){
​
      int i;
​
      printf("\t\t+");
​
      for(i=min;i<max;i*=2)
​
      {
​
            printf("----------");
​
      }
​
      printf("---+\n");
​
}
​
double Tserial(int n){
​
      return pow(n,2);
​
}
​
double Tparallela(int n, int p){
​
      return Tserial(n)/p + log(p)/log(2);
​
}
​
double Tparallelb(int n, int p, int Toverhead){
​
      return Tserial(n)/p + Toverhead;
​
}
​
double speedupA(int n, int p){
​
      return Tserial(n)/Tparallela(n,p);
​
}
​
double efficiencyA(int n, int p){
​
      return Tserial(n)/(p*Tparallela(n,p));
​
}
​
double efficiencyB(int n, int p, int Toverhead){
​
      return Tserial(n)/(p*Tparallelb(n,p,Toverhead));
​
}
​
int main(){
​
​
      int n;
​
​
      int p;
​
      printf("\t\tSpeedup\n");
​
​
      printHorizontalLine(nMin,nMax);
​
​
      printf("\t\t");
​
      for(n=nMin;n<=nMax;n*=2){
​
            printf("|%8.d",n);
​
      }
​
      printf("|\n");
​
​
      printHorizontalLine(nMin,nMax);
​
​
      for(p=pMin;p<=pMax;p*=2){
​
            printf("\t%8.d",p);
​
            for(n=nMin;n<=nMax;n*=2){
​
                  printf("|%8.4f",speedupA(n,p));
​
            }
​
            printf("|\n");
​
      }
​
​
      printHorizontalLine(nMin,nMax);
​
​
      printf("\n\n");
​
​
      printf("\t\tEfficiency\n");
​
      printHorizontalLine(nMin,nMax);
​
      printf("\t\t");
​
      for(n=nMin;n<=nMax;n*=2){
​
            printf("|%8.d",n);
​
      }
​
      printf("|\n");
​
​
      printHorizontalLine(nMin,nMax);
​
​
      for(p=pMin;p<=pMax;p*=2){
​
            printf("\t%8.d",p);
​
            for(n=nMin;n<=nMax;n*=2){
​
                  printf("|%8.4f", efficiencyA(n,p));
​
            }
​
            printf("|\n");
​
      }
​
​
      printHorizontalLine(nMin,nMax);
​
​
      printf("\n\n");
​
​
      printf("\t\tEfficiency when Toverhead=n\n");
​
      printHorizontalLine(nMin,nMax);
​
      printf("\t\t");
​
      for(n=nMin;n<=nMax;n*=2){
​
            printf("|%8.d",n);
​
      }
​
      printf("|\n");
​
​
      printHorizontalLine(nMin,nMax);
​
​
      for(p=pMin;p<=pMax;p*=2){
​
            printf("\t%8.d",p);
​
            for(n=nMin;n<=nMax;n*=2){
​
                  printf("|%8.6f",efficiencyB(n,p,n));
​
            }
​
            printf("|\n");
​
      }
​
​
      printHorizontalLine(nMin,nMax);
​
​
      printf("\n\n");
​
​
      printf("\t\tEfficiency when Toverhead=n^3\n");
​
    
​
      printHorizontalLine(nMin,nMax);
​
​
      printf("\t\t");
​
      for(n=nMin;n<=nMax;n*=2){
​
            printf("|%8.d",n);
​
      }
​
      printf("|\n");
​
      printHorizontalLine(nMin,nMax);
​
​
      for(p=pMin;p<=pMax;p*=2){
​
            printf("\t%8.d",p);
​
            for(n=nMin;n<=nMax;n*=2){
​
                  printf("|%8.6f",efficiencyB(n,p,n*n*n));
​
            }
​
            printf("|\n");
​
      }
​
​
      printHorizontalLine(nMin,nMax);
​
​
      printf("\n\n");
​
      return 0;
​
}

----------

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h> 
int main()
{
	int x, p, i, sum, comm_sz, my_rank, j, d1=1, d2=2;
	//int sum, comm_sz, my_rank, i, next, value;
	int n=0;
	MPI_Init(NULL, NULL); 
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	while(pow(2,n)!=comm_sz)
		n++;
	int values[comm_sz];
	srandom((unsigned)time(NULL) + my_rank);
	
	x = random()%10;
	values[my_rank]=x;
	printf("%d  ",x);
​
	
			for(i = 0; i < n; i++)
			{
				for(j = 0; j < comm_sz-d1; j = j + d2)
				{  
				if(my_rank==j+d1)
				{
				x=values[j+d1];	
				MPI_Send(&x, 1, MPI_INT, j, 0, MPI_COMM_WORLD);
				}
				
				else if(my_rank==j){
				MPI_Recv(&x, 1, MPI_INT, j+d1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				values[j]+=x;
				}
				}
			
				d2 = d2 * 2;
				d1 = d1 * 2;
			}			
		
		if(my_rank==0)
		printf("\n sum=: %d \n", values[0]);
	
​
	MPI_Finalize(); 
	return 0;
}
