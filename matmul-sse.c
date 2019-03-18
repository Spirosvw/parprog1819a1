#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>


void get_walltime(double *wct);

int main(int argc, char *argv[]) {
    int i, j, k;
    //The result of the multiplication of arrays a and b will be stored to array c. The array sumArray is used as a temproraty storing point of the multiplication when using the sse
    float *a, *b, *c , *pc,*sumArray;
    //The sum will contain temproraty sum of all the elements of the array sumArray
    float sum = 0;
    //128 bit pointers
    __m128 *va, *vb, *vsum;
    //Allocating memory
    int checkA = posix_memalign((void**)&a, 16, N*N*sizeof(float));
    if(checkA!=0) {
        exit(1);
    }
    int checkB = posix_memalign((void**)&b, 16, N*N*sizeof(float));
    if(checkB!=0) {
        free(a);
        exit(1);
    }
    int checkC = posix_memalign((void**)&c, 16, N*N*sizeof(float));
    if(checkC!=0) {
        free(a);
        free(b);
        exit(1);
    }
    //the sumArray needs to contain only 4 elements
    int checkSum = posix_memalign((void**)&sumArray, 16, 4*sizeof(float));
    if(checkSum!=0) {
        free(a);
        free(b);
        free(c);
        exit(1);
    }


    //Initialising array elements
    for(i=0; i<N*N; i++) { 
		a[i] = 2.0;
		b[i] = 3.0;
		c[i] = 0.0;
    }

    //each pointer now points at the beginning of the array they were set to.
    va = (__m128*) a;
    vb = (__m128*) b;
    vsum = (__m128*) sumArray;
    //pointer pc is set at the beginning of array c
    pc = c;

    //Getting time at start of process
    double ts, te;
	get_walltime(&ts);

    for (i=0; i<N; i++){
        //everytime the outer for-loop advances by one we need to set the vp pointer to the beginning of the array b, because it reached the last element during the inner for-loop
        vb = (__m128*) b;
        for (j=0; j<N; j++){
            //the va pointer needs to point at the i*N element of the array a
            va = (__m128*)(a+i*N);
            
            //inside this for-loop we send 4 float elements of the array a and 4 float items of the array b to the processor for multiplying using the SSE
            for (k=0; k<N; k+=4){
                //the results are stored in this array called sumArray
                *vsum = _mm_mul_ps(*va,*vb);
                //we add every element of the sumArray and we store it to a variable
                sum = sum + sumArray[0] + sumArray[1] + sumArray[2] + sumArray[3];
                //also we icrease the va and vb in order to get the next 4 elements of each array
                va++;
                vb++;
            }
            //then the sum is stored in the array c through the pointer pc
            *pc = sum;
            pc++;
            //and the sum variable is set to 0 in order to be used on the next multiplication
            sum = 0;

        }
    }

    //get time after process was finished
    get_walltime(&te);
	

    //Results
    float mflops = (unsigned long)N * N * N / ((te - ts) * 1e6);
    printf("mflops: %f , time: %lf\n", mflops,(te - ts));
	


    //Check if everything went as planned
    for(i=0; i<N; i++) {
     	if(c[i] != a[i] * b[i] * N) {
		printf("Something went wrong. Check your code \n");
		break;
	}
    }

    //Free allocated memory because we are good and responsible programmers :D
    free(a);
    free(b);
    free(c);
    free(sumArray);

    return 0;

}






void get_walltime(double *wct) {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct = (double)(tp.tv_sec + tp.tv_usec / 1000000.0);
}


