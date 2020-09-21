#include <stdio.h>
#include <math.h>

#define nMin 16
#define nMax 32
#define pMin 1
#define pMax 128



double t_serial(int n) {
        return pow(n, 2);
}

double t_parallel(int n, int p) {
        return (t_serial(n) / p) + (log(p) / log(2));
}

double speedups(int n, int p) {
        return t_serial(n) / t_parallel(n, p);
}

double efficiency(int n, int p) {
        return t_serial(n) / (p * t_parallel(n, p));
}

int main() {
        int n; 
        int p;
        
        printf("\n\n");
        printf("\tSpeedup Table\n");
        
        printf("\t");
        
        for(n = nMin; n <= nMax; n *= 2)
                printf("|%8.d", n); 
        printf("|\n");

       
        for(p = pMin; p <= pMax; p *= 2) {
                printf("%8.d", p); 
                for(n = nMin; n <= nMax; n *= 2)
                        printf("|%8.4f", speedups(n, p));
                printf("|\n");
        }


        printf("\n\n");

        printf("\tEfficiency Table\n");
       
        printf("\t");
        
        for(n = nMin; n <= nMax; n *= 2)
                printf("|%8.d", n);
        printf("|\n");

        for(p = pMin; p <= pMax; p *= 2) {
                printf("%8.d", p); 
                for(n = nMin; n <= nMax; n *= 2)
                        printf("|%8.4f", efficiency(n, p));
                printf("|\n");
        }
        printf("\n\n");
        return 0;
}