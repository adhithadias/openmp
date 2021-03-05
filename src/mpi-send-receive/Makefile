# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

EXECS=mpi_send_receive
MPICC?=mpicc

.PHONY: all mpi_send_receive clean

all: ${EXECS}

mpi_send_receive: mpi_send_receive.c
	@echo "creating the executable.."
	${MPICC} -o mpi_send_receive mpi_send_receive.c

clean:
	@echo "removing executables.."
	rm ${EXECS}
