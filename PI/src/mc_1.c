#include <math.h>
#include <stdio.h>
#include <stdarg.h> 
#include <stdlib.h>
#include <time.h>

#define SEED 35791246
#define PI 3.141592653589793238462643

int main (int argc, char ** argv) {
	int n;
	double x, y;
	int i;
	int count = 0;
	double z;
	double my_pi;

	printf("Intervals: ");
	scanf("%d", &n);
	//time_t t;

	srand(SEED); 
	
	for (i = 0; i < n; i++) {
		// random
		x = ((double)rand()) / ((double)RAND_MAX);
		y = ((double)rand()) / ((double)RAND_MAX);
		//printf("RAND = %f\n", x);
		z = sqrt((x * x) + (y * y));
		// if in the circle count++
		if (z <= 1) count++;
	}
	my_pi = ((double)count / (double)n) * 4.0;
	printf("Calculated PI = %.17f, Error is %.17f\n", my_pi, fabs(my_pi - PI));
}