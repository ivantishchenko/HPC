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

	if (id == 0) {
		pi *= 2.0 * l / 3.0;
		xprintf("Calculated pi %.16f\n Error is %.16f\n", pi, fabs(pi - PI));
	}

	MPI_Finalize();
}