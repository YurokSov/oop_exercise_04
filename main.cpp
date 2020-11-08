#include <iostream>
#include <utility>
#include <tuple>
#include <cmath>

namespace figs {
    const double PI = 3.14159265;
    template<typename T>
    using vertex = std::pair<T, T>;
    template<typename T>
    struct Triangle {
        vertex<T> vertices[3];
    };
    template<typename T>
    struct Square {
        vertex<T> vertices[4];
    };
    template<typename T>
    struct Rectangle {
        vertex<T> vertices[4];
    };
    template<typename T, template<typename> typename F>
    void Read(F<T> &f) {
        double alpha = 0;
        int k = 0;
        if (std::is_same<F<T>, Square<T>>::value) {
            alpha = 90.0 / 180.0 * PI;
            k = 4;
        } else if (std::is_same<F<T>, Triangle<T>>::value) {
            alpha = 120.0 / 180.0 * PI;
            k = 3;
        } else if (std::is_same<F<T>, Rectangle<T>>::value) {
            alpha = 90.0 / 180.0 * PI;
            k = 4;
        } else {
            std::cout << "Bad figure!";
        }
        vertex<double> center;
        std::cin >> center.first >> center.second >> f.vertices[0].first >> f.vertices[0].second;
        for (int i = 1; i < k; ++i) {
            double a = -sin(alpha) * (static_cast<double>(f.vertices[i - 1].second) - center.second)
                       + cos(alpha) * (static_cast<double>(f.vertices[i - 1].first) - center.first) + center.first;
            double b = cos(alpha) * (static_cast<double>(f.vertices[i - 1].second) - center.second)
                       + sin(alpha) * (static_cast<double>(f.vertices[i - 1].first) - center.first) + center.second;
            if constexpr (std::is_same<T, int>::value) {
                f.vertices[i].first = static_cast<T>(std::round(a));
                f.vertices[i].second = static_cast<T>(std::round(b));
            } else {
                f.vertices[i].first = a;
                f.vertices[i].second = b;
            }
        }
    }
    template<typename T>
    T dist(const vertex<T> &a, const vertex<T> &b) {
        return static_cast<T>(sqrt(
                (a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second)));
    }
    template<typename T, template<typename> typename F>
    typename std::enable_if<std::is_same<F<T>, Rectangle<T>>::value>::type PrintFigure(F<T> &a) {
        std::cout << "Rectangle: ";
        for (int i = 0; i < 4; ++i) {
            std::cout << '[' << a.vertices[i].first << ',' << a.vertices[i].second << ']' << ' ';
        }
        std::cout << std::endl;
    }
    template<typename T, template<typename> typename F>
    typename std::enable_if<std::is_same<F<T>, Square<T>>::value>::type PrintFigure(F<T> &a) {
        std::cout << "Square: ";
        for (int i = 0; i < 4; ++i) {
            std::cout << '[' << a.vertices[i].first << ',' << a.vertices[i].second << ']' << ' ';
        }
        std::cout << std::endl;
    }
    template<typename T, template<typename> typename F>
    typename std::enable_if<std::is_same<F<T>, Triangle<T>>::value>::type PrintFigure(F<T> &a) {
        std::cout << "Triangle: ";
        for (int i = 0; i < 3; ++i) {
            std::cout << '[' << a.vertices[i].first << ',' << a.vertices[i].second << ']' << ' ';
        }
        std::cout << std::endl;
    }
    template<typename T>
    void PrintFigure(T &a) {
        std::cout << "Bad Figure!\n";
    };
    template<typename T, template<typename> typename F>
    typename std::enable_if<std::is_same<F<T>, Rectangle<T>>::value, double>::type GetFigureArea(F<T> &a) {
        return dist(a.vertices[0], a.vertices[1]) * dist(a.vertices[2], a.vertices[3]);
    }
    template<typename T, template<typename> typename F>
    typename std::enable_if<std::is_same<F<T>, Square<T>>::value, double>::type GetFigureArea(F<T> &a) {
        return dist(a.vertices[0], a.vertices[1]) * dist(a.vertices[2], a.vertices[3]);
    }
    template<typename T, template<typename> typename F>
    typename std::enable_if<std::is_same<F<T>, Triangle<T>>::value, double>::type GetFigureArea(F<T> &a) {
        T A = dist(a.vertices[0], a.vertices[1]);
        T B = dist(a.vertices[1], a.vertices[2]);
        T C = dist(a.vertices[2], a.vertices[0]);
        double p = (A + B + C) / 2.0;
        double x = p * (p - A) * (p - B) * (p - C);
        return sqrt(x);
    }
    template<typename T>
    double GetFigureArea(T &a) {
        //std::cout << "Bad Figure: ";
        return -1;
    }
    template<typename F, size_t index>
    typename std::enable_if<index < 0, double>::type ProcessTuple(F &t) {
        return 0;
    }
    template<typename F, size_t index>
    typename std::enable_if<index >= std::tuple_size<F>::value, double>::type ProcessTuple(F &t) {
        return 0;
    }
    template<typename F, size_t index = 0>
    typename std::enable_if<index < std::tuple_size<F>::value, double>::type ProcessTuple(F &t) {
        auto v = std::get<index>(t);
        PrintFigure(v);
        double area = GetFigureArea(v);
        if (area >= 0) {
            std::cout << "\tFigure Area: " << area << '\n';
        } else {
            std::cout << "\tError!\n";
        }
        return area + ProcessTuple<F, index + 1>(t);
    }
}

int main() {
    bool f = true;
    while (f) {
        f = false;
        std::cout << "1. Целочисленные координаты\n2. Вещественные координаты" << std::endl;
        int option;
        std::cin >> option;
        if (option == 1) {
            figs::Square<int> a;
            figs::Triangle<int> b;
            figs::Rectangle<int> c;
            std::cout << "Введите координаты для квадрата (центр и вершина): \n";
            Read(a);
            std::cout << "Введите координаты для треугольника (центр и вершина): \n";
            Read(b);
            std::cout << "Введите координаты для прямоугольника (центр и вершина): \n";
            Read(c);
            auto t = std::make_tuple(a, b, c);
            double area = figs::ProcessTuple(t);
            std::cout << "Total area: " << area << std::endl;
        } else if (option == 2) {
            figs::Square<double> a;
            figs::Triangle<double> b;
            figs::Rectangle<double> c;
            std::cout << "Введите координаты для квадрата (центр и вершина): \n";
            Read(a);
            std::cout << "Введите координаты для треугольника (центр и вершина): \n";
            Read(b);
            std::cout << "Введите координаты для прямоугольника (центр и вершина): \n";
            Read(c);
            auto t = std::make_tuple(a, b, c);
            double area = figs::ProcessTuple(t);
            std::cout << "Total area: " << area << std::endl;
        } else {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        if (!std::cin.fail()) f = true;
        //std::tuple<figs::Square<int>, figs::Triangle<int>, figs::Rectangle<int>> t;
    }
    return 0;
}