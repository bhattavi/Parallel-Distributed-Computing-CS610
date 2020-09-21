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
	

	MPI_Finalize(); 
	return 0;
}


