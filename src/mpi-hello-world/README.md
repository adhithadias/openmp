## Executing the file
```bash
# to create the executable
make

# to run the executable
mpirun -n 4 ./mpi_hello_world

# run processes more than the number that is available
mpirun -n 8 --use-hwthread-cpus ./mpi_hello_world
mpirun -n 8 --oversubscribe ./mpi_hello_world
```

## Reference
1. https://mpitutorial.com/tutorials/mpi-hello-world/
2. https://github.com/mpitutorial/mpitutorial
3. https://opensource.com/article/18/8/what-how-makefile