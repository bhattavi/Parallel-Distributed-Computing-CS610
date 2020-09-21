//File name: 3.c
#include <stdio.h>	
#include <stdlib.h>
#include <mpi.h> 
#include <time.h>

int main(void)
{
  int x, p, sum, comm_sz, my_rank;

  MPI_Init(NULL, NULL); 
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
  srandom((unsigned)time(NULL) + my_rank);
  x = random()%10;
  printf("%d,",x);
  if (my_rank != 0)
    MPI_Send(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
  else
  {	
    sum = x;
    for (p = 1; p < comm_sz; p++)
    {
      MPI_Recv(&x, 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      sum += x;
    }
    printf("\nsum=: %d\n", sum);
  }    
  MPI_Finalize(); 
  return 0;
}