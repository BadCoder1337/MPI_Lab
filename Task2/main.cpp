#include <mpi.h>
#include "../Common/lab.h"

constexpr auto MAX = 10;
constexpr auto N = 500;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> points;

    // Нулевой процесс генерирует случайные точки
    if (rank == 0) {
        std::srand(std::time(nullptr));
        for (int i = 0; i < N; ++i) {
            Point p(
                1.0 * MAX * std::rand() / RAND_MAX,
                1.0 * MAX * std::rand() / RAND_MAX            
            );
            points.push_back(p);
        }

        Point::visualize(points);
    }

    // Распределение точек по процессам
    int points_per_proc = N / size;
    std::vector<Point> local_points(points_per_proc);

    MPI_Scatter(points.data(), points_per_proc * sizeof(Point), MPI_BYTE,
        local_points.data(), points_per_proc * sizeof(Point), MPI_BYTE, 0, MPI_COMM_WORLD);

    // Находим максимально и минимально удаленные точки в каждом процессе
    double max_dist = -1.0, min_dist = std::numeric_limits<double>::infinity();
    Point local_max, local_min;
    for (const auto& p : local_points) {
        double dist = Point::distance(p);
        if (dist > max_dist) {
            max_dist = dist;
            local_max = p;
        }
        if (dist < min_dist) {
            min_dist = dist;
            local_min = p;
        }
    }

    // Сбор результатов на нулевом процессе
    std::vector<Point> all_max_points(size), all_min_points(size);
    MPI_Gather(&local_max, sizeof(Point), MPI_BYTE, all_max_points.data(), sizeof(Point), MPI_BYTE, 0, MPI_COMM_WORLD);
    MPI_Gather(&local_min, sizeof(Point), MPI_BYTE, all_min_points.data(), sizeof(Point), MPI_BYTE, 0, MPI_COMM_WORLD);

    // Вывод результатов
    if (rank == 0) {
        Point global_max = all_max_points[0];
        Point global_min = all_min_points[0];
        for (int i = 1; i < size; ++i) {
            if (Point::distance(all_max_points[i]) > Point::distance(global_max)) 
                global_max = all_max_points[i];
            if (Point::distance(all_min_points[i]) < Point::distance(global_min)) 
                global_min = all_min_points[i];
        }

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Farthest point: (" << std::setw(5) << global_max.x << ", " << std::setw(5) << global_max.y << ")" << std::endl;
        std::cout << "Nearest point: (" << std::setw(5) << global_min.x << ", " << std::setw(5) << global_min.y << ")" << std::endl;
    }

    MPI_Finalize();
    return 0;
}

/*
PS C:\Users\anton\Projects\MPI_Lab\x64\Debug> mpiexec -n 10 .\Task2.exe
^10
|*.* .**. ...   *..* ...#...* .
| .#..... .... ...*.  . . .. .
| .. .      ... . . .. ..   ...
|..  . . **  . .. ..   .*  ....
|.   . ..  * ..*  .....* . *.*.
|.* *. ..#...  . ......  ..  .#
|... ..*  .... .. ** .. ... *..
| *   .  .  .. .. .. ...* ..*..
| .**  ... .. *   .*. ...  ..
| ..*  *     ....  ..  .... ...
|..*. ... * .   . *   ....#   .
|. .* ......... . .. .**.*   .
|  .. .*.. ... ...  .... .. *.
|*.* .. . .  .  ..  ..* *.... .
|. .. .... *...   .....  . * ..
0------------------------------>10
Farthest point: ( 9.78,  9.37)
Nearest point: ( 0.06,  0.27)
*/