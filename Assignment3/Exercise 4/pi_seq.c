
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <mpi.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
  int rank, size, i, provided;

  MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double start_time = MPI_Wtime();
  int count = 0;
  double x, y, z, pi;

  int per_process = NUM_ITER / size;

  srand(SEED * rank); // Important: Multiply SEED by "rank" when you introduce MPI!

  // Calculate PI following a Monte Carlo method
  for (int iter = 0; iter < per_process; iter++)
    {
      // Generate random (X,Y) points
      x = (double)random() / (double)RAND_MAX;
      y = (double)random() / (double)RAND_MAX;
      z = sqrt((x*x) + (y*y));

      // Check if point is in unit circle
      if (z <= 1.0)
	{
	  count++;
	}
    }

  int total;

  MPI_Reduce(&count, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  
  // Estimate Pi and display the result
  pi = ((double)total / (double)NUM_ITER) * 4.0;
  double time = MPI_Wtime() - start_time;
  if (rank == 0)
    printf("The result is %f\nTime taken: %f\n", pi, time);
  return 0;
}

