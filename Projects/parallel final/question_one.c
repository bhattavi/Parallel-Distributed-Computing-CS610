#include <stdio.h>

#include <math.h>

#define nMin 16

#define nMax 32

#define pMin 1

#define pMax 128

void printHorizontalLine(int min, int max){

      int i;

      printf("\t\t+");

      for(i=min;i<max;i*=2)

      {

            printf("----------");

      }

      printf("---+\n");

}

double Tserial(int n){

      return pow(n,2);

}

double Tparallela(int n, int p){

      return Tserial(n)/p + log(p)/log(2);

}

double Tparallelb(int n, int p, int Toverhead){

      return Tserial(n)/p + Toverhead;

}

double speedupA(int n, int p){

      return Tserial(n)/Tparallela(n,p);

}

double efficiencyA(int n, int p){

      return Tserial(n)/(p*Tparallela(n,p));

}

double efficiencyB(int n, int p, int Toverhead){

      return Tserial(n)/(p*Tparallelb(n,p,Toverhead));

}

int main(){


      int n;


      int p;

      printf("\t\tSpeedup\n");


      printHorizontalLine(nMin,nMax);


      printf("\t\t");

      for(n=nMin;n<=nMax;n*=2){

            printf("|%8.d",n);

      }

      printf("|\n");


      printHorizontalLine(nMin,nMax);


      for(p=pMin;p<=pMax;p*=2){

            printf("\t%8.d",p);

            for(n=nMin;n<=nMax;n*=2){

                  printf("|%8.4f",speedupA(n,p));

            }

            printf("|\n");

      }


      printHorizontalLine(nMin,nMax);


      printf("\n\n");


      printf("\t\tEfficiency\n");

      printHorizontalLine(nMin,nMax);

      printf("\t\t");

      for(n=nMin;n<=nMax;n*=2){

            printf("|%8.d",n);

      }

      printf("|\n");


      printHorizontalLine(nMin,nMax);


      for(p=pMin;p<=pMax;p*=2){

            printf("\t%8.d",p);

            for(n=nMin;n<=nMax;n*=2){

                  printf("|%8.4f", efficiencyA(n,p));

            }

            printf("|\n");

      }


      printHorizontalLine(nMin,nMax);


      printf("\n\n");


      printf("\t\tEfficiency when Toverhead=n\n");

      printHorizontalLine(nMin,nMax);

      printf("\t\t");

      for(n=nMin;n<=nMax;n*=2){

            printf("|%8.d",n);

      }

      printf("|\n");


      printHorizontalLine(nMin,nMax);


      for(p=pMin;p<=pMax;p*=2){

            printf("\t%8.d",p);

            for(n=nMin;n<=nMax;n*=2){

                  printf("|%8.6f",efficiencyB(n,p,n));

            }

            printf("|\n");

      }


      printHorizontalLine(nMin,nMax);


      printf("\n\n");


      printf("\t\tEfficiency when Toverhead=n^3\n");

    

      printHorizontalLine(nMin,nMax);


      printf("\t\t");

      for(n=nMin;n<=nMax;n*=2){

            printf("|%8.d",n);

      }

      printf("|\n");

      printHorizontalLine(nMin,nMax);


      for(p=pMin;p<=pMax;p*=2){

            printf("\t%8.d",p);

            for(n=nMin;n<=nMax;n*=2){

                  printf("|%8.6f",efficiencyB(n,p,n*n*n));

            }

            printf("|\n");

      }


      printHorizontalLine(nMin,nMax);


      printf("\n\n");

      return 0;

}