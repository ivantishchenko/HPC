/*
Parallel and GRID computing
pi_1.c
Purpose: Calculates the PI

@author Ivan Tishchenko
@version 1.0
*/
#include <math.h>
#include <stdio.h>

//#define PI 3.141592653589793238462643

int id = 0;

int main(int argc, char ** argv) {
	int n;
	double my_pi, h, sum, x, e;

	printf("Intervals: ");
	scanf("%d", &n);

	h = 1.0 / (double)n;
	sum = 0.0;
	//(1 / n) *4 / (1 + x * x)
	for (int i = 1; i <= n; i++) {
		x = h * ((double)i - 0.5);
		sum += 4.0 / (1.0 + x * x);
	}

	my_pi = h * sum;

	e = 2 / (3. * n * n);

	printf("Calculated PI = %.17f, Error is %.17f\n", my_pi, e);

	return 0;
}