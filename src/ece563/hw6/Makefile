# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

EXECS=mm
MPICC?=mpicc

.PHONY: all mm clean

all: ${EXECS}

mm: mpi_mm.c omp_mm.c
	@echo "creating the executable.."
	${MPICC} -o mpi_mm mpi_mm.c
	${MPICC} -o omp_mm omp_mm.c

clean:
	@echo "removing executables.."
	rm omp_mm mpi_mm
