#ifndef TREE_H
#define TREE_H

#include <vector>
#include <string>

class Node {
private:
    int Letters;
    std::vector<int> Neighbors;
public:
    Node();
    void SetLetters(int count);
    int GetLetters() const;
    void AddNeighbor(int neighbor);
    const std::vector<int>& GetNeighbors() const;
};

class Tree {
private:
    int N;
    std::vector<Node> Nodes;
public:
    Tree();
    bool LoadFromFile(const std::string& fileName);
    bool LoadFromConsole();
    void GenerateRandom(int n, int maxLetters);
    void PrintData() const;
    int ComputeMinApologies();
};

#endif
