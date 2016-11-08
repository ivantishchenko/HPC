/*
Parallel and GRID computing
pi_1.c
Purpose: Calculates the PI

@author Ivan Tishchenko
@version 1.0
*/
#include <math.h>
#include <stdio.h>

#define PI 3.141592653589793238462643

double func_integer(double x) {
	return 4.0 / (1.0 + x * x);
}

double simpson(int i, double y, double l, double sum, int n) {
	sum += func_integer(y);
	sum += 2.0 * func_integer(y - l);
	if (i == (n - 1))
		sum += 2.0 * func_integer(y + l);
	return sum;
}
int main(int argc, char *argv[]) {
	int n;
	double total;
	double my_pi, y, x1, x2, l, sum;

	printf("Intervals: ");
	scanf("%d", &n);

	/* Each processor computes its interval */
	x1 = 0;
	x2 = 1;

	l = 1 / ((double)(2 * n));
	sum = 0.0;
	for (int i = 1; i < n; i++)
	{
		y = x1 + (x2 - x1) * i / ((double)n);
		sum = simpson(i, y, l, sum, n);
	}

	sum += (func_integer(x1) + func_integer(x2)) / 2.0;
	my_pi = sum * 2.0 * l / 3.0;
	printf("Calculated PI = %.17f, Error is %.17f\n", my_pi, fabs(my_pi - PI));
	return 0;
}