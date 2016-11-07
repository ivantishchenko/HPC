#include <stdio.h>

/* Return 1 if 'i'th bit of 'n' is 1; 0 otherwise */
#define EXTRACT_BIT(n,i) ((n&(1<<i))? 1 : 0)
#define EXTRACT_BIT_CHAR(n, i) ((n & (1 << i)) ? '1': '0')

int check_circuit(int input) {
    int v[16], i;
    for (i = 0; i < 16; i++) v[i] = EXTRACT_BIT(input,i);
    return ((v[0] || v[1])    && (!v[1] || !v[3]) && (v[2] || v[3])  &&
            (!v[3] || !v[4])  && (v[4] || !v[5])  && (v[5] || !v[6]) &&
            (v[5] || v[6])    && (v[6] || !v[15]) && (v[7] || !v[8]) &&
            (!v[7] || !v[13]) && (v[8] || v[9])   && (v[8] || !v[9]) &&
            (!v[9] || !v[10]) && (v[9] || v[11])  && (v[10] || v[11])&&
            (v[12] || v[13])  && (v[13] || !v[14])&& (v[14] || v[15]));
}

char * int_to_binary(int x) {
	unsigned short bits = sizeof(int) * 8; // bits in int
	char * b = (char*)calloc(bits + 1, sizeof(char));

	for (unsigned short i = 0; i < bits; i++) {
		b[i] = EXTRACT_BIT_CHAR(x, i);
	}

	b[bits] = '\0';
	return b;
}

int main() {
	char * out;
	for ( int i = 0; i < 65535; i++ ) {
		if ( check_circuit(i) == 1 ) {
			out = int_to_binary(i);
			printf("- found solution %x = | %s \n", i, out);
			free(out);
		}
	}
    return 0;
}
