# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

EXECS=mpi_hello_world
MPICC?=mpicc

.PHONY: all mpi_hello_world clean

all: ${EXECS}

mpi_hello_world: mpi_hello_world.c
	@echo "creating the executable.."
	${MPICC} -o mpi_hello_world mpi_hello_world.c

clean:
	@echo "removing executables.."
	rm ${EXECS}
