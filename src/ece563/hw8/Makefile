# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

CC = mpicc
CFLAGS = -std=c11
INCLUDES = -I../include
LFLAGS = -L../lib
LIBS = -lm -fopenmp

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXECS = $(SRCS:%.c=%)

.PHONY: all mm clean

all: ${EXECS}
	@echo  sources compiled

${EXECS}:  %: %.o
	@echo "creating the executable.."
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $< $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(LIBS)

clean:
	@echo "removing executables.."
	$(RM) *.o *~ $(EXECS)
