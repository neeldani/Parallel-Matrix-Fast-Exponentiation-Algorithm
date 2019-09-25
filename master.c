#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "config.h"

int *getMatrix(int m, int n) {

	/*
		returns a matrix of size m, n
	*/

	int *arr = (int*)malloc(sizeof(int)*m*n);
	return arr;
}

int *getIdentityMatrix(int m) {

	/*
		returns an identity matrix of 
		size m, n
	*/

	int *I = getMatrix(m, m);

	for(int i=0; i<m; i++) {

		for(int j=0; j<m; j++) {

			if (i == j)
				I[i*m + j] = 1;

			else
				I[i*m + j] = 0;
		}
	}

	return I;
}

int *Mastermatmul(int *result, int *A, MPI_Comm intercomm) {

	/*
		Broadcasts the matrix "result".
		Scatters the matrix "A".
		Gathers the matrix sent by each slave process.
	*/
	
	int *buf = getMatrix(M, M); // redundant for master.

	int n_rows = M*M / P; // no. of rows to be sent to each child
	MPI_Bcast(A, M*M, MPI_INT, MPI_ROOT, intercomm);
	MPI_Scatter(result, n_rows, MPI_INT, buf, n_rows, MPI_INT, MPI_ROOT, intercomm);
	MPI_Gather(&buf, n_rows, MPI_INT, result, n_rows, MPI_INT, MPI_ROOT, intercomm);
	
	return result;
}	

int main(int argc, char *argv[])
{
	MPI_Comm intercomm;
	int q = Q;

	if (M % P != 0) {
		perror("M (dimension of matrix) should be divisible by P (no. of slave processes spawned");
		exit(0);
	}


	MPI_Init(&argc, &argv);

	// spawning P no. of slave processes.
	MPI_Comm_spawn("worker_program", MPI_ARGV_NULL, P, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE); 

	// initialise result and A 
	int *result = getIdentityMatrix(M);
	int *A = getMatrix(M, M);

	printf("Enter a matrix of dimension %d x %d\n", M, M);
	for(int i=0; i<M*M; i++)
   		scanf("%d", &A[i]);

   	printf("\nInput matrix:\n");
   	fflush(stdout);

   	for(int i=0; i<M; i++) {

   		for(int j=0; j<M; j++){
   			
   			printf("%d ", A[i*M + j]);
   			fflush(stdout);
   		}

   		printf("\n");
   		fflush(stdout);
   	}

   	printf("\n");
   	fflush(stdout);



   	printf("Power to be calculated: %d\n\n", Q);
   	fflush(stdout);

	while(q > 0) {
   		
   		if (q % 2 == 1) {
   			result = Mastermatmul(result, A, intercomm);
   		}

   		A = Mastermatmul(A, A, intercomm);
   		q = q >> 1;
	}



	printf("Output matrix:\n");
   	fflush(stdout);

	for(int i = 0; i < M; i++) {
   		
   		for(int j = 0; j < M; j++) {
   			printf("%d ", result[i*M + j]);
   			fflush(stdout);
   		}

   		printf("\n");
   		fflush(stdout);
	}

	MPI_Finalize();
	return 0;
}