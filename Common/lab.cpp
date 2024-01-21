#include "./lab.h"

double Point::distance(const Point& a)
{
    return std::sqrt(a.x * a.x + a.y * a.y);
}

double Point::distance(const Point& a, const Point& b)
{
	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Point::Point() : x(0.0), y(0.0) {}

Point::Point(double x, double y) : x(x), y(y) {}


constexpr auto WIDTH = 30;
constexpr auto HEIGHT = 15;

// Функция для визуализации точек
void Point::visualize(const std::vector<Point>& points)
{
    int max = 0;

    // Поиск максимума
    for (const auto& point : points) {
        if (point.x > max) max = std::ceil(point.x);
        if (point.y > max) max = std::ceil(point.y);
    }

    // Создаем сетку для визуализации
    std::vector<int> grid(HEIGHT * WIDTH, 0);

    // Масштабируем и помещаем точки в сетку
    for (const auto& point : points) {
        int x = point.x / max * WIDTH;
        int y = point.y / max * HEIGHT;
        int pos = y * WIDTH + x;
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) grid[pos]++;
    }

    // Визуализация сетки
    std::cout << "^" << max << std::endl;
    for (int y = HEIGHT - 1; y >= 0; y--) {
        std::cout << "|";
        for (int x = 0; x < WIDTH; x++) {
            int pos = y * WIDTH + x;
            char symbol = ' ';
            if (grid[pos] > 0) {
                if (grid[pos] < 3) symbol = '.';
                else if (grid[pos] < 5) symbol = '*';
                else symbol = '#';
            }
            std::cout << symbol;
        }
        std::cout << std::endl;
    }
    std::cout << "0";
    for (int x = 0; x < WIDTH; x++) std::cout << "-";
    std::cout << ">" << max << std::endl;
}
