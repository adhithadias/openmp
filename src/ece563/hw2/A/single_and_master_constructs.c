#include <stdio.h>
#include <omp.h>
#include <unistd.h>

void print_thread_info(char *dir) {
    int curr_thread, nthreads, nprocessors;
    int len = 30;
    char name [30];

    curr_thread = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    nprocessors = omp_get_num_procs();
    gethostname(name, len);
    printf("REGION:%s   Runhost:%s   Thread:%d of %d threads and processes:%d   hello, world\n", dir, name,curr_thread,nthreads,nprocessors);
    return;
}

/*  Example to check master construct and single construct directives
*   single and master constructs when specified, the statement is executed by only one thread
*   single construct, unless otherwise specified with nowait clause, wait at an implicit barrier
*   at the end of the single construct
*/
void single_and_master_dir() {

    print_thread_info("SERIAL");

    #pragma omp parallel
    {
        #pragma omp master
        {
            printf("\nMASTER THREAD\n");
            print_thread_info("=============MASTER THREAD");
        }
        // no implicit barrier after the master construct

        #pragma omp single
        {
            printf("\n==================BEGINNING WORK 1===================\n");
            print_thread_info("=============SINGLE 1");
        }
        // implicit barrier after the single construct

        print_thread_info("WORK 1");

        #pragma omp single
        {
            printf("\n====================FINISHED WORK 1 AND BEGINNING WORK 2.==========================\n");
            print_thread_info("=============SINGLE 2");
        }
        // implicit barrier -- all threads wait here until work 1 is finished by all the threads

        print_thread_info("WORK 2");
        

        #pragma omp single nowait
        {
            printf("=============LET WORK 3 TO EXECUTE WHILE WORK 2 IS BEING PROCESSED===================\n");
            print_thread_info("=============SINGLE NO WAIT");
        }
        // no wait clause removes the barrier -- work 3 and work 2 maybe run in parallel by different threads
        

        print_thread_info("WORK 3");
    }
}

int main (int argc, char *argv []) {

    single_and_master_dir();
    return 0;
}
