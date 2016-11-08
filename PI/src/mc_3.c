#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit  */
#include <stdarg.h>    /* for va_{list,args... */
#include <mpi.h>
#include <math.h>
#include <time.h>

#define N 100000

#define SEED 35791246
#define PI 3.141592653589793238462643


int id = 0; // MPI id for the current process (set global to be used in xprintf)

void xprintf(char *format, ...) {
	va_list args;
	va_start(args, format);
	printf("[Node %i] ", id);
	vprintf(format, args);
	fflush(stdout);
}


int main(int argc, char *argv[]) {

	int p;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	double elapsed_time = 0.0;
	double my_pi;
	double e = 0.001;

	if (id == 0) {
		xprintf("Total Number of processes : %i\n", p);
	}
	elapsed_time = -MPI_Wtime();

	MPI_Group world_group;
	MPI_Group workers;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);

	// ranks which are not to appear
	int ranks[1] = { p - 1 };

	MPI_Group_excl(world_group, 1, ranks, &workers);

	MPI_Comm work_comm;

	MPI_Comm_create(MPI_COMM_WORLD, workers, &work_comm);

	double points[N];
	// if generator node
	if (id == p - 1) {
		int request_worker = 1;
		int number;
		int partner_rank;

		srand(SEED);

		while (request_worker) {
			MPI_Status status;
			MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			if (!number) request_worker = 0;
			else {
				for (int i = 0; i < N; i++) 
					points[i] = ((double)rand()) / ((double)RAND_MAX);

				MPI_Send(&points, N, MPI_DOUBLE, status.MPI_SOURCE, 0, MPI_COMM_WORLD);

			}
		}
	}
	// if other nodes
	else {

		MPI_Bcast(&e, 1, MPI_DOUBLE, 0, work_comm);

		int p_inside = 0, p_outside = 0, global_p_inside = 0, gloabal_p_outside = 0;
		int request_generator = 1;
		MPI_Send(&request_generator, 1, MPI_INT, p - 1, 0, MPI_COMM_WORLD);

		while (request_generator) {
			MPI_Recv(&points, N, MPI_DOUBLE, p - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int i = 0; i<N - 1; i = i + 2) {
				if ((points[i] * points[i] + points[i + 1] * points[i + 1]) <= 1.0f) p_inside++;
				else p_outside++;
			}
			// reduce the the sums from all WORKERS
			MPI_Allreduce(&p_inside, &global_p_inside, 1, MPI_INT, MPI_SUM, work_comm);
			MPI_Allreduce(&p_outside, &gloabal_p_outside, 1, MPI_INT, MPI_SUM, work_comm);

			// we achieved our accuracy, don't accept generated numbers
			if (e > ((double)1 / ((double)2 * sqrt(global_p_inside + gloabal_p_outside)))) request_generator = 0;

			MPI_Send(&request_generator, 1, MPI_INT, p - 1, 0, MPI_COMM_WORLD);
		}

		if (id == 0) {
			my_pi = 4.0 * (double)p_inside / (double)(p_outside + p_inside);
			xprintf("Calculated PI = %.17f, Error is %f\n", my_pi, fabs(my_pi - PI));
		}

	}



	MPI_Finalize();
	return 0;
}
