#include <mpi.h>
#include <iostream>
#include <cmath>

using namespace std;

bool isPrime(int number) {
    if (number <= 1) return false;
    if (number == 2) return true;
    if (number % 2 == 0) return false;
    for (int i = 3; i <= sqrt(number); i += 2) {
        if (number % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    int start = 1, end = 100000000;

    MPI_Init(&argc, &argv);

    int rank, size;
    double startTime, endTime;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    startTime = MPI_Wtime();

    int range = end - start + 1;
    int chunk = range / size;
    int local_start = start + rank * chunk;
    int local_end = (rank == size - 1) ? end : local_start + chunk - 1;

    int local_count = 0;
    for (int i = local_start; i <= local_end; ++i) {
        if (isPrime(i)) {
            ++local_count;
        }
    }

    int global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    endTime = MPI_Wtime();

    if (rank == 0) {
        cout << "Total prime numbers: " << global_count << endl;
        cout << "Execution time: " << endTime - startTime << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}