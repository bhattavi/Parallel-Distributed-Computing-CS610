
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h> 
int main(void)
{
  int x, sum=0, comm_sz, my_rank;
  MPI_Init(NULL, NULL); 
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
  srandom((unsigned)time(NULL) + my_rank);
  x = random()%10;
  printf("Process %d generates: %d\n", my_rank, x);
  MPI_Reduce(&x, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (my_rank == 0)
    printf("sum from reduce function = %d\n", sum); 
  MPI_Finalize(); 
  return 0;
}
