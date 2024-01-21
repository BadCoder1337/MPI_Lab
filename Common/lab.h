#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <limits>

struct Point {
    double x, y;

    static double distance(const Point& a);
    static double distance(const Point& a, const Point& b);
    static void visualize(const std::vector<Point>& points);

    Point();
    Point(double x, double y);
};

constexpr auto TEXT_BLACK = "\033[30m";;
constexpr auto TEXT_WHITE_BG = "\033[107m";;
constexpr auto TEXT_RESET = "\033[0m";;