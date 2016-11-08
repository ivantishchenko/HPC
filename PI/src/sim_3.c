/*
Parallel and GRID computing
pi_3.c
Purpose: Calculates the PI and calculate the time

@author Ivan Tishchenko
@version 1.0
*/
/*
Parallel and GRID computing
pi_2.c
Purpose: Calculates the PI

@author Ivan Tishchenko
@version 1.0
*/
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

int id; // Id of the process
const double PI = 3.141592653589793238462643;  // 25 elements of PI


double integrate_f(double x) {
	return 4.0 / (1.0 + x * x);
}

double simpson(int i, double y, double l, double sum, int n) {
	sum += integrate_f(y);
	sum += 2.0 * integrate_f(y - l);
	if (i == (n - 1))
		sum += 2.0 * integrate_f(y + l);
	return sum;
}

/**
* Redefinition of the xprintf to include the buffer flushing
*/
void xprintf(char *format, ...) {
	va_list args;
	va_start(args, format);
	printf("[Node %i] ", id);
	vprintf(format, args);
	fflush(stdout);
}

int main(int argc, char *argv[]) {
	int p, n;
	double pi, y, processor_output_share[32], x1, x2, l, sum;
	MPI_Status status;
	double min_time, max_time, avg_time;
	double elapse_time = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	if (id == 0) {
		xprintf("Itervals: ");
		scanf("%d", &n);
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	elapse_time = -MPI_Wtime();

	x1 = id / ((double)p);
	x2 = (id + 1) / ((double)p);

	l = 1 / ((double)(2 * n * p));
	sum = 0.0;
	for (int i = 1; i < n; i++)
	{
		y = x1 + (x2 - x1) * i / ((double)n);
		sum = simpson(i, y, l, sum, n);
	}

	sum += (integrate_f(x1) + integrate_f(x2)) / 2.0;

	elapse_time += MPI_Wtime();

	MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Reduce(&elapse_time, &min_time, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
	MPI_Reduce(&elapse_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&elapse_time, &avg_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (id == 0) {
		pi *= 2.0 * l / 3.0;
		xprintf("Calculated pi %.16f\n Error is %.16f\n", pi, fabs(pi - PI));

		avg_time /= p;
		xprintf("Elapsed time MAX: %f s\n", max_time);
		xprintf("Elapsed time MIN: %f s\n", min_time);
		xprintf("Elapsed time AVG: %f s\n", avg_time);
		FILE* fp;
		fp = fopen("data.txt", "a");
		fprintf(fp, "%d %f %f %f\n", p, max_time, min_time, avg_time);
		fclose(fp);
	}

	MPI_Finalize();
}