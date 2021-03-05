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



```bash
# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

CC = gcc                        # compiler to use

LINKERFLAG = -lm

SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

all: ${BINS}

%: %.o
    @echo "Checking.."
    ${CC} ${LINKERFLAG} $< -o $@

%.o: %.c
    @echo "Creating object.."
    ${CC} -c $<

clean:
    @echo "Cleaning up..."
    rm -rvf *.o ${BINS}
```