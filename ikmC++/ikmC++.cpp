#include "Tree.h"
#include <iostream>
#include <string>
#include <clocale>
#include <limits>

int main() {
    std::setlocale(LC_ALL, "rus");
    Tree tree;

    while (true) {
        std::cout << "\tМеню:\n"
            << "1. Загрузка из файла\n"
            << "2. Ввод из консоли\n"
            << "3. Генерация случайного дерева\n"
            << "4. Выход\n"
            << "Выберите пункт: ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cerr << "Ошибка ввода выбора. Попробуйте снова.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        bool loaded = false;
        switch (choice) {
        case 1: {
            std::cout << "Введите имя файла: ";
            std::string fileName;
            std::getline(std::cin, fileName);
            loaded = tree.LoadFromFile(fileName);
            break;
        }
        case 2:
            loaded = tree.LoadFromConsole();
            break;
        case 3: {
            int n, maxLetters;
            std::cout << "Введите число гнёзд и максимум писем: ";
            if (!(std::cin >> n >> maxLetters)) {
                std::cerr << "Ошибка: некорректные параметры генерации.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            tree.GenerateRandom(n, maxLetters);
            tree.PrintData();
            loaded = true;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        case 4:
            std::cout << "Выход. Пока!\n";
            return 0;
        default:
            std::cout << "Неверный выбор, попробуйте снова.\n";
            continue;
        }

        if (!loaded) continue;
        int result = tree.ComputeMinApologies();
        std::cout << "Минимальное количество извинений: " << result << "\n";
    }
}
