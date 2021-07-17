#include<string.h>
//#include<mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
void write_output(char fname[], double** arr, int n ){
    FILE *f = fopen(fname, "w");
    for( int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            fprintf(f, "%0.12f ", arr[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char *argv[])
{
  int n = atoi(argv[1]);
  char *input_file = argv[2];
  int t = atoi(argv[3]);
  int s = atoi(argv[4]);

  double** A = malloc(n * sizeof(double*));
  for(int i=0; i<n; i++){
    A[i] = malloc(n * sizeof(double*));
  }

  FILE *infile;
  infile = fopen(input_file, "r");
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      if(!fscanf(infile, "%lf", &A[i][j]))
        break;
    }
  }
  fclose(infile);

 double** L = malloc(n * sizeof(double*));
   for(int i=0; i<n; i++){
     L[i] = malloc(n * sizeof(double*));
   }
   double** U = malloc(n * sizeof(double*));
   for(int i=0; i<n; i++){
     U[i] = malloc(n * sizeof(double*));
   }

 int i, j, k;
 double sum = 0;
 #pragma omp parallel for num_threads(t)
 for (int i = 0; i < n; i++) {
    U[i][i] = 1;
   }

 for (j = 0; j < n; j++) {
   for (int i = j; i < n; i++) {
       sum = 0;
       #pragma omp parallel for num_threads(t)
       for (int k = 0; k < j; k++) {
           double v = L[i][k] * U[k][j];
           #pragma omp critical
           {sum = sum + v;}
       }
       #pragma omp barrier
       L[i][j] = A[i][j] - sum;
   }

   for (int i = j; i < n; i++) {
     sum = 0;

     #pragma omp  sections
     {
       #pragma omp  section
       {
         double s = 0;
         for (int k = 0; k < j/4 ; k++) {
             s = s + L[j][k] * U[k][i];
         }
         #pragma omp critical
           {sum= sum + s;}
       }

       #pragma omp  section
       {
         double s = 0;
         for (int k = j/4; k < 2*j/4; k++) {
           s=s+ L[j][k] * U[k][i];
         }
         #pragma omp critical
           {sum = sum + s;}
       }

       #pragma omp  section
       {
         double s = 0;
         for (int k = 2*j/4; k < 3*j/4; k++) {
             s=s+ L[j][k] * U[k][i];
         }
         #pragma omp critical
         sum = sum + s;
       }

       #pragma omp  section
       {
         double s = 0;
         for (int k = 3*j/4; k < 4*j/4; k++) {
             s= s + L[j][k] * U[k][i];
         }
         #pragma omp critical
         sum = sum + s;
       }
     }
       if (L[j][j] == 0) {
           exit(0);
       }
       U[j][i] = (A[j][i] - sum) / L[j][j];
   }
 }
 char fname1[50];
 sprintf(fname1, "output_U_%d_%d.txt", s,t);
 write_output(fname1, U, n );
 char fname2[50];
 sprintf(fname2, "output_L_%d_%d.txt", s,t);
 write_output(fname2, L, n );
  }
