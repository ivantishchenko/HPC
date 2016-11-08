/*
Parallel and GRID computing
pi_3.c
Purpose: Calculates the PI and calculate the time

@author Ivan Tishchenko
@version 1.0
*/
#include <math.h>
#include <stdio.h>
#include <mpi.h>
#include <stdarg.h> 

#define PI 3.141592653589793238462643

int id = 0;

void xprintf(char *format, ...) {
	va_list args;
	va_start(args, format);
	printf("[Node %i] ", id);
	vprintf(format, args);
	fflush(stdout);
}

int main(int argc, char ** argv) {
	int n, p;
	double local_pi, global_pi, h, sum, x;
	double elapsed_time = 0.0;
	double min_time, max_time, avg_time;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (id == 0) {
		xprintf("Intervals: ");
		scanf("%d", &n);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	elapsed_time = -MPI_Wtime();
	// each process would get a number of intervals
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (n == 0) return;

	h = 1.0 / (double)n;
	sum = 0.0;
	//(1 / n) *4 / (1 + x * x)
	for (int i = id + 1; i <= n; i += p) {
		x = h * ((double)i - 0.5);
		sum += 4.0 / (1.0 + x * x);
	}
	
	local_pi = h * sum;

	elapsed_time += MPI_Wtime();
	// sum up local PIs to a gloabl PI
	MPI_Reduce(&local_pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// at the end, compute elapsed time
	
	MPI_Reduce(&elapsed_time, &min_time, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
	MPI_Reduce(&elapsed_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&elapsed_time, &avg_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (id == 0) {
		xprintf("Calculated PI = %.17f, Error is %.17f\n", global_pi, fabs(global_pi - PI));
		xprintf("Elapsed time: %2f s\n", elapsed_time);

		FILE *fp;

		avg_time /= p;
		xprintf("Elapsed time MAX: %f s\n", max_time);
		xprintf("Elapsed time MIN: %f s\n", min_time);
		xprintf("Elapsed time AVG: %f s\n", avg_time);

		fp = fopen("data_trap.txt", "a");
		fprintf(fp, "%d %f %f %f\n", p, max_time, min_time, avg_time);
		fclose(fp);
	}
	MPI_Finalize();
	return 0;
}