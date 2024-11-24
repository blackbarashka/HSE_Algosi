#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>

// Окружность: центр (x, y) и радиус r.
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

// Функция для генерации случайного числа в диапазоне [min, max].
long double random_double(long double min, long double max, std::mt19937 &gen, std::uniform_real_distribution<> &dist) {
    return dist(gen) * (max - min) + min;
}

int main() {
    // Заданные окружности.
    Circle c1 = {1.0, 1.0, 1.0};
    Circle c2 = {1.5, 2.0, std::sqrt(5.0)/2.0};
    Circle c3 = {2.0, 1.5, std::sqrt(5.0)/2.0};

    // Определение области генерации точек.
    // Найдём минимальные и максимальные координаты, охватывающие все три окружности.
    long double min_x = std::min(c1.x - c1.r, std::min(c2.x - c2.r, c3.x - c3.r));
    long double max_x = std::max(c1.x + c1.r, std::max(c2.x + c2.r, c3.x + c3.r));
    long double min_y = std::min(std::min(c1.y - c1.r, c2.y - c2.r), c3.y - c3.r);
    long double max_y = std::max(std::max(c1.y + c1.r, c2.y + c2.r), c3.y + c3.r);

    long double area_rect = (max_x - min_x) * (max_y - min_y);

    // Настройка генератора случайных чисел.
    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    // Точное значение площади пересечения.
    long double exact_S = 0.25 * M_PI + 1.25 * std::asin(0.8) - 1.0;

    // Открытие файла для записи результатов.
    std::ofstream outfile("results.csv");
    if (!outfile.is_open()) {
        std::cerr << "Ошибка при открытии файла для записи.\n";
        return 1;
    }

    // Запись заголовка в файл.
    outfile << "N;area;areaf;Relative deviation (%)\n";

    // Вывод заголовка таблицы в консоль.
    std::cout << std::setw(10) << "N"
              << std::setw(20) << "area"
              << std::setw(20) << "areaf"
              << std::setw(25) << "Relative deviation (%)" << "\n";

    // Перебор значения N от 100 до 100000 с шагом 500.
    for(int N = 100; N <= 100000; N += 500) {
        int count = 0;
        for(int i = 0; i < N; ++i) {
            long double px = random_double(min_x, max_x, gen, dist);
            long double py = random_double(min_y, max_y, gen, dist);
            if(c1.contains(px, py) && c2.contains(px, py) && c3.contains(px, py)) {
                count++;
            }
        }
        long double estimated_S = ((long double)count / N) * area_rect;
        long double difference = std::abs(estimated_S - exact_S);
        long double relative_deviation = (difference / std::abs(exact_S)) * 100.0; // В процентах

        // Вывод данных в консоль.
        std::cout << std::setw(10) << N
                  << std::setw(20) << std::fixed << std::setprecision(6) << estimated_S
                  << std::setw(20) << difference
                  << std::setw(25) << relative_deviation << "\n";

        // Запись данных в файл.
        outfile << N << ";"
                << std::fixed << std::setprecision(6) << estimated_S << ";"
                << difference << ";"
                << relative_deviation << "\n";
    }

    // Закрытие файла.
    outfile.close();

    // Вывод точного значения площади.

    std::cout << "\nThe exact value of the intersection area: " << exact_S << "\n";
    std::cout << "The results are recorded in the results.csv file\n";

    return 0;
}
