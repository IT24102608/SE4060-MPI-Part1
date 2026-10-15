#include <cstdio>
#include <random>
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long total_iterations = 10000000;

    long long local_iterations =
        total_iterations / size;

    if (rank == size - 1)
    {
        local_iterations =
            total_iterations -
            (local_iterations * (size - 1));
    }

    std::mt19937 generator(12345 + rank);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    long long local_inside = 0;

    MPI_Barrier(MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    for (long long i = 0; i < local_iterations; i++)
    {
        double x = distribution(generator);
        double y = distribution(generator);

        if ((x * x + y * y) <= 1.0)
        {
            local_inside++;
        }
    }

    long long total_inside = 0;

    MPI_Reduce(
        &local_inside,
        &total_inside,
        1,
        MPI_LONG_LONG,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    double end_time = MPI_Wtime();

    if (rank == 0)
    {
        double pi =
            4.0 *
            static_cast<double>(total_inside) /
            static_cast<double>(total_iterations);

        printf("Total iterations = %lld\n",
               total_iterations);

        printf("Points inside circle = %lld\n",
               total_inside);

        printf("Estimated Pi = %.12f\n",
               pi);

        printf("Number of processes = %d\n",
               size);

        printf("Execution time = %.10f seconds\n",
               end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}