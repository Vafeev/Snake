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
}