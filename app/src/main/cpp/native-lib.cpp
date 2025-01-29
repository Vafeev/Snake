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
    // Функция для обновления состояния игры
    void updateGame() {
        // Обновление позиций змеек
        clients_mutex.lock();
        for (auto& client : clients) {
            // Движение змейки
            int newX = client.body[0].first;
            int newY = client.body[0].second;
            switch (client.direction) {
            case 0: newY--; break;
            case 1: newX++; break;
            case 2: newY++; break;
            case 3: newX--; break;
            }
            client.body.insert(client.body.begin(), std::make_pair(newX, newY));
            client.body.pop_back();

            // Проверка на столкновение
            for (size_t i = 1; i < client.body.size(); i++) {
                if (client.body[0] == client.body[i]) {
                    // Змейка врезалась в себя, игра окончена
                    clients.erase(clients.begin() + (&client - &clients[0]));
                    break;
                }
            }

            // Проверка на сбор еды
            if (client.body[0] == food) {
                client.body.push_back(client.body.back());
                generateFood();
            }
        }
        clients_mutex.unlock();
    }

}