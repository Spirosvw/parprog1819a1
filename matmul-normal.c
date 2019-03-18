#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>




void get_walltime(double *wct);

int main(int argc, char *argv[]) {
	int i, j, k;



	float *a, *b, *c;
	a = (float*)malloc(N*N*sizeof(float));
	b = (float*)malloc(N*N*sizeof(float));
	c = (float*)malloc(N*N*sizeof(float));

	for(i=0; i<N*N; i++) {
		a[i] = 2.0;
		b[i] = 3.0;
		c[i] = 20.0;
	}
	

	double ts, te;
	get_walltime(&ts);



	float *pa, *pb, *pc, sum;
	pc = c;
	for(k=0; k<N; k++) {
		pb = b;
		for(j=0; j<N; j++) {
			pa = a + k*N;
			sum = 0;
			for(i=0; i<N; i++) {
				sum = sum + (*pa) * (*pb);
				pa++; pb++;
			}
			*pc = sum;
			pc++;
		}
	}
	
	get_walltime(&te);
	
	float mflops = (unsigned long)N * N * N / ((te - ts) * 1e6);
	printf("mflops: %f %lf\n", mflops,(te-ts));
	

	for(i=0; i<N*N; i++) {
		if(c[i] != 2.0*3.0*N) {
			return 1;
		}
	}


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
