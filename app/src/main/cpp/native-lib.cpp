#include <jni.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_snake_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    / Игровое поле
    const int FIELD_WIDTH = 50;
    const int FIELD_HEIGHT = 30;

// Змейка
    struct Snake {
        std::vector<std::pair<int, int>> body;
        int direction; // 0 - вверх, 1 - вправо, 2 - вниз, 3 - влево
    }
    // Еда
    std::pair<int, int> food;
// Клиенты
    std::vector<Snake> clients;
    std::mutex clients_mutex;

// Генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, FIELD_WIDTH - 1);
    std::uniform_int_distribution<> dis2(0, FIELD_HEIGHT - 1);

// Функция для генерации новой еды
    void generateFood() {
        food = std::make_pair(dis(gen), dis2(gen));
    }

}