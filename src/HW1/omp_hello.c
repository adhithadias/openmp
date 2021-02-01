/*  C  */

#include <stdio.h>
#include <unistd.h>
#include <omp.h>

int main () {

    /* Serial Region  (master thread)                                */
    /* Parameters of the Application                                 */
    int len=30;
    char name[30];

    /* OpenMP Parameters */
    int id, nthreads, nprocessors;

    /* Master thread obtains information about itself and its environment. */
    nthreads = omp_get_num_threads();       /* get number of threads */
    id = omp_get_thread_num();              /* get thread            */
    nprocessors = omp_get_num_procs();
    gethostname(name,len);                  /* get run-host name     */
    printf("SERIAL REGION:     Runhost:%s   Thread:%d of %d thread and processes:%d   hello, world\n", name,id,nthreads,nprocessors);

    /* Open parallel region. */
    /* Each thread obtains information about itself and its environment. */
    #pragma omp parallel private(name,id,nthreads)
    {nthreads = omp_get_num_threads();      /* get number of threads */
     id = omp_get_thread_num();             /* get thread            */
     nprocessors = omp_get_num_procs();
     gethostname(name,len);                 /* get run-host name     */
     printf("PARALLEL REGION:   Runhost:%s   Thread:%d of %d threads and processes:%d   hello, world\n", name,id,nthreads,nprocessors);
    }
    /* Close parallel region. */

    /* Serial Region  (master thread)                                */
    printf("SERIAL REGION:     Runhost:%s   Thread:%d of %d thread and processes:%d    hello, world\n", name,id,nthreads,nprocessors);
    return 0;
}
