#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

#define ROWS 8
#define COLS 8

void printArray(double *a, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			printf("%.2f ", *(a + i * cols + j));
		}
		printf("\n");
	}
	printf("\n\n\n");
}

double *makeArray(int rows, int cols, int value, int p, int stripeSize, int is_b)
{
	double *arr = (double *)malloc(rows * cols * sizeof(double));

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (value == -1) {
				if(is_b) {
					*(arr + r * cols + c) = (double)((ROWS+1)*(c+2*p));
				} else {
					*(arr + r * cols + c) = (double)((ROWS+1)*c);
				}
			}
			else
			{
				*(arr + r * cols + c) = value;
			}
		}
	}

	return arr;
}

int min(int i, int j)
{
	return i < j ? i : j;
}

void MatrixMul(double *c, double *a, double *b, int stripeSize, int msg_from, int p)
{
	// if (p == 1) {
	// 			printArray(a, stripeSize, COLS);
	// 	printArray(b, COLS, stripeSize);
	// }
	
	for (int i = 0; i < stripeSize; i++)
	{
		for (int j = 0; j < stripeSize; j++)
		{
			double comp = 0.;
			for (int k = 0; k < COLS; k++)
			{
				// if (p == 1) {
				// 	printf("a: %f, b: %f\n", *(a + i * COLS + k), *(b + k * stripeSize + j));
				// }
				comp += *(a + i * COLS + k) * *(b + k * stripeSize + j);
			}
			// if (p == 1) {
			// 	printf("loc: %d, comp: %f\n\n\n", i * COLS + j, comp);
			// }
			
			// *(c + i*COLS + j) = comp;
			*(c + i * COLS + j + msg_from*stripeSize) = comp;
		}
	}
}

int main(int argc, char *argv[])
{

	MPI_Init(&argc, &argv);

	int world_size = 4;
	int p;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &p);
	MPI_Get_processor_name(processor_name, &name_len);

	const int stripeSize = COLS / world_size;

	double *a = makeArray(stripeSize, COLS, -1, p, stripeSize, 0);
	double *b = makeArray(ROWS, stripeSize, -1, p, stripeSize, 1);
	double *c = makeArray(stripeSize, COLS, 0, p, stripeSize, 0);

	int msg_from = p;
	double t1, t2;
	t1 = MPI_Wtime();

	if (p == 1)
	{
		printArray(a, stripeSize, COLS);
		printArray(b, ROWS, stripeSize);
	}

	// initial matrix multiplication
	MatrixMul(c, a, b, stripeSize, msg_from, p);

	if (p == 1)
	{
		printArray(c, stripeSize, COLS);
	}

	int received_from = p;

	// pass data
	double *received_b = (double *)malloc(ROWS * stripeSize * sizeof(double));
	double *temp;
	int size = ROWS * stripeSize;

	msg_from = p - 1;
	if (msg_from < 0)
	{
		msg_from = world_size + msg_from;
	}
	int send_to = (p + 1) % world_size;

	for (int i = 0; i < world_size - 1; i++){
		received_from = received_from - 1;
		if (received_from < 0){
			received_from = world_size + received_from;
		}

		if (p % 2 == 0){
			MPI_Recv(received_b, size, MPI_DOUBLE, msg_from, 0, MPI_COMM_WORLD,
					 MPI_STATUS_IGNORE);
		} else {
			MPI_Send(b, size, MPI_DOUBLE, send_to, 0,
					 MPI_COMM_WORLD);
		}

		if (p % 2 != 0) {
			MPI_Recv(received_b, size, MPI_DOUBLE, msg_from, 0, MPI_COMM_WORLD,
					 MPI_STATUS_IGNORE);
			// printf("Process %d received token %f, t %f from process %d\n", p, received_b[0],
			// 	   received_b[1], msg_from);
		} else {
			MPI_Send(b, size, MPI_DOUBLE, send_to, 0,
					 MPI_COMM_WORLD);
		}

		temp = received_b;
		received_b = b;
		b = temp;

		if (p == 1)
		{
			printf("Process %d received token %f, t %f from process %d\n", p, received_b[0],
				received_b[1], received_from);
			printArray(b, ROWS, stripeSize);
		}

		// matrix multiplication after receives
		MatrixMul(c, a, b, stripeSize, received_from, p);
		
		// if (p == 1)
		// {
		// 	printArray(c, stripeSize, ROWS);
		// }
	}

	// if (p == 1)
	// {
	// 	printArray(c, stripeSize, COLS);
	// 	printArray(c, ROWS, stripeSize);
	// }

	double *final_c = makeArray(ROWS, COLS, 0, p, stripeSize, 0);
	MPI_Gather(c, size, MPI_DOUBLE, final_c, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	t2 = MPI_Wtime();
	printf("Elapsed time is %f\n", t2 - t1);

	printArray(final_c, ROWS, COLS);

	MPI_Finalize();
	return EXIT_SUCCESS;
}
