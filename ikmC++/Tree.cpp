#include "Tree.h"
#include <iostream>
#include <fstream>
#include <random>
#include <limits>
#include <queue>
#include <algorithm>

Node::Node() : Letters(0) {}

void Node::SetLetters(int count) {
    Letters = count;
}

int Node::GetLetters() const {
    return Letters;
}

void Node::AddNeighbor(int neighbor) {
    Neighbors.push_back(neighbor);
}

const std::vector<int>& Node::GetNeighbors() const {
    return Neighbors;
}

Tree::Tree() : N(0) {}

bool Tree::LoadFromFile(const std::string& fileName) {
    std::ifstream input(fileName);
    if (!input.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл '" << fileName << "'.\n";
        return false;
    }
    if (!(input >> N)) {
        std::cerr << "Ошибка: файл пустой или содержит некорректные данные.\n";
        return false;
    }
    if (N <= 0 || N >= 1000) {
        std::cerr << "Ошибка: количество гнёзд M должно быть в диапазоне 1..999 (M=" << N << ").\n";
        return false;
    }
    Nodes.assign(N + 1, Node());
    for (int i = 1; i <= N; ++i) {
        int neighborCount, lettersCount;
        if (!(input >> neighborCount >> lettersCount)) {
            std::cerr << "Ошибка: не удалось прочитать данные для гнезда " << i << ".\n";
            return false;
        }
        Nodes[i].SetLetters(lettersCount);
        for (int j = 0; j < neighborCount; ++j) {
            int neighbor;
            if (!(input >> neighbor)) {
                std::cerr << "Ошибка: ожидалось " << neighborCount
                    << " соседей для гнезда " << i << ", получено меньше.\n";
                return false;
            }
            if (neighbor < 1 || neighbor > N) {
                std::cerr << "Ошибка: сосед " << neighbor
                    << " вне диапазона 1.." << N << ".\n";
                return false;
            }
            Nodes[i].AddNeighbor(neighbor);
        }
    }

    long long total = 0;
    for (int i = 1; i <= N; ++i) total += Nodes[i].GetNeighbors().size();
    if (total % 2 != 0 || total / 2 != N - 1) {
        std::cerr << "Ошибка: структура гнёзд не является корректным деревом.\n";
        return false;
    }
    return true;
}

bool Tree::LoadFromConsole() {
    std::cout << "Введите количество гнёзд M: ";
    if (!(std::cin >> N)) {
        std::cerr << "Ошибка: некорректный ввод числа гнёзд.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    if (N <= 0 || N >= 1000) {
        std::cerr << "Ошибка: количество гнёзд M должно быть в диапазоне 1..999.\n";
        return false;
    }
    Nodes.assign(N + 1, Node());
    for (int i = 1; i <= N; ++i) {
        int neighborCount, lettersCount;
        std::cout << "Гнездо " << i << ": введите число соседей и число писем: ";
        if (!(std::cin >> neighborCount >> lettersCount)) {
            std::cerr << "Ошибка: некорректные данные для гнезда " << i << ".\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }
        Nodes[i].SetLetters(lettersCount);
        std::cout << "  Введите номера " << neighborCount << " соседей: ";
        for (int j = 0; j < neighborCount; ++j) {
            int neighbor;
            if (!(std::cin >> neighbor)) {
                std::cerr << "Ошибка: некорректный ввод номера соседа.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return false;
            }
            if (neighbor < 1 || neighbor > N) {
                std::cerr << "Ошибка: сосед " << neighbor << " вне диапазона 1.." << N << ".\n";
                return false;
            }
            Nodes[i].AddNeighbor(neighbor);
        }
    }
    return true;
}

void Tree::GenerateRandom(int n, int maxLetters) {
    N = n;
    Nodes.assign(N + 1, Node());
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 2; i <= N; ++i) {
        std::uniform_int_distribution<> dist(1, i - 1);
        int p = dist(gen);
        Nodes[i].AddNeighbor(p);
        Nodes[p].AddNeighbor(i);
    }
    std::uniform_int_distribution<> ld(0, maxLetters);
    for (int i = 1; i <= N; ++i) {
        Nodes[i].SetLetters(ld(gen));
    }
    std::cout << "Сгенерировано дерево из " << N
        << " гнёзд (макс писем в узле = " << maxLetters << ").\n";
}

int Tree::ComputeMinApologies() {
    if (N == 0) return 0;

    std::vector<int> parent(N + 1, 0), dist(N + 1, -1);
    std::queue<int> q;
    dist[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : Nodes[u].GetNeighbors()) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    std::vector<bool> inSub(N + 1, false);
    int terminals = 0;
    for (int i = 1; i <= N; ++i) {
        if (Nodes[i].GetLetters() > 0) {
            ++terminals;
            int u = i;
            while (!inSub[u]) {
                inSub[u] = true;
                if (u == 1) break;
                u = parent[u];
            }
        }
    }
    int Vp = 0;
    for (int i = 1; i <= N; ++i)
        if (inSub[i]) ++Vp;
    int Ep = std::max(0, Vp - 1);

    int maxD = 0;
    for (int i = 1; i <= N; ++i)
        if (Nodes[i].GetLetters() > 0)
            maxD = std::max(maxD, dist[i]);

    return Ep - maxD;
}
