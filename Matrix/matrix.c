// Matrix-vector I/O and sequential algorithm

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SRAND 32425
#define MPI_DATA_TYPE MPI_DOUBLE

#define BLOCK_LOW(id, p, k) ( id * k / p )
#define BLOCK_HIGH(id, p, k) ( BLOCK_LOW((id) + 1, p, k) - 1)
#define BLOCK_SIZE(id, p, k) ( BLOCK_HIGH(id, p, k) - BLOCK_LOW(id, p, k) + 1)
#define BLOCK_OWNER(j, p, k) ( ((p) * (j + 1) - 1) / k )

void * vector_alloc(size_t n, size_t size) {
	return calloc(n, size);
}

void ** matrix_alloc(size_t m, size_t n, size_t size) {
	void ** matr = calloc(m, sizeof(void*));
	for (int i = 0; i < m; i++) {
		matr[i] = calloc(n, size);
	}
	return matr;
}

void create_count_disp_arrays(int id, int p, size_t k, int **count, int **disp) {
	int size = BLOCK_SIZE(id, p, k);
	(*disp)[0] = 0;
	(*count)[0] = size;
	for (int i = 1; i < p; ++i) {
		(*disp)[i] = (*disp)[i - 1] + (*count)[i - 1];
		(*count)[i] = size;
	}
}

void create_mixed_count_disp_arrays(int p, size_t k, int **count, int **disp) {
	(*count)[0] = BLOCK_SIZE(0, p, k);
	(*disp)[0] = 0;

	for (int i = 1; i < p; ++i) {
		(*disp)[i] = (*disp)[i - 1] + (*count)[i - 1];
		(*count)[i] = BLOCK_SIZE(i, p, k);
	}
}

double* base_i_vector_n(int i, int n) {
	double *v = (double*)vector_alloc(n, sizeof(double));
	v[i] = 1;
	return v;
}

double* random_vector_n(int n) {
	double *v = (double*)vector_alloc(n, sizeof(double));
	for (int i = 0; i < n; i++)
		v[i] = rand() % 255;
	return v;
}

double** id_matrix_m_n(int m, int n) {
	double **matr = (double**)matrix_alloc(m, n, sizeof(double));
	if (m < n)
		for (int i = 0; i < m; ++i)
			matr[i][i] = 1;
	else for (int i = 0; i < n; ++i)
		matr[i][i] = 1;
	return matr;
}

double** random_matrix_m_n(int m, int n) {
	double **matr = (double**)matrix_alloc(m, n, sizeof(double));
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			matr[i][j] = (rand() % 255);
	return matr;
}

void vector_free(void *v) {
	free(v);
}

void matrix_free(void **M, size_t m) {
	for (int i = 0; i < m; i++) {
		free(M[i]);
	}
	free(M);
}

void store_vector(char *f, size_t n, size_t size, void *v) {
	FILE *file;
	file = fopen(f, "w");
	fwrite(v, n * size, 1, file);
	fclose(file);
}

void store_matrix(char *f, size_t m, size_t n, size_t size, void **M) {
	FILE *file;
	file = fopen(f, "w");
	for (int i = 0; i < m; ++i) {
		fwrite(M[i], size * n, 1, file);
	}
	fclose(file);
}

void read_vector(char *f, size_t size, size_t n, void **v) {
	FILE *file;
	file = fopen(f, "r");
	fread(*v, size * n, 1, file);
	fclose(file);
}

void read_matrix(char *f, size_t size, size_t m, size_t n, void ***M) {
	FILE *file;
	file = fopen(f, "r");
	for (int i = 0; i < m; ++i) {
		fread((*M)[i], size * n, 1, file);
	}
	fclose(file);
}

void print_vector_lf(void *v, MPI_Datatype type, size_t n) {
	for (int i = 0; i < n; ++i) {
		if (type == MPI_INT)
			printf("%d ", ((int *)v)[i]);
		if (type == MPI_DOUBLE)
			printf("%0.2f ", ((double *)v)[i]);
		if (type == MPI_CHAR)
			printf("%c ", ((char *)v)[i]);
	}
}

void print_matrix_lf(void **M, MPI_Datatype type, size_t m, size_t n) {
	for (int j = 0; j < m; ++j) {
		for (int i = 0; i < n; ++i) {
			if (type == MPI_INT)
				printf("%d ", ((int**)M)[j][i]);
			if (type == MPI_DOUBLE)
				printf("%0.2f ", ((double**)M)[j][i]);
			if (type == MPI_CHAR)
				printf("%c ", ((char**)M)[j][i]);
		}
		printf("\n");
	}
}

void seq_matvec_mult_lf(double **M, double *b, size_t m, size_t n, double **c) {
	for (int i = 0; i < m; ++i) {
		(*c)[i] = 0;
		for (int j = 0; j < n; ++j) {
			(*c)[i] += M[i][j] * b[j];
		}
	}
}


int main(int argc, char* argv[]) {
   srand(SRAND);
   char *p1 = "base_i_vector_n.dat";
   char *p2 = "random_vector_n.dat";
   char *p3 = "id_matrix_m_n.dat";
   char *p4 = "random_matrix_m_n.dat";
   int i, m, n;
   printf("Type n:\n");
   scanf("%d",&n);
   printf("Type m:\n");
   scanf("%d",&m);
   printf("Type i:\n");
   scanf("%d",&i);
   double* v1 = (double*) base_i_vector_n(i, n);
   double* v2 = (double*) random_vector_n(n);
   double** matr1 = (double**) id_matrix_m_n(m, n);
   double** matr2 = (double**) random_matrix_m_n(m, n);
   store_vector(p1, n, sizeof(double), v1);
   store_vector(p2, n, sizeof(double), v2);
   store_matrix(p3, m, n, sizeof(double), matr1);
   store_matrix(p4, m, n, sizeof(double), matr2);
   vector_free(v1);
   vector_free(v2);
   matrix_free(matr1, m);
   matrix_free(matr2, m);
   
   v1 = (double*) vector_alloc(n, sizeof(double));
   v2 = (double*) vector_alloc(n, sizeof(double));
   double *v3 = (double*) vector_alloc(m, sizeof(double));
   matr1 = (double**) matrix_alloc(m, n, sizeof(double));
   matr2 = (double**) matrix_alloc(m, n, sizeof(double));
   printf("Read file base_i_vector_n.dat\n");
   read_vector(p1, sizeof(double), n, &v1);
   print_vector_lf(v1, MPI_DATA_TYPE, n); 
   printf("\nRead file random_vector_n.dat\n");
   read_vector(p2, sizeof(double), n, &v2);
   print_vector_lf(v2, MPI_DATA_TYPE, n);
   printf("\nRead file id_matrix_m_n.dat\n");
   read_matrix(p3, sizeof(double), m, n, &matr1);
   print_matrix_lf(matr1, MPI_DATA_TYPE, m, n);
   printf("\nRead file random_matrix_m_n.dat\n");
   read_matrix(p4, sizeof(double), m, n, &matr2);
   print_matrix_lf(matr2, MPI_DATA_TYPE, m, n);
   printf("Seq matr and vector\n");
   seq_matvec_mult_lf(matr2, v2, m, n, &v3);
   print_vector_lf(v3, MPI_DATA_TYPE, m);
   vector_free(v1);
   vector_free(v2);
   vector_free(v3);
   matrix_free(matr2, m);
   matrix_free(matr1, m);
   return 0;
}
