#include <stdio.h>
#include <mpi.h> 

int main(void) 
{
  int comm_sz, my_rank, index = 0;

   MPI_Init(NULL, NULL); 
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
   index++;
   printf("index = %d\n", index);
   MPI_Finalize(); 

   return 0;
}