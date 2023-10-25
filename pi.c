#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUMSTEPS 1000000
#include <mpi.h>



int main() {

	int myrank;//rank
	int numofprocs;//number of processes
	int source;
	double totalpi;


	MPI_Init(NULL,NULL); // mpi initilization

	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numofprocs);
	

	double local_x;
	int i;
        double pi, sum = 0.0;
        struct timespec start, end;

        clock_gettime(CLOCK_MONOTONIC, &start);


	double localstep = NUMSTEPS/numofprocs;
        double step = 1.0/(double) NUMSTEPS;   //change in  x value
       local_x =(myrank+ 0.5) * step;

        for (i=myrank;i<NUMSTEPS; i+= numofprocs){
                local_x += step*numofprocs;

                sum += 4.0/(1.0+local_x*local_x); // y value 
        }


        pi = step * sum;// size of rectangle 
        clock_gettime(CLOCK_MONOTONIC, &end);
        u_int64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;




	if(myrank != 0){
	
	MPI_Send(&pi, 1, MPI_DOUBLE , 0, 0, MPI_COMM_WORLD);
	}
	else{
	totalpi = pi;
	for(source = 1; source < numofprocs; source++){
	
	MPI_Recv(&pi, 1, MPI_DOUBLE, source,0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	totalpi += pi;
	}

	}


	if(myrank == 0){

	printf("PI is %.20f\n",totalpi);
	printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
	


	}







        
	MPI_Finalize(); // no more mpi
	return 0;
}

