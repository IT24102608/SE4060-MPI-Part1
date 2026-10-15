#include <cstdio>
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long N = 10000000;

    long long chunk = N / size;

    long long start = rank * chunk + 1;
    long long end;

    if (rank == size - 1)
        end = N;
    else
        end = (rank + 1) * chunk;

    long long local_sum = 0;

    double start_time = MPI_Wtime();

    for (long long i = start; i <= end; i++)
    {
        local_sum += i;
    }

    long long total_sum = 0;

    MPI_Reduce(
        &local_sum,
        &total_sum,
        1,
        MPI_LONG_LONG,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    double end_time = MPI_Wtime();

    printf(
        "Rank %d calculated from %lld to %lld. Local sum = %lld\n",
        rank,
        start,
        end,
        local_sum
    );

    if (rank == 0)
    {
        printf("\nTotal sum = %lld\n", total_sum);
        printf("Number of processes = %d\n", size);
        printf("Execution time = %.10f seconds\n",
               end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}