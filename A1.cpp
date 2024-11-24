#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>
#include <locale>

// Структура для описания окружности.
struct Circle {
    long double x;
    long double y;
    long double r;

    bool contains(long double px, long double py) const {
        long double dx = px - x;
        long double dy = py - y;
        return (dx * dx + dy * dy) <= (r * r);
    }
};

bool is_point_inside_intersection(long double x, long double y, const Circle &c1, const Circle &c2, const Circle &c3) {
   return c1.contains(x, y) && c2.contains(x, y) && c3.contains(x, y);
}

long double intersection_area(int n, long double scale, const Circle &c1, const Circle &c2, const Circle &c3) {
    // Определение границ области генерации с учётом масштаба.
    long double max_x = std::max(c1.x + c1.r, std::max(c2.x + c2.r, c3.x + c3.r)) * scale;
    long double min_x = std::min(c1.x - c1.r, std::min(c2.x - c2.r, c3.x - c3.r)) * scale;
    long double max_y = std::max(c1.y + c1.r, std::max(c2.y + c2.r, c3.y + c3.r)) * scale;
    long double min_y = std::min(c1.y - c1.r, std::min(c2.y - c2.r, c3.y - c3.r)) * scale;

    // Настройка генератора случайных чисел.
    std::mt19937 generator(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<long double> rndx(min_x, max_x);
    std::uniform_real_distribution<long double> rndy(min_y, max_y);

    int k = 0; // Счётчик точек внутри пересечения.

    for(int i = 0; i < n; ++i){
        long double x = rndx(generator);
        long double y = rndy(generator);
        if(is_point_inside_intersection(x, y, c1, c2, c3)){
            k++;
        }
    }

    long double area_rect = (max_x - min_x) * (max_y - min_y);
    return (static_cast<long double>(k) / n) * area_rect;
}

int main() {
// Заданные окружности.
Circle c1 = {1.0, 1.0, 1.0};
Circle c2 = {1.5, 2.0, std::sqrt(5.0)/2.0};
Circle c3 = {2.0, 1.5, std::sqrt(5.0)/2.0};

// Точное значение площади пересечения.
long double right_area = M_PI/4 + 5*asin(0.8)/4 - 1;

// Создание и открытие файлов для записи результатов.
std::ofstream csv_n("result.csv");
csv_n.imbue(std::locale::classic());
csv_n << "n;area;area_diff\n";

std::ofstream csv_scale("scale_result.csv");

// Установка локали "C" для использования точки в качестве десятичного разделителя.
csv_scale.imbue(std::locale::classic());
csv_scale << "scale;area;area_diff\n";

for(int n = 100; n <= 100000; n += 500){
    long double area = intersection_area(n, 1.0, c1, c2, c3);
    long double diff = std::abs(area - right_area);
    csv_n << n << ';' << std::fixed << std::setprecision(6) << area << ';' << diff << '\n';
}

// Варьирование масштаба области генерации.
for(long double scale = 1.0; scale < 10.0; scale += 0.1){
    long double scaled_right_area = right_area * scale * scale;
    long double area = intersection_area(1000000, scale, c1, c2, c3);
    long double diff = std::abs(area - right_area); // Предполагаемое масштабирование площади.
    csv_scale << std::fixed << std::setprecision(1) << scale << ';' << std::fixed << std::setprecision(6) << area << ';' << diff << '\n';
}

csv_n.close();
csv_scale.close();


return 0;
}
