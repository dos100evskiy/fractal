#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QFile>
#include <QDebug>
#include <complex>
#include <cassert>
#include <iostream>
#include <math.h>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_ITER = 256;

// Функция для фрактала Мандельброта
void drawMandelbrot(QImage& image) {
    assert(image.width() == WIDTH && image.height() == HEIGHT); // Отладочная проверка

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            std::complex<double> c(-2.0 + (x / double(WIDTH)) * 2.5, -1.25 + (y / double(HEIGHT)) * 2.5);
            std::complex<double> z(0, 0);
            int iter = 0;
            while (std::abs(z) < 2.0 && iter < MAX_ITER) {
                z = z * z + c;
                ++iter;
            }
            int color = (iter == MAX_ITER) ? 0 : (255 * iter / MAX_ITER);
            image.setPixelColor(x, y, QColor(
                                    std::clamp(color * 2, 0, 255),
                                    std::clamp(color, 0, 255),
                                    0));
        }
    }
}

// Функция для фрактала Джулии
void drawJulia(QImage& image) {
    assert(image.width() == WIDTH && image.height() == HEIGHT); // Отладочная проверка

    std::complex<double> c(0.285, 0.); // Константа для Джулии
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            std::complex<double> z(-2.0 + (x / double(WIDTH)) * 4.0, -1.5 + (y / double(HEIGHT)) * 3.0);
            int iter = 0;
            while (std::abs(z) < 2.0 && iter < MAX_ITER) {
                z = z * z + c;
                ++iter;
            }
            int color = (iter == MAX_ITER) ? 0 : (255 * iter / MAX_ITER);
            image.setPixelColor(x, y, QColor(
                                    std::clamp(color * 2, 0, 255),
                                    std::clamp(color, 0, 255),
                                    0));

        }
    }
}

// Функция для снежинки Коха (рекурсивный фрактал)
void drawKochCurve(QPainter& painter, QPointF p1, QPointF p2, int depth, int maxDepth) {
    if (depth == 0) {
        int hue = static_cast<int>((p1.x() + p1.y()) * 0.5) % 360;

        painter.setPen(QPen(QColor::fromHsv(hue, 255, 255), 1));

        painter.drawLine(p1, p2);
        return;
    }
    QPointF delta = (p2 - p1) / 3.0;
    QPointF p3 = p1 + delta;
    QPointF p4 = p2 - delta;
    QPointF p5 = p3 + QPointF(-delta.y(), delta.x()) * std::sqrt(3.0) / 2.0 + delta / 2.0; // Поворот на 60 градусов

    drawKochCurve(painter, p1, p3, depth - 1, maxDepth);
    drawKochCurve(painter, p3, p5, depth - 1, maxDepth);
    drawKochCurve(painter, p5, p4, depth - 1, maxDepth);
    drawKochCurve(painter, p4, p2, depth - 1, maxDepth);
}

void drawKochSnowflake(QImage& image) {
    assert(image.width() == WIDTH && image.height() == HEIGHT); // Отладочная проверка

    QPainter painter(&image);
    painter.setPen(QPen(Qt::white, 1));
    painter.fillRect(0, 0, WIDTH, HEIGHT, Qt::black);

    int depth = 10;
    QPointF p1(100, HEIGHT / 2 + 200);
    QPointF p2(WIDTH - 100, HEIGHT / 2 + 200);
    QPointF p3(WIDTH / 2, HEIGHT / 2 - 200 * std::sqrt(3.0) / 2.0);

    drawKochCurve(painter, p1, p2, depth, depth);
    drawKochCurve(painter, p2, p3, depth, depth);
    drawKochCurve(painter, p3, p1, depth, depth);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Создание и сохранение Мандельброта
    QImage mandelbrot(WIDTH, HEIGHT, QImage::Format_RGB32);
    drawMandelbrot(mandelbrot);
    bool saved = mandelbrot.save("mandelbrot.png");
    assert(saved); // Отладочная проверка сохранения
    std::cout << "Mandelbrot saved: " << saved << std::endl;

    // Создание и сохранение Джулии
    QImage julia(WIDTH, HEIGHT, QImage::Format_RGB32);
    drawJulia(julia);
    saved = julia.save("julia.png");
    assert(saved);
    std::cout << "Julia saved: " << saved << std::endl;

    // Создание и сохранение снежинки Коха
    QImage koch(WIDTH, HEIGHT, QImage::Format_RGB32);
    drawKochSnowflake(koch);
    saved = koch.save("koch.png");
    assert(saved);
    std::cout << "Koch saved: " << saved << std::endl;

    return 0; // Без окна, просто генерация файлов
}
