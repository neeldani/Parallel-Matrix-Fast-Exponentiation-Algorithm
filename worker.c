#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "config.h"

int *getMatrix(int m, int n) {

	int *arr = (int*)malloc(sizeof(int)*m*n);
	memset(arr, 0, n*m*sizeof(int));
	return arr;
}

int *Matmul(int *A, int *B) {

	int m = M/P;
	int n = M;
	int *result = getMatrix(m, n);
	
	for(int i=0; i<m; i++) {

		for(int j=0; j<n; j++) {

			for(int k=0; k<n; k++)
				result[i*n + j] += A[i*n + k] * B[k*n + j]; 
		}
	}

	return result;
}

void Slavematmul(MPI_Comm intercomm) {

	int n_rows = M * M / P;

	int *A = getMatrix(M, M);
	int *recvbuf = getMatrix(M / P, M);
	
	int *buf = getMatrix(M, M); // redundant for slave 
	
	MPI_Bcast(A, M*M, MPI_INT, 0, intercomm);
	MPI_Scatter(buf, n_rows, MPI_INT, recvbuf, n_rows, MPI_INT, 0, intercomm);
	int *result = Matmul(recvbuf, A);
	MPI_Gather(result, n_rows, MPI_INT, &buf, n_rows, MPI_INT, 0, intercomm);
}

int main(int argc, char *argv[])
{  
	int q = Q;
	MPI_Init(&argc, &argv);

	MPI_Comm intercomm; 
	MPI_Comm_get_parent(&intercomm);


	while (q > 0) {

		if (q % 2 == 1) {

			Slavematmul(intercomm);
		}

		Slavematmul(intercomm);

		q = q >> 1;
	}

	

	MPI_Finalize();
	return 0;
}
