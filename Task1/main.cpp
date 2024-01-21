#include <mpi.h>
#include "../Common/lab.h"

constexpr auto MAX = 100;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> matrix(size * size);
    std::vector<int> row(size);
    int diagonal_element;

    // Инициализация и заполнение матрицы на процессе 0
    if (rank == 0) {
        // Заполнение матрицы случайными значениями
        srand(time(nullptr));
        for (int i = 0; i < size * size; ++i)
            matrix[i] = rand() % MAX; // Случайные значения от 0 до 99
        std::cout << "Matrix:" << std::endl;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int index = i * size + j;
                if (i == j)
                    std::cout 
                        << TEXT_BLACK << TEXT_WHITE_BG
                        << std::setfill('0') << std::setw(2)
                        << matrix[index]
                        << TEXT_RESET 
                        << " ";
                else
                std::cout 
                    << std::setfill('0') << std::setw(2)
                    << matrix[index] 
                    << " ";
            }
            std::cout << std::endl;
        }
    }

    // Рассылка строк матрицы соответствующим процессам
    MPI_Scatter(matrix.data(), size, MPI_INT, row.data(), size, MPI_INT, 0, MPI_COMM_WORLD);

    // Извлечение элемента диагонали на каждом процессе
    diagonal_element = row[rank];

    // Сбор диагональных элементов на процессе 0
    std::vector<int> diagonal;
    if (rank == 0) {
        diagonal.resize(size);
    }
    MPI_Gather(&diagonal_element, 1, MPI_INT, diagonal.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Вывод диагональных элементов на процессе 0
    if (rank == 0) {
        std::cout << "Diagonal elements: ";
        for (int i = 0; i < size; ++i) 
            std::cout 
                << std::setfill('0') << std::setw(2)
                << diagonal[i] 
                << " ";
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}

/*
PS C:\Users\anton\Projects\MPI_Lab\x64\Debug> mpiexec -n 4 .\Task1.exe
Matrix:
78 31 87 62
95 91 69 81
21 52 70 76
27 66 12 78
Diagonal elements: 78 91 70 78
*/