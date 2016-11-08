/*
Parallel and GRID computing
mc_1.c
Purpose: Calculates the PI Monte carlo

@author Ivan Tishchenko
@version 1.0
*/
#include <math.h>
#include <stdio.h>
#include <stdarg.h> 
#include <stdlib.h>
#include <time.h>

#define SEED 35791246
#define PI 3.141592653589793238462643

int main (int argc, char ** argv) {
	int n = 0;
	double x, y;
	int i;
	int count = 0;
	double z;
	double my_pi;
	const double e = 0.001f;
	//time_t t;
	FILE* fp;
	fp = fopen("data_circle.txt", "a");
	srand(SEED); 
	do {
		n++;
		x = ((double)rand()) / ((double)RAND_MAX);
		y = ((double)rand()) / ((double)RAND_MAX);
		//printf("RAND = %f\n", x);
		z = sqrt((x * x) + (y * y));
		// if in the circle count++
		if (z <= 1) {
			count++;
			//inside a circle
			fprintf(fp, "%d %f %f\n", 1, x, y);
		}
		//outside of a circle
		else fprintf(fp, "%d %f %f\n", 0, x, y);
	} while (e < ( 1 / ((double)2 * sqrt(n))));

	my_pi = ((double)count / (double)n) * 4.0;
	fclose(fp);
	printf("Calculated PI = %.17f, Error is %.17f\n", my_pi, fabs(my_pi - PI));
}