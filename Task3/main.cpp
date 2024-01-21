#include <omp.h>
#include "../Common/lab.h"

constexpr auto MAX = 100;
constexpr auto R = 6;
constexpr auto C = 4;

int main() {
    std::srand(std::time(nullptr));

    std::vector<std::vector<int>> matrix(R, std::vector<int>(C));

    srand(time(nullptr));
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < C; ++j)
            matrix[i][j] = rand() % MAX;

    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) 
            std::cout
            << std::setfill('0') << std::setw(2)
            << matrix[i][j]
            << " ";
        std::cout << std::endl;
    }

    int max_sum = 0;

    #pragma omp parallel for reduction(max:max_sum)
    for (int i = 0; i < R; ++i) {
        int sum = 0;
        for (int j = 0; j < C; ++j) 
            sum += matrix[i][j];
        max_sum = std::max(max_sum, sum);

        #pragma omp critical
        std::cout << "[thread " << omp_get_thread_num() << "] Sum: " << sum << std::endl;
    }

    std::cout << "Norm: " << max_sum << std::endl;

    return 0;
}

/*
PS C:\Users\anton\Projects\MPI_Lab\x64\Debug> .\Task3.exe
Matrix:
80 08 85 94
59 99 56 19
63 69 48 17
56 89 43 96
79 23 97 93
46 17 66 85
[thread 0] Sum: 267
[thread 1] Sum: 233
[thread 2] Sum: 197
[thread 4] Sum: 292
[thread 5] Sum: 214
[thread 3] Sum: 284
Norm: 292
*/