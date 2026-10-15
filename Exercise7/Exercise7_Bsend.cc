#include <cstdio>
#include <cstdlib>
#include <mpi.h>

int main(void)
{
    int rank;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char name[30];
    int len;
    MPI_Get_processor_name(name, &len);

    int x[10], y[10];

    if (rank == 1)
    {
        for (int r = 0; r < 10; r++)
            x[r] = 10 * r;

        int buffer_size;
        MPI_Pack_size(10, MPI_INT, MPI_COMM_WORLD, &buffer_size);
        buffer_size += MPI_BSEND_OVERHEAD;

        void* buffer = malloc(buffer_size);

        MPI_Buffer_attach(buffer, buffer_size);

        printf("Sending message to computer 3 from computer 1\n");

        MPI_Bsend(x, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);

        MPI_Buffer_detach(&buffer, &buffer_size);

        free(buffer);
    }
    else if (rank == 3)
    {
        MPI_Recv(y, 10, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        printf("In computer 3 the value of y is printed\n");

        for (int r = 0; r < 10; r++)
            printf(" %d ", y[r]);

        printf("\n");
    }
    else
    {
        printf("Just a normal process From rank %d machine %s\n",
               rank, name);
    }

    MPI_Finalize();

    return 0;
}