#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>




void get_walltime(double *wct);

int main(int argc, char *argv[]) {
	int i, j, k;


	//These are three pointers pointing at the first cell of an array. In this program i will multiply the arrays a and b and i will store the result in array c
	float *a, *b, *c;
	//By using malloc we can allocate the memory needed for the arrays 
	a = (float*)malloc(N*N*sizeof(float));
	b = (float*)malloc(N*N*sizeof(float));
	c = (float*)malloc(N*N*sizeof(float));

	//Array Initialisation
	for(i=0; i<N*N; i++) {
		a[i] = 2.0;
		b[i] = 3.0;
		c[i] = 20.0;
	}
	
	//Getting time before the multiplying process
	double ts, te;
	get_walltime(&ts);


	//these pointer show at one cell of each array 
	float *pa, *pb, *pc, sum;
	//pointer pc is set at the beginning of array c
	pc = c;
	for(k=0; k<N; k++) {
		//pointer pb is set everytime at the beginning of array b because it changes during the inner for-loop
		pb = b;
		for(j=0; j<N; j++) {
			//pointer pa is set everytime at the cell k*N of array  because it changes during the inner for-loop
			pa = a + k*N;
			sum = 0;
			for(i=0; i<N; i++) {
				//this is the result of the addition of all the multiplications of a line of array a with row of array c
				sum = sum + (*pa) * (*pb);
				//the pointers move one cell
				pa++; pb++;
			}
			//then the sum is stored in the pointed cell of the array c
			*pc = sum;
			//and the pointer pc moves one cell 
			pc++;
		}
	}
	
	//then we get the timestamp to compare it to the one before the multiplications
	get_walltime(&te);
	
	//I print the result
	float mflops = (unsigned long)N * N * N / ((te - ts) * 1e6);
	printf("mflops: %f %lf\n", mflops,(te-ts));
	
	//I check if the process was done correctly
	for(i=0; i<N*N; i++) {
		if(c[i] != a[i]*b[i]*N) {
			return 1;
		}
	}

	//Free allocated memory because we are good and responsible programmers :D
	free(a);
	free(b);
	free(c);

	return 0;
}

void get_walltime(double *wct) {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct = (double)(tp.tv_sec + tp.tv_usec / 1000000.0);
}
