#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void p0_set_input(int *input1, int *input2) {
    *input1 = 10000000;
    *input2 = 100000;
    printf ( "\nP0_SET_PARAMETERS:\n Set INPUT1 = %d\n INPUT2 = %d\n",
    *input1 , *input2 ) ;
}
void p0_send_input(int input1, int input2) {
    int id=1, tag=1;
    MPI_Send(&input1, 1, MPI_INT, id, tag, MPI_COMM_WORLD) ;
    id = 2; tag = 2;
    MPI_Send(&input2, 1, MPI_INT, id, tag, MPI_COMM_WORLD) ;
}
void p0_receive_output(int *output1, int *output2) {
    int output, output_received=0, source;
    MPI_Status status;
    while(output_received<2) {
        MPI_Recv(&output, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
        MPI_COMM_WORLD, &status) ;
        source = status. MPI_SOURCE;
        if (source == 1) *output1 = output;
        else *output2 = output;
        output_received++;
    }
    printf("\n Process 1 returned OUTPUT1 = %d\nProcess 2 returned OUTPUT2 = %d\n", *output1, *output2) ;
}
int p1_receive_input() {
    int id=0, input1, tag=1; MPI_Status status;
    MPI_Recv(&input1, 1, MPI_INT, id, tag, MPI_COMM_WORLD, &status) ;
    return input1;
}
int p1_compute_output(int input1) {
    int i, j , k, output1=0;
    for(i = 2; i <= input1; i++) {
        j = i; k = 0;
        while(1 < j ) {
            if((j % 2) == 0) j = j / 2;
            else j = 3 * j + 1;
            k++;
        } if(output1 < k) output1 = k;
    }
    return output1;
}
void p1_send_output(int output1) {
    int id=0, tag=3; MPI_Send ( &output1, 1, MPI_INT, id, tag,
    MPI_COMM_WORLD ) ;
}
int p2_receive_input() {
    int id=0, input2, tag = 2; MPI_Status status;
    MPI_Recv ( &input2, 1, MPI_INT, id, tag, MPI_COMM_WORLD, &status ) ;
    return input2;
}
int p2_compute_output(int input2) {
    int i, j , output2=0, prime;
    for(i = 2; i <= input2; i++) {
        prime = 1;
    for(j = 2; j < i; j ++) {
        if(i % j == 0) {
            prime = 0; break;
        }
    }
    }
    return output2;
}
void p2_send_output(int output2) {
    int id=0, tag = 4; MPI_Send(&output2, 1, MPI_INT, id, tag, MPI_COMM_WORLD) ;
}

void timestamp() {
#define TIME_SIZE 40
    static char time_buffer[TIME_SIZE] ;
    const struct tm *tm;
    time_t now;
    now = time(NULL) ;
    tm = localtime(&now) ;
    strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm) ;
    printf("%s\n", time_buffer) ;
#undef TIME_SIZE
}

int main ( int argc, char **argv ) {
    int id, ierr, input1, input2, output1, output2, p;
    double wtime;
    ierr = MPI_Init(&argc, argv) ;
    if ( ierr != 0 ) {
        printf("\nMPI_MULTITASK - Fatal error!\nMPI_Init returned nonzero IERR.\n") ;
        exit(1) ;
    }
    ierr = MPI_Comm_rank(MPI_COMM_WORLD,&id) ; MPI_Comm_size(MPI_COMM_WORLD, &p) ;
    if (p < 3) {
        printf("\nMPI_MULTITASK-Fatal error!\nNumber of available processes must be at least 3!\n" ) ;
        MPI_Finalize() ;
        exit(1) ;
    }
    if(id == 0) {
        timestamp() ;
        printf ("\nMPI_MULTITASK:\nC / MPI version\n") ;
        wtime = MPI_Wtime() ;
        p0_set_input(&input1, &input2) ;
        p0_send_input(input1, input2) ;
        p0_receive_output(&output1, &output2) ;
        wtime = MPI_Wtime() - wtime;
        printf(" Process 0 time = %g\n", wtime) ;
        MPI_Finalize() ;
        printf("\nMPI_MULTITASK:\n Normal end of execution.\n") ;
        timestamp() ;
    }
    else if (id == 1) {
        wtime = MPI_Wtime() ;
        input1 = p1_receive_input() ;
        output1 = p1_compute_output(input1) ;
        p1_send_output(output1) ;
        wtime = MPI_Wtime() -wtime;
        printf(" Process 1 time = %g\n", wtime) ;
        MPI_Finalize() ;
        int id=0, input1, tag=1; MPI_Status status;
    }
    else if (id == 2) {
        wtime = MPI_Wtime() ;
        input2 = p2_receive_input() ;
        output2 = p2_compute_output(input2) ;
        p2_send_output (output2) ;
        wtime = MPI_Wtime() - wtime;
        printf(" Process 2 time = %g\n", wtime) ;
        MPI_Finalize() ; 
    }
    else
        MPI_Finalize() ;
}
