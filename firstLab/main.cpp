//
// Created by meteo on 11.12.2023.
//
#include "SimpleIni.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <windows.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>

using namespace std;

bool hasCycle(const std::string &job,
              const std::unordered_map<std::string, std::vector<int>> &dependencies,
              std::unordered_map<std::string, bool> &visited,
              std::unordered_map<std::string, bool> &inStack) {
    if (!visited[job]) {
        visited[job] = true;
        inStack[job] = true;

        for (const auto &dep: dependencies.at(job)) {
            std::string dep_str = std::to_string(dep);
            if (!visited[dep_str] && hasCycle(dep_str, dependencies, visited, inStack))
                return true;
            else if (inStack[dep_str])
                return true;
        }
    }

    inStack[job] = false;
    return false;
}

bool hasCycle(const std::unordered_map<std::string, std::vector<int>> &dependencies) {
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, bool> inStack;

    for (const auto &job: dependencies) {
        if (hasCycle(job.first, dependencies, visited, inStack))
            return true;
    }

    return false;
}

void DepthFirstSearch(const std::string &node,
                      const std::unordered_map<std::string, std::vector<int>> &graph,
                      std::unordered_map<std::string, bool> &visited) {
    visited[node] = true;
    for (const auto &neighbor: graph.at(node)) {
        std::string neighbor_str = std::to_string(neighbor);
        if (!visited[neighbor_str]) {
            DepthFirstSearch(neighbor_str, graph, visited);
        }
    }
}

bool hasOnlyOneComponent(const std::unordered_map<std::string, std::vector<int>> &graph) {
    std::unordered_map<std::string, bool> visited;
    int components = 0;

    for (const auto &node: graph) {
        if (!visited[node.first]) {
            DepthFirstSearch(node.first, graph, visited);
            components++;
            if (components > 1) {
                return false;
            }
        }
    }

    return true;
}

bool hasStartAndEnd(const std::unordered_map<std::string, std::vector<int>> &graph) {
    std::unordered_set<std::string> nodesWithIncomingEdges;
    std::unordered_set<std::string> nodesWithOutgoingEdges;

    for (const auto &node: graph) {
        if (!node.second.empty()) {
            nodesWithOutgoingEdges.insert(node.first);
            for (const auto &edge: node.second) {
                nodesWithIncomingEdges.insert(std::to_string(edge));
            }
        }
    }

    std::string startNode, endNode;
    for (const auto &node: graph) {
        if (nodesWithIncomingEdges.find(node.first) == nodesWithIncomingEdges.end()) {
            if (!startNode.empty()) return false;  // More than one start node
            startNode = node.first;
        }
        if (nodesWithOutgoingEdges.find(node.first) == nodesWithOutgoingEdges.end()) {
            if (!endNode.empty()) return false;  // More than one end node
            endNode = node.first;
        }
    }

    return !startNode.empty() && !endNode.empty();  // There is a start and an end node
}

void runProcess(const std::unordered_map<std::string, std::vector<int>> &graph, int maxParallel) {
    std::unordered_map<std::string, int> inDegree;
    for (const auto &node: graph) {
        inDegree[node.first] = 0; // Инициализация входящих степеней
    }

    for (const auto &node: graph) {
        for (const auto &edge: node.second) {
            inDegree[std::to_string(edge)]++; // Вычисление входящих степеней
        }
    }

    std::queue<std::string> q;
    for (const auto &node: graph) {
        if (inDegree[node.first] == 0) {
            q.push(node.first); // Добавление вершин с нулевой входящей степенью в очередь
        }
    }

    int runningProcesses = 0;
    while (!q.empty() || runningProcesses > 0) {
        while (!q.empty() && runningProcesses < maxParallel) {
            std::string node = q.front();
            q.pop();

            // Запуск процесса
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            // Замените "MyProgram.exe" на имя вашего исполняемого файла
            if (!CreateProcess("MyProgram.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                std::cerr << "CreateProcess failed ";
                return;
            }

            runningProcesses++;

            // Закрытие дескрипторов процесса и основного потока
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            for (const auto &edge: graph.at(node)) {
                inDegree[std::to_string(edge)]--; // Уменьшение входящих степеней для следующих вершин
                if (inDegree[std::to_string(edge)] == 0) {
                    q.push(std::to_string(edge)); // Добавление вершин с нулевой входящей степенью в очередь
                }
            }
            // Ожидание завершения процессов
            WaitForSingleObject(pi.hProcess, INFINITE);
            runningProcesses--;
        }
    }
}


int main() {
    CSimpleIniA ini;
    ini.LoadFile("config.ini");

    std::unordered_map<std::string, std::vector<int>> map;

    CSimpleIniA::TNamesDepend sections;
    ini.GetAllSections(sections);

    for (const auto &section: sections) {
        std::string key = section.pItem;

        std::string value = ini.GetValue(key.c_str(), "dependencies", "");
        std::istringstream basicIstringstream(value);
        std::vector<int> dependencies;

        int num;
        while (basicIstringstream >> num) {
            dependencies.push_back(num);
            if (basicIstringstream.peek() == ',')
                basicIstringstream.ignore();
        }

        map[key] = dependencies;
    }

    runProcess(map, 2);

    return 0;
}