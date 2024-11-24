#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
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

bool is_point_inside_intersection(long double x, long double y, const Circle& c1, const Circle& c2, const Circle& c3) {
    return c1.contains(x, y) && c2.contains(x, y) && c3.contains(x, y);
}

int main() {
    Circle circles[3];
    for (int i = 0; i < 3; ++i){
        std::cin >> circles[i].x >> circles[i].y >> circles[i].r;
    }

    long double min_x = std::min({circles[0].x - circles[0].r, circles[1].x - circles[1].r, circles[2].x - circles[2].r});
    long double max_x = std::max({circles[0].x + circles[0].r, circles[1].x + circles[1].r, circles[2].x + circles[2].r});
    long double min_y = std::min({circles[0].y - circles[0].r, circles[1].y - circles[1].r, circles[2].y - circles[2].r});
    long double max_y = std::max({circles[0].y + circles[0].r, circles[1].y + circles[1].r, circles[2].y + circles[2].r});

    long double area_rect = (max_x - min_x) * (max_y - min_y);

    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<long double> dist_x(min_x, max_x);
    std::uniform_real_distribution<long double> dist_y(min_y, max_y);

    const long long num = 5000000;
    long long count_inside = 0;

    for(long long i = 0; i < num; ++i){
        long double x = dist_x(rng);
        long double y = dist_y(rng);
        if(is_point_inside_intersection(x, y, circles[0], circles[1], circles[2])){
            ++count_inside;
        }
    }

    long double estimated_area = (static_cast<long double>(count_inside) / num) * area_rect;

    std::cout << estimated_area << std::endl;

    return 0;
}
