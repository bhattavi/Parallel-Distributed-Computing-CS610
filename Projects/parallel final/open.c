
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

// generate a random between low to high inclusive
int generateRandom(int low, int high)
{
    int n = high - low + 1;
    int rd = rand();
    int ret = rd%n + low;

    return ret;
}

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    // generate random threads
    // int nIteration = generateRandom(10, 100);
    // int nThreads = generateRandom(3, 15);
    // int nChunk = generateRandom(3, 15);

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
        printf("Thread %d    ", id);
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




