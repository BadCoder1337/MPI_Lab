#include <omp.h>
#include "../Common/lab.h"

constexpr auto MAX = 100;
constexpr auto N = 500;

int main() {
    std::vector<Point> points;
    double min = std::numeric_limits<double>::infinity();
    double max = 0.0;

    std::srand(std::time(nullptr));
    for (int i = 0; i < N; ++i) {
        Point p(
            1.0 * MAX * std::rand() / RAND_MAX,
            1.0 * MAX * std::rand() / RAND_MAX
        );
        points.push_back(p);
    }

    Point::visualize(points);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double dist = Point::distance(points[i], points[j]);
            if (dist < min) 
            #pragma omp critical
            min = dist;

            if (dist > max)
            #pragma omp critical
            max = dist;
        }
    }

    std::cout << "Minimum distance: " << min << std::endl;
    std::cout << "Maximum distance: " << max << std::endl;

    return 0;
}

/*
PS C:\Users\anton\Projects\MPI_Lab\x64\Debug> .\Task4.exe
^100
|.   ...*... *  ..*... ....  .
| ..  .. *.  *.... . ....   ..#
|  *..*   . ... ...... .....  .
|*... .*.. .... *... *. *..*.
|... .*..  .  *. ....  .  . *.*
|* .. . .# .... .......*. .....
| . . .**... . .   .**.. .  ...
|.  ...  .  .  **    ... ... ..
|. .. .. .  . .*.*.. .. ..   .*
|.  *.... ....*...... .*. ....
|...... ....... ......*... .
|..*........ .  .....    *. . .
|* * . *..* * ..  ... ....   ..
| ...  *. . .*   .. . . .. ..**
|.. .. ... ..   .   .. .....
0------------------------------>100
Minimum distance: 0.196199
Maximum distance: 136.6
*/