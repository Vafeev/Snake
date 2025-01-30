//Логика приложения
// серверная часть

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>

// Игровое поле
const int FIELD_WIDTH = 50;
const int FIELD_HEIGHT = 30;

// Змейка
struct Snake {
    std::vector<std::pair<int, int>> body;
    int direction; // 0 - вверх, 1 - вправо, 2 - вниз, 3 - влево
};

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

int main() {
    // Генерация начальной еды
    generateFood();

    // Основной игровой цикл
    while (true) {
        updateGame();
        // Отправка обновленного состояния клиентам
        // ...
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}







// клиентская часть

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

// Игровое поле
const int FIELD_WIDTH = 50;
const int FIELD_HEIGHT = 30;

// Змейка
struct Snake {
    std::vector<std::pair<int, int>> body;
    int direction; // 0 - вверх, 1 - вправо, 2 - вниз, 3 - влево
};

// Состояние игры
Snake mySnake;
std::pair<int, int> food;
std::vector<Snake> otherSnakes;

// Функция для управления змейкой
void controlSnake() {
    while (true) {
        // Получение ввода от пользователя
        char input;
        std::cin >> input;

        // Обновление направления змейки
        switch (input) {
            case 'w': mySnake.direction = 0; break;
            case 'd': mySnake.direction = 1; break;
            case 's': mySnake.direction = 2; break;
            case 'a': mySnake.direction = 3; break;
        }

        // Отправка обновленного состояния змейки на сервер
        // ...
    }
}

int main() {
    // Подключение к серверу
    // ...

    // Создание змейки
    mySnake.body.push_back(std::make_pair(FIELD_WIDTH / 2, FIELD_HEIGHT / 2));
    mySnake.direction = 0;

    // Запуск потока для управления змейкой
    std::thread controlThread(controlSnake);

    // Основной игровой цикл
    while (true) {
        // Получение обновленного состояния игры с сервера
        // ...

        // Отрисовка игрового поля
        system("cls");
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            for (int x = 0; x < FIELD_WIDTH; x++) {
                bool drawn = false;
                for (const auto& segment : mySnake.body) {
                    if (segment.first == x && segment.second == y) {
                        std::cout << "O";
                        drawn = true;
                        break;
                    }
                }
                if (!drawn) {
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    controlThread.join();
    return 0;
}
